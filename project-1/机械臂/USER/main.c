#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"	    				//包含stm32库系统头文件
#include "servor.h"							//包含GPIO库头文件
#include "usart.h"		  					//包含串口通信设置头文件
#include "delay.h"						//包含系统定时器库头文件
#include "timer.h"					//包含定时器设置头文件
#include "PS2.h"
#include "key.h"
#include "led.h"
#include "beep.h"
#include "flash.h"
#include "common.h"
#include "adc.h"
#include "sensor.h"

#define SIZE sizeof(TEXT_Buffer)	
	
const u8 TEXT_Buffer[]={"Flash SPI TEST"};
u8 yysb_init[] = {0xa0, 0xa0, 0xa0};	//初始化
u8 yysb_mul[] = {0xab, 0xab, 0x00};		//启动连续识别
u8 yysb_exit[] = {0xac,0xac,0x00};
u8 yysb_rmul=0;
u8 yysb_flag=0;
u8 datatemp[SIZE];
u32 FLASH_SIZE;
u8 date[3];									//该变量用来存储安卓终端发来的数据
u32 value;									
u32 key,key_bak;
extern u32 a;		  						//a用来计数，配合系统滴答定时器可检测代码运行时间
extern uint8 flag_RecFul;
uint16 pos[7][MOTOR_NUM]={ {1000,1500,1500,1500,1500,1500,1500,1500,1500},
					 {1000,1500,1500,1500,1500,1500,1500,1500,1500},
					 {1000,1500,1500,1500,1500,1500,1500,1500,1500},
					 {1000,500,500,500,500,500,500,500,500},
					 {1000,1500,1500,1500,1500,1500,1500,1500,1500},
					 {1000,500,500,500,500,500,500,500,500},
					 {1000,1500,1500,1500,1500,1500,1500,1500,1500}
};	 //位置
uint16 CPWM[MOTOR_NUM]=   {1500,1500,1500,1500,1500,1500,1500,1500,1500};
uint16 UartRec[MOTOR_NUM]={1500,1500,1500,1500,1500,1500,1500,1500,1500};
u8 yysb_start,Total_index;
unsigned int dj1=1500;
unsigned int dj2=1500;
unsigned int dj3=1500;
unsigned int dj4=1500;
unsigned int dj5=1500;
unsigned int dj6=1600;
unsigned char buf[30];

uint8 line=0;					//缓存存入口与出口之间的距离，即当前缓存中有多少个没有执行的数据
char point_now=0;				//与point_aim一起标记缓存出口位置，即取数位置
char point_aim=1;
char point_in=1;				//标记缓存入口位置，即上一行数据存放位置
uint8 flag_connect = 0;
uint8 flag_stop=1;				//表示一行执行结束
uint8 flag_vpwm=0;				//表示到达了该更新pwm[]的时间
uint8 flag_in=1;		 			//表示缓存中有空闲空间
uint8 flag_out=0;					//表示缓存中有可执行数据的标志位
uint8 flag_run_ready=0;			//表示有要要放入缓存的EErom数据

uint16 n=1000;					//用来计算需要建立多少个中间数据
uint16 m=1;						//用来累计已经执行了多少中间数据
double dp;
double dp0[MOTOR_NUM] = {0};					//插补增量
uint8 flag_download = 0;//判断是否下载
uint8 flag_All_download = 0;//判断是否批量下载命令
uint8 flag_All_Stop_Down=0;//标志位1
uint8 flag_read = 0;// 读取flash内容，发送上位机
uint8 flag_connect_run = 0;//连接上位机的执行flash保存的命令
uint8 flag_stop_download = 0;//停止下载
uint8 flag_online_run = 0;
uint8 flag_uart2_rev = 0;
uint8 flag_uart2_rev_time_out = 0;
uint8 flag_ps2_rev = 0;
char redata[257] = {0};    // 定义接收数据变量数组

unsigned long send_mode = 0;//当前串口接收到命令状态存储
MotorData motor_data;//舵机总信息
MotorOneCmd motor_one_cmd;//单个命令
CurrentItem cur_item;
uint16 tuoji_count = 0;//脱机执行次数
unsigned char flag_scan_ps2 = 0;
uint8 flag_adc=0;
uint8 flag_sensor=0;
uint8 error = 0;
uint8 file_list[MAX_SUPPORT_FILE_SAVE] = {0};
int file_list_count = 0;
int file_last_num = 1;
unsigned char ps2_buf[120] = {0};
char uart2_buf[50] = {0};
uint16 cur_count = 0;
uint16 adc_value=0;
u8 ps2_mode=0;
char csb=1;
char All_down_num=-1;
uint8 Flag_Flash=0;
void SerVor_Test(void);
void Ps2_Test(void);
void Flash_Test(void);
void updata_file_list(void);
void InitMotor(void);
void vpwm(void);


/**********************************************************************************/
/*****************************主函数***********************************************/
int main(void)
{   
	SysTick_Init();									   //系统滴答定时器初始化
	Servor_GPIO_Config();									   //GPIO初始化
  Uart_Init(1);
  Uart_Init(2);	
	Uart_Init(3);
	Timer_Init();									   //定时器初始化
	Timer_ON();										   //开启定时器	 		
	LED_Init();	
	Beep_Init();
	PS2_Init();
	Adc_Init();
	Sensor_init();
	USART_Config(USART1,115200); 
	USART_Config(USART2,115200); 
	USART3_Config(115200);
	SpiFlashInit();  		//初始化flash	 	
	while(SpiFlashReadID()!=W25Q64)							//检测不到W25Q64
	{
		UART_PutStr (USART1,"Not Found W25Q64\r\n");
	}
	Led_Test();
	Beep_Test();
	Delay_ms(500);
	Beep_Test();
  InitMotor();
	
	while (1)
	{	
		static unsigned int i=0;
		static unsigned int z=1;
		u8 Res;		
		if(flag_vpwm==1)		  
		{	
			vpwm();					//更新pwm[]数组
			flag_vpwm=0;
		}
		if(flag_RecFul==1)		   //串口接受完一条指令
		{
			
		 	DealRec();	 			//处理串口缓存中的数据
			flag_RecFul=0;
		}
		GetOneMotorCMD();//获取一个命令
		SendUartState();//发送状态信息
	}
} 


void updata_file_list(void)
{
	unsigned char i = 0;
	unsigned char j = 0;
	file_last_num = -1;
	ReadMoterInfor();
	for (i = 0; i < motor_data.filecount; i++)
	{
		if (motor_data.file_flag[i] == 1)
		{
			file_list[j] = i;
			j++;
			file_last_num = i;
		}
	}
	file_list_count = j;
}
void InitMotor(void)
{
	ReadMoterInfor();//读取舵机控制信息
	updata_file_list();
	memset(&cur_item,0,sizeof(cur_item));
}

/***************************************************************************************************************
函 数 名：从flash读取舵机总的信息
功能描述：初始化舵机控制信息
输入参数：无  
返 回 值：无  
备    注：
****************************************************************************************************************/	
void ReadMoterInfor(void)
{
	memset(&motor_data,0,sizeof(motor_data));//清 0
	SpiFlashRead((unsigned char *)&motor_data,(CMD_FLASH_ADDR)<<WRITE_BIT_DEPTH,sizeof(motor_data));//读取信息
	if (motor_data.CRC1 != 0x12345678 || motor_data.sum < 0 || motor_data.duoji_count  > MOTOR_NUM)//判断信息存储是否有错
	{
		memset(&motor_data,0,sizeof(motor_data));
		//memset(&cur_item,0,sizeof(cur_item));
	}
	else//正常信息
	{
	//	UART1_SendOneChar(motor_data.sum + 0x30);
		//cur_item.tuoji_count = motor_one_cmd.tuoji_count;//脱机运行次数
		//cur_item.cur_num = 0;//清 0
	}
		
}
void ReadOneCmdInfor(unsigned int addr)
{
	memset(&motor_one_cmd,0,sizeof(motor_one_cmd));//清 0
	SpiFlashRead((unsigned char *)&motor_one_cmd,((((unsigned long)addr)<<4) + FILE_FLASH_ADDR)<<WRITE_BIT_DEPTH,sizeof(motor_one_cmd));//读取信息
	if (motor_one_cmd.start >= motor_one_cmd.end || motor_one_cmd.cur_file_num != (addr) || motor_data.file_flag[motor_one_cmd.cur_file_num] == 0)//判断信息存储是否有错
	{
		memset(&motor_one_cmd,0,sizeof(motor_one_cmd));
	}
	else//正常信息,以后留着验证用的
	{
		cur_item.tuoji_count = motor_one_cmd.tuoji_count;//脱机运行次数
		cur_item.cur_num = motor_one_cmd.start;
	//	UART1_SendOneChar(motor_one_cmd.tuoji_count + 0x30);
	//	cur_item.cur_num = 0;//清 0
	}
}
/***************************************************************************************************************
函 数 名：把舵机信息写到flash中
功能描述：把舵机信息写到flahs中
输入参数：无  
返 回 值：无  
备    注：
****************************************************************************************************************/	
void WriteMoterInfor(void)
{
	unsigned char temp =0;
	motor_data.CRC1 = 0x12345678;//校验码
	motor_data.duoji_count = MOTOR_NUM-1;
	temp = motor_data.filecount;
	SpiFlashEraseSector(CMD_FLASH_ADDR >> 4);//擦除以前存储的信息
	SpiFlashWrite((unsigned char *)&motor_data,CMD_FLASH_ADDR<<WRITE_BIT_DEPTH,sizeof(motor_data)); //写入flash
	ReadMoterInfor();
	if (temp != motor_data.filecount)
	{
		error |= ERROR_FLASH_WRITE;
	}
	else
	{
		error &= ~ERROR_FLASH_WRITE;
	}
}
void WriteOneCmdInfor(unsigned int addr)
{
	unsigned char temp = 0;
	temp = motor_one_cmd.end;
	if (((((unsigned long)addr)<<4) + FILE_FLASH_ADDR) % 16 == 0)
	SpiFlashEraseSector(((((unsigned long)addr)<<4) + FILE_FLASH_ADDR) >> 4);//擦除以前存储的信息
	SpiFlashWrite((unsigned char *)&motor_one_cmd,((((unsigned long)addr)<<4) + FILE_FLASH_ADDR)<<WRITE_BIT_DEPTH,sizeof(motor_one_cmd)); //写入flash
	ReadOneCmdInfor(addr);
	if (temp !=  motor_one_cmd.end)
	{
		error |= ERROR_FLASH_WRITE1;
	}
	else
	{
		error &= ~ERROR_FLASH_WRITE1;
	}
}
/***************************************************************************************************************
函 数 名：获取一串舵机的控制字符串
功能描述：处理从flash读取的舵机控制字符串处理
输入参数：无  
返 回 值：无  
备    注：
****************************************************************************************************************/	
void GetOneMotorCMD(void)
{
#if DEBUG
	uchar buf[20] = {0};
#endif
	if(flag_stop_download)//接收到了上位机的停止下载的命令
	{
		flag_download = 0;//清楚下载状态标志位
		flag_stop_download = 0;
		flag_read = 0;
		if(motor_data.filecount < MAX_SUPPORT_FILE_SAVE)
		{
			updata_file_list();
			motor_data.sum = motor_one_cmd.end;
			motor_data.file_flag[motor_data.filecount] = 1;
			motor_one_cmd.cur_file_num = file_last_num + 1;
			motor_data.filecount = motor_one_cmd.cur_file_num + 1;
			error &= ~MAX_SUPPORT_FILE_SAVE;
#if DEBUG		
			sprintf(buf,"%d %d\r\n",(unsigned int)motor_data.filecount,(unsigned int)motor_data.file_flag[motor_data.filecount-1]);
			UART_PutStr(USART1,buf);
#endif
			WriteMoterInfor();
			WriteOneCmdInfor(motor_one_cmd.cur_file_num);
			updata_file_list();
#if DEBUG
			sprintf(buf,"%d %d\r\n",(uint)motor_data.filecount,(uint)motor_data.file_flag[motor_data.filecount-1]);
			UART_PutStr(USART1,buf);
#endif
		}
		else
		{
			error |= MAX_SUPPORT_FILE_SAVE;
		}
		if	(!(error &(MAX_SUPPORT_FILE_SAVE | ERROR_FLASH_FULL)))
			send_mode |= SEND_DOWN_OK;//状态位置为
	}
	if (flag_connect)//如果当前跟上位机联机状态
	{
		if (flag_read)//如果上位机读取flash内存储的信息
		{		
			if (cur_item.cur_num < motor_one_cmd.end)//判断是否超过之前存储的数
			{
				if ((send_mode & SEND_SEND_FILE))//开始接收到读取命令需要先发送个start
				{
					UART_PutStr(USART1,"#Start\r\n");
					send_mode &= ~SEND_READ_FILE;
				}
				memset(redata,0,WRITE_SIZE);//清 0
				SpiFlashRead((unsigned char *)redata,(((long)cur_item.cur_num)<<WRITE_BIT_DEPTH),WRITE_SIZE);//读取信息
#if DEBUG
				sprintf(buf,"%d\r\n",cur_item.cur_num);
				UART_PutStr(USART1,buf);
#endif
				UART_PutStr(USART1,redata);//发送
				cur_item.cur_num++;
			}
			else//否则
			{
				if (cur_item.cur_num > 0)
					UART_PutStr(USART1,"#End\r\n");//发送结束字符串
				flag_read = 0;
			}
			send_mode = 0;//请 0
		}	
		if (flag_online_run)
		{
			//	UART_PutStr(USART1,"111\r\n");
			if ((send_mode & SEND_CC) != 0  || cur_item.cur_num == motor_one_cmd.start)//如果当前需要更新舵机命令
			{
				if (cur_item.tuoji_count > 0)//脱机次数没结束
				{
					if (cur_item.cur_num < motor_one_cmd.end)//判断是否读取结束
					{
						SpiFlashRead((unsigned char *)redata,((long)cur_item.cur_num)<<WRITE_BIT_DEPTH,WRITE_SIZE);//读取命令
						//UART_PutStr(USART1,redata);
						//UART_Put_Num(cur_item.cur_num);
					//	UART_Put_Num(motor_one_cmd.end);
						flag_RecFul = 1;//标志位为1，
						cur_item.cur_num++;//
					}
					else//执行玩一遍
					{
						cur_item.cur_num = motor_one_cmd.start;
						cur_item.tuoji_count--;//减一
					}
				}
				else//执行完成
				{
				//	UART_PutStr(USART1,"444\r\n");
					flag_online_run = 0;
					if (flag_connect_run)//如果上位机选择执行的功能，需要发送 AGF作为结束
					{
						UART_PutStr(USART1,"#AGF\r\n");
						flag_connect_run = 0;
					}
				}
				//读取数据
			}
		}
	} 
	else//脱机
	{
		if (file_list_count < 0)
		{
			return;
		}
		if (cur_item.tuoji_count > 0)
		{
			if ((send_mode & SEND_CC) != 0  || cur_item.cur_num == motor_one_cmd.start)//如果当前需要更新舵机命令
			{
				if (cur_item.cur_num < motor_one_cmd.end)//判断是否读取结束
				{
					SpiFlashRead((unsigned char *)redata,((long)cur_item.cur_num)<<WRITE_BIT_DEPTH,WRITE_SIZE);//读取命令
					flag_RecFul = 1;//标志位为1，
					cur_item.cur_num++;//
				}
				else//执行玩一遍
				{
					cur_item.cur_num = motor_one_cmd.start;
					cur_item.tuoji_count--;//减一
				}
				//读取数据
			}
		}
		else
		{
			ReadOneCmdInfor(file_list[cur_item.file_num]);
			file_list_count--;
			cur_item.file_num++;
		}
	}
}
/***************************************************************************************************************
函 数 名：发送串口状态信息
功能描述：根据状态标志位发送相应的信息
输入参数：无  
返 回 值：无  
备    注：
****************************************************************************************************************/	
void SendUartState(void)
{
  char buf[40] = {0};
	unsigned char read_motor_num = 0;
	unsigned int i = 0;
	static int count = 0;
	if (send_mode)//如果有状态需要发送
	{
		if (send_mode & SEND_A) //发送A
		{
			 UART_PutChar(USART1,'A');
			send_mode &= ~SEND_A;//清状态
		}
		if (send_mode & SEND_CC) //发送CC
		{
			UART_PutStr(USART1,"#CC\r\n");
			send_mode &= ~SEND_CC;
		}
		if (send_mode & SEND_DOWN_OK)//发送下载ok的状态字符串
		{
			sprintf(buf,"#Down+OK+%d\r\n",(int)motor_data.filecount-1);
			UART_PutStr(USART1,buf);
			send_mode &= ~SEND_DOWN_OK;
#if DEBUG
			sprintf(buf,"%d\r\n",(uint)motor_data.filecount);
			UART_PutStr(USART1,buf);
#endif
		}
		if (send_mode & SEND_START_OK)//发送连接时候的字符串
		{
			UART_PutStr(USART1,"#Veri+UART+OK+20160906+176\r\n");
			send_mode &= ~SEND_START_OK;
		}
		if (send_mode & SEND_READ_FILE)//发送读取文件的时候字符串
		{
			if (motor_data.filecount > 0)//如果保存的有舵机命令
			{
				//#Name:1.txt--Size:48--Name:2.txt--Size:190--Name:desktop.ini--Size:531--
				UART_PutStr(USART1,"#");//发送
				for (i = 0; i < motor_data.filecount;i++)
				{
					if (motor_data.file_flag[i] == 1)
					{
						ReadOneCmdInfor(i);
						if (motor_one_cmd.end - motor_one_cmd.start <= 0)
						{
							motor_data.file_flag[i] = 0;
							WriteMoterInfor();
#if DEBUG
							sprintf(buf,"E=%d S=%d",motor_one_cmd.end, motor_one_cmd.start);
#endif
						}
						else
						{
							sprintf(buf,"Name:%d.txt--Size:%d--",i,motor_one_cmd.end - motor_one_cmd.start);//获取命令个数
							UART_PutStr(USART1,buf);//发送
						}
					}
				}
				UART_PutStr(USART1,"\r\n");				
			}
			else
			{
				sprintf(buf,"#\r\n",motor_data.sum);
				UART_PutStr(USART1,buf);
			}
			send_mode &= ~SEND_READ_FILE;
		}
		if (send_mode & SEND_SET_OFFLINE_OK)//设置脱机运行次数
		{
			WriteOneCmdInfor(motor_one_cmd.cur_file_num);//保存
			UART_PutStr(USART1,"#Enable+OK...\r\n");
			send_mode &= ~SEND_SET_OFFLINE_OK;
		}
		if (send_mode & SEND_SET_DISABLEOFFLINE_OK)//禁止脱机运行
		{
			for (i = 0; i < motor_data.filecount;i++)
			{
				if (motor_data.file_flag[i] == 1)
				{
					ReadOneCmdInfor(i);
					motor_one_cmd.tuoji_count = 0;
					WriteOneCmdInfor(i);
				}
			}
			WriteMoterInfor();
			UART_PutStr(USART1,"#Disable+OK...\r\n");
			send_mode &= ~SEND_SET_DISABLEOFFLINE_OK;
		}
		if (send_mode & SEND_SET_ONLINE_OK)//发送联机运行状态
		{
			UART_PutStr(USART1,"#OK\r\n");
			sprintf(buf,"#%dGC%d\r\n",cur_item.file_num,cur_item.tuoji_count);
			UART_PutStr(USART1,buf);
			UART_PutStr(USART1,"#LP=0\r\n");
			send_mode &= ~SEND_SET_ONLINE_OK;
			flag_connect_run = 1;
		}
		if (send_mode & SEND_SET_DELETE_ONE_FILE_OK)//发送删除文件命令
		{
			//cur_item.tuoji_count = 0;
			if (cur_item.delete_num < motor_data.filecount)
			{
				motor_data.file_flag[cur_item.delete_num] = 0;
				WriteMoterInfor();
				updata_file_list();
				if (cur_item.delete_num  == motor_data.filecount-1)
				{
					motor_data.filecount = file_last_num + 1; 
					if (file_last_num == -1)
					{
						motor_data.sum = 0;
					}
					else
					{
						ReadOneCmdInfor(file_last_num);
						motor_data.sum = motor_one_cmd.end;
						motor_data.sum = (((long int)(motor_data.sum) >>4)<<4) + (1<<4);
					}
					WriteMoterInfor();
				}
				updata_file_list();		
				UART_PutStr(USART1,"#FDel+OK\r\n");
			}
			send_mode &= ~SEND_SET_DELETE_ONE_FILE_OK;
			
		}
		if (send_mode & SEND_SET_DELETE_ALL_FILE_OK)//发送擦除所有文件的命令
		{
			UART_PutStr(USART1,"#Format+Start\r\n");
			Flag_Flash=1;
			SpiFlashEraseChip();
			cur_item.tuoji_count = 0;
			motor_data.sum = 0;
			motor_data.filecount = 0;
			memset(motor_data.file_flag,0,sizeof(motor_data.file_flag));
			WriteMoterInfor();
			UART_PutStr(USART1,"#Format+OK\r\n");
			Flag_Flash=0;
			send_mode &= ~SEND_SET_DELETE_ALL_FILE_OK;
			updata_file_list();
		}
		if (send_mode & SEND_SET_PS2_OK)
		{
			UART_PutStr(USART1,"#PS2+OK...\r\n");
			send_mode &= ~SEND_SET_PS2_OK;
		}
		
#define MATHION_HAND_NUM 20
		if (send_mode & SEND_SET_READ_UART_MOTOR_ANGLE)
		{
			if (cur_item.read_num < MATHION_HAND_NUM)
			{
				
				if (flag_uart2_rev)
				{
					//	UART_PutStr(USART1,"test\r\n");	
					read_motor_num = atoi(uart2_buf + 1);
					// UART_Put_Inf("mor_num:",read_motor_num);
					if (read_motor_num == cur_item.read_num)
					{
						//UART_Put_Inf("num:",read_motor_num);
						i = atoi(uart2_buf+5);
						sprintf(uart2_buf,"#%dP%d",(int)read_motor_num,i);
						UART_PutStr(USART1,uart2_buf);
						cur_item.read_num++;
						buf[0] = '#';
						buf[1] = cur_item.read_num / 100 % 10 + 0x30;
						buf[2] = cur_item.read_num / 10 % 10 + 0x30;
						buf[3] = cur_item.read_num % 10 + 0x30;
						buf[4] = 'P';buf[5] = 'R';buf[6] = 'A';buf[7] = 'D';buf[8] = '\r';buf[9] = '\n';
						UART_PutStr(USART3,buf);
					}
					flag_uart2_rev = 0;
					count = 0;
				}
				else
				{
					count++;
					if (count >= 300)
					{
						cur_item.read_num++;
						buf[0] = '#';
						buf[1] = cur_item.read_num / 100 % 10 + 0x30;
						buf[2] = cur_item.read_num / 10 % 10 + 0x30;
						buf[3] = cur_item.read_num % 10 + 0x30;
						buf[4] = 'P';buf[5] = 'R';buf[6] = 'A';buf[7] = 'D';buf[8] = '\r';buf[9] = '\n';
						UART_PutStr(USART3,buf);		
						flag_uart2_rev = 0;
						flag_uart2_rev_time_out = 0;
						count = 0;
					}
				}
			}
			else
			{
				send_mode &= ~SEND_SET_READ_UART_MOTOR_ANGLE;
				cur_item.read_num= 0;
				UART_PutStr(USART1,"\r\n");
			}
		}
	}
	if (send_mode & SEND_SET_SET_UART_MOTOR_PULK)
	{
		if (cur_item.pulk_num < MATHION_HAND_NUM)
		{
			count++;
			if (count >= 20)
			{
				sprintf(uart2_buf,"#%dPULK\r\n",(int)cur_item.pulk_num);
				UART_PutStr(USART3,uart2_buf);
#if DEBUG
				UART_PutStr(USART1,uart2_buf);
#endif
				cur_item.pulk_num++;
				count = 0;
			}
		}
		else
		{
			send_mode &= ~SEND_SET_SET_UART_MOTOR_PULK;
		}
	}
	if (send_mode & SEND_SET_SET_UART_MOTOR_ANGLE)
	{
		if (cur_item.pulk_num < MATHION_HAND_NUM)
		{
			count++;
			if (count >= 5)
			{
				sprintf(uart2_buf,"#%dPMOD%d\r\n",(int)cur_item.pulk_num,(int)cur_item.angle_mode);
				UART_PutStr(USART3,uart2_buf);
#if DEBUG
				UART_PutStr(USART1,uart2_buf);
#endif
				cur_item.pulk_num++;
				count = 0;
			}
		}
		else
		{
			sprintf(buf,"#255PMOD%d+0K...\r\n",(int)cur_item.angle_mode);
			UART_PutStr(USART1,buf);
			send_mode &= ~SEND_SET_SET_UART_MOTOR_ANGLE;
			flag_uart2_rev = 0;
		}
	}
	if (send_mode & SEND_SET_BEEP_ON)
	{
		ReadMoterInfor();
		motor_data.beep_mode = 1;
		//beep_mode = 1;
		WriteMoterInfor();
		UART_PutStr(USART1,"#FMQENABLE+OK...\r\n");
		send_mode &= ~SEND_SET_BEEP_ON;
	}
	if (send_mode & SEND_SET_BEEP_OFF)
	{
		ReadMoterInfor();
		motor_data.beep_mode = 0;
		//beep_mode = 0;
		WriteMoterInfor();
		UART_PutStr(USART1,"#FMQDISABLE+OK...\r\n");
		send_mode &= ~SEND_SET_BEEP_OFF;
	}		
}
/***************************************************************************************************************
函 数 名：作业初位置，末尾置更新函数  
功能描述：从缓存中取一个新的目标位置替换原来的目标位置，原来的目标位置变为新的初位置，一次更替
		：有效的数据是插补增量，和插补次数，知道这两个量，和当前初位置即可
备    注： 先进先出，循环访问	
****************************************************************************************************************/	
 void change(void)
{	
	unsigned char s;
	if(line>0)			   	//缓存中有数据
	{  
		line--;			   	//执行一行
		if(line<5)			//缓存允许放入新的数据			
			flag_in=1;
		point_now++;		//取数位置更新
		point_aim++;
		
		if(point_aim==7)
		   	point_aim=0;
		if(point_now==7)
		   	point_now=0;
		n=pos[point_aim][0]/20;	//计算新的插补次数	

		for(s=1;s<MOTOR_NUM;s++)	//计算新的插补增量
		{
		 if(pos[point_aim][s]>pos[point_now][s])
			{
	   			dp=pos[point_aim][s]-pos[point_now][s];
			   	dp0[s]=dp/n;
			}
		    if(pos[point_aim][s]<=pos[point_now][s])
			{
				dp=pos[point_now][s]-pos[point_aim][s];
				dp0[s]=dp/n;
				dp0[s]=-dp0[s];
			}
				//UART_Put_Inf("dp:",dp);
			// UART_Put_Inf("n",n);
	   }
		m=0;				  	//m清0
		flag_stop=0;		  	//产生了新的目标位置，停止标志清零
	}
	else	  					//没有缓存数据，即line==0
	{
		flag_out=0;				//缓存中没有数据
	}
}
/***************************************************************************************************************
函 数 名：vpwm()  
功能描述：数据插补，插补时间间隔为20/12ms，由timer0控制，使舵机平滑实现速度控制
		：另一个功能是执行完一行后去更新下一行数据，即调用change()
备    注：
****************************************************************************************************************/	
void vpwm(void)		 
{
	unsigned char j=0;
	unsigned char how=0;
  static unsigned char flag_how;
	static unsigned char flag_Tover;

	if(flag_stop==1)   					//一行作业全部完成
	{	
		if(flag_out==1)	 				//缓冲数组中有数据
		{
			change();					//更新行
		}
	}
	else								//不是一行数据全部完成，处于中间插补阶段
	{
		m++;							//用来累加插补过的次数
		if(m==n)						//n是本行作业要插补的总次数
		{
		  flag_Tover=1;				//一行数据的执行时间已经完成
			send_mode |= SEND_CC;
		}
		for(j=1;j<MOTOR_NUM;j++)
		{
			if(abs(CPWM[j]-pos[point_aim][j])<5)
			{						   	//检测靠近终点位置
			   how++;				   	//是，则累加一个
			   CPWM[j]=pos[point_aim][j];//并且直接过度到终点位置
			}	
			else						//不靠近终点，继续插补
			{
				CPWM[j]=pos[point_now][j]+m*dp0[j];
			}
		} 								
		if(how==MOTOR_NUM-1)
			flag_how=1;	  					//16个舵机都到达终点
		how=0; 
		if((flag_Tover==1)&&(flag_how==1))
		{								//从插补次数，和脉宽宽度两方面都到达终点，本作业行完成
			 flag_Tover=0;
			 flag_how=0;
			 flag_stop=1;
		}
	 }
	return;
}
/************************************************************************/
#if 0
void SerVor_Test(void)
{
		CPWM[0]=700;CPWM[1]=700;CPWM[2]=700;CPWM[3]=700;CPWM[4]=700;CPWM[5]=700;
		Delay_ms(2000);
		CPWM[0]=2300;CPWM[1]=2300;CPWM[2]=2300;CPWM[3]=2300;CPWM[4]=2300;CPWM[5]=2300;
		Delay_ms(2000);
}
#endif
#if 0
void Ps2_Test(void)
{
		key = PS2_DataKey();
		if(key!=0)
    	{
				printf("  \r\n   %d  is  pressed  \r\n",key);
    	}
     printf(" %5d %5d %5d %5d\r\n",PS2_AnologData(PSS_LX),PS2_AnologData(PSS_LY),
		                              PS2_AnologData(PSS_RX),PS2_AnologData(PSS_RY) );
    Delay_ms(1000);   
}
#endif
#if 1
void Flash_Test(void)
{
		while(SpiFlashReadID()!=W25Q64)							//检测不到W25Q64
	{
		UART_PutStr (USART1,"Not Found W25Q64\r\n");
	}
	UART_PutStr (USART1," Found W25Q64\r\n");
	SpiFlashWrite((u8*)TEXT_Buffer,FLASH_SIZE-100,SIZE);		//从倒数第100个地址写入数据
	UART_PutStr (USART1,"Flash Write Success\r\n");
	SpiFlashRead(datatemp,FLASH_SIZE-100,SIZE);	  //读取写入flash中的数据
	UART_PutStr (USART1,datatemp);
	UART_PutStr (USART1,"\r\n Flash Read Success\r\n");
	while(1);
}
#endif
