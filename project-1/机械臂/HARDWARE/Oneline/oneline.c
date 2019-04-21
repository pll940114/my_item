#include "oneline.h"
#include "usart.h"
#include <string.h>
#include "flash.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
extern char All_down_num;
extern uint16 UartRec[MOTOR_NUM]; 			  //上位机字符串解析都放在这个数组里
extern uint16 pos[7][MOTOR_NUM];
extern uint16 CPWM[MOTOR_NUM];
extern char redata[257];
extern uint8 ps2_buf[120];
	uint8 flag_mp3=0;
extern char point_in;
extern uint8 flag_stop;
extern uint8 line;
extern uint8 flag_in;
extern uint8 flag_out;
extern uint8 flag_download;
extern uint8 flag_All_download;
extern uint8 flag_All_Stop_Down;//标志位1
extern uint8 flag_read;
extern uint8 flag_RecFul;
extern uint8 flag_stop_download;
extern uint8 flag_online_run;
uint8 pwm_num;	
extern uint16 tuoji_count;
extern unsigned long send_mode;
extern MotorData motor_data;
extern MotorOneCmd motor_one_cmd;
extern CurrentItem cur_item;
extern uint8 error;
/***************************************************************************************************************
函 数 名：RecStr_to_pos()  
功能描述：把解析之后的上位机运行数据 送入缓存 
输入参数：无，由全局数组变量 UartRec[33]传递 
备    注：这个缓存更新函数不用放在主函数循环里，由中断直接调用; 
****************************************************************************************************************/	
void RecStr_to_pos(void)	 
{
	uint8 i = 0;
	if (!flag_download)
	{
		if(line<7) 					//缓存还有空闲，没有满
		{   
			line++;					//缓存中又增加了一行数据
			point_in++;
			if(point_in==7)
				point_in=0;
			for(i=0;i<MOTOR_NUM;i++)
			{
				pos[point_in][i]= UartRec[i];
				
			}
		}
		else
		{
			flag_in=0;				//表示没有空间了
		}

		if(line>0)
		{
		   flag_out=1;					//表示缓存中有数据
		   //flag_stop = 1;
		}
	}
	else 
	{
		if (motor_one_cmd.end < CMD_FLASH_ADDR)
		{
			if (((motor_one_cmd.end)% 16) == 0)
			{
				SpiFlashEraseSector((motor_one_cmd.end) >> 4);  //页数除以16得到扇区地址  每个扇区 16页 4K
			}
			SpiFlashWrite((unsigned char*)redata,(((unsigned long int)motor_one_cmd.end)<<WRITE_BIT_DEPTH),WRITE_SIZE); //每次写入256个字节
			motor_one_cmd.end += 1;		
			send_mode |= SEND_A;
			error &= ~ERROR_FLASH_FULL;
		}
		else
		{
			error |= ERROR_FLASH_FULL;
		}
	}
}

/***************************************************************************************************************
函 数 名：RecStr_to_pwm()  
功能描述：把解析之后的上位机指令-滚动条实时角度变化值 送入缓存 
输入参数：无，由全局数组变量 UartRec[17]传递 
备    注：因为要求实时性好，不要求插补，直接放入pwm[] 
****************************************************************************************************************/	
void RrcStr_to_pwm(void)
{
	int i=0;
	for(i=1;i<7;i++)
	 {CPWM[i]= UartRec[i];}
}
/***************************************************************************************************************
函 数 名：Parse_String_Cmd()  
功能描述：解析串口接收到的命令字符串
输入参数：传递接受到串口数据 
备    注：
****************************************************************************************************************/

int Parse_String_Cmd( char *str)
{
	char *p = NULL;

	p = strchr(str,'#');

	
	if (p == NULL )
		return 1;
	p++;
	if ((*(p) >= '0' && *(p) <= '9' && *(p+1) == 'P')|| ((*(p)>='0' && *(p) <= '9')&& (*(p+1) == '0'&&*(p+1) <= '9') &&*(p+2) == 'P'))//如果当前字符串以#数字P开始的，说明是舵机转动命令直接返回
		return 0;
	if(*p=='M')
	{
		//UART_PutStr(USART3,str);// 串口2发送出去这个命令
		return 0;
	}
	if (strstr(str,"#255PRAD"))
	{
		send_mode |= SEND_SET_READ_UART_MOTOR_ANGLE;
		memset(ps2_buf,0,sizeof(ps2_buf));
		cur_item.read_num = 0;
		UART_PutStr(USART1,"#PWMR");
		return 1;
	}
	if (strstr(str,"#255PULK"))
	{
		//UART_PutStr(USART2,"#255PULK\r\n");
		send_mode |= SEND_SET_SET_UART_MOTOR_PULK;
		cur_item.pulk_num = 0;
		return 1;
	}
	if ((p = strstr(str,"#255PMOD"))!=NULL)
	{
		//UART_PutStr(USART2,"#255PULK\r\n");
		send_mode |= SEND_SET_SET_UART_MOTOR_ANGLE;
		cur_item.pulk_num = 0;
		cur_item.angle_mode = atoi(p + strlen("#255PMOD"));
		return 1;
	}
	if (strstr(str,"#Stop"))//收到下载时候上位机发送Stop说明下载成功了
	{
		if (flag_download)
		{
			flag_stop_download = 1;
		}
		else
		{
			flag_stop = 1;
			flag_out = 0;
		}
		return 1;
	}

	if (strstr(str,"#Down"))//下载命令
	{
		ReadMoterInfor();
		memset(&motor_one_cmd,0,sizeof(motor_one_cmd));
		motor_one_cmd.start = motor_data.sum ;
		motor_one_cmd.end = motor_data.sum ;
		flag_download = 1;//标志位1
		send_mode |= SEND_A;
		return 1;
	}

	
	if (strstr(str,"#STOP"))//接受到停止运行
	{
		tuoji_count = 0;
		flag_download = 0;
		flag_stop = 1;
		flag_out = 0;
		return 1;
	}
	if (strstr(str,"#Veri+"))//接收到连机命令
	{
		flag_stop_download = 0;
		flag_download = 0;
		flag_read = 0;
		send_mode |= SEND_START_OK;
		return 1;
	}

	if (strstr(str,"#Flist"))//接收到刷新命令
	{
		flag_download = 0;
		flag_All_download=0;
		flag_stop = 1;
		flag_out = 0;
		ReadMoterInfor();
		send_mode |= SEND_READ_FILE;
		return 1;
	}
	if ((p = strstr(str,"#FRead-"))!=NULL)//接收到读取命令
	{
		flag_download = 0;
		flag_read = 1;
		flag_stop = 1;
		flag_out = 0;
		ReadMoterInfor();
		cur_item.read_num = atoi(p+strlen("#FRead-"));
		ReadOneCmdInfor(cur_item.read_num);
		send_mode |= SEND_SEND_FILE;
		return 1;
	}
	if ( strstr(str,"#PS2"))//接收到了PS2手柄设置信息
	{
		SpiFlashEraseSector((PS2_FLASH_ADDR) >> 4);//擦除以前存储的信息
		SpiFlashWrite((unsigned char*)redata,(PS2_FLASH_ADDR)<<WRITE_BIT_DEPTH,WRITE_SIZE); 
		send_mode |= SEND_SET_PS2_OK;
		return 1;
	}	
	if (((p = strstr(str,"#Enable#")))!=NULL)//接收到脱机运行几次命令
	{
		flag_download = 0;
		cur_item.file_num = atoi(p+strlen("#Enable#"));//算出当前文件
		ReadOneCmdInfor(cur_item.file_num);
		p = strstr(str,"GC");
		motor_one_cmd.tuoji_count = atoi(p+strlen("GC"));//算出脱机执行次数
		send_mode |= SEND_SET_OFFLINE_OK;//
		flag_stop = 1;
		flag_out = 0;
		return 1;
	}
	if (strstr(str,"#Disable"))//接收到禁止脱机运行的数据
	{
		flag_download = 0;
		ReadMoterInfor();
		motor_data.stop_tuoji_flag = 1;
		send_mode |= SEND_SET_DISABLEOFFLINE_OK;
		flag_stop = 1;
		flag_out = 0;
		return 1;
	}
	if ((p = strstr(str,"GC"))!=NULL)//接收到了联机运行几次的命令
	{
		flag_download = 0;
		ReadMoterInfor();
		p = strstr(str,"#");
		cur_item.file_num = atoi(p+strlen("#"));
		ReadOneCmdInfor(cur_item.file_num);
		p = strstr(str,"GC");
		cur_item.tuoji_count = atoi(p+strlen("GC"));	
		send_mode |= SEND_SET_ONLINE_OK;
		flag_online_run = 1;
		flag_stop = 1;
		flag_out = 0;
		return 1;
	}
	
	if ((p = strstr(str,"#FDel-"))!=NULL)//接收到删除命令
	{
		flag_download = 0;
		ReadMoterInfor();
		cur_item.delete_num = atoi(p+strlen("#FDel-"));
		send_mode |= SEND_SET_DELETE_ONE_FILE_OK;
		flag_stop = 1;
		flag_out = 0;
		return 1;
	}
	if ( strstr(str,"#Format"))//接收到了格式化的命令
	{
		send_mode |= SEND_SET_DELETE_ALL_FILE_OK;
		flag_stop = 1;
		flag_out = 0;
		return 1;
	}
	if ( strstr(str,"#FMQENABLE"))//接收到了格式化的命令
	{
		send_mode |= SEND_SET_BEEP_ON;
		return 1;
	}
	if ( strstr(str,"#FMQDISABLE"))//接收到了格式化的命令
	{
		send_mode |= SEND_SET_BEEP_OFF;
		return 1;
	}
	return 0;
}
/***************************************************************************************************************
函 数 名：OneLine()  
功能描述：把串口接收到的上位机编辑运行数据字符串解析出来，放到UartRec[33] 
输入参数：*str.串口接受字符的存放地址 
返 回 值：无  
备    注：原来单个舵机是用OneMotor()处理的。现在合并为一个函数处理，当T<100时直接放入pwm[],不进行插补
****************************************************************************************************************/	
void OneLine(char *str)
{
	uint8 motor_num=0;		   //舵机号
	uint16 motor_jidu=0;	   //舵机脉宽值
	uint16 motor_time=0;	   //执行时间
	uint8 num_now=0;		   //编号解析中间变量
	uint8 jidu_now=0;		   //脉宽解析中间变量
	uint8 time_now=0;		   //执行时间解析中间变量
	uint8 flag_num=0;		   //标记出现过#
	uint8 flag_jidu=0;		   //标记出现过P
	uint8 flag_time=0;		   //标记出现过T

	uint16 i=0;
//	uint8 model_flag = 0;				   //用来移动字符串
	//uint8 buf[257] = {0};
  if (Parse_String_Cmd(str))
		return ;
	UART_PutStr(USART3,str);// 串口2发送出去这个命令
	if(strncmp(str,"#MP3",4)==0)
	{
	
		//UART_PutStr(USART1,"test\r\n");
		if(flag_download)
			{
					RecStr_to_pos();
			}
		else
		{
				send_mode |= SEND_CC;
		}
			
  }
	else 
	{
		while( str[i]!='\n'  && i < 255)
		{
			if(flag_num==1)	 				//出现过#
			{
				if(str[i]!='P' && str[i]!='M')				//如果当前字符不是P
				{
					num_now=ASC_To_Valu(str[i]);//把当前数字字符转化成数字的值
					motor_num=motor_num*10+num_now;
				}
				else  						//当前字符是P
					flag_num=0;
			}
			

			if(flag_jidu==1)				//出现过P
			{
				if((str[i]!='T')&(str[i]!='#')& (str[i-1]!='M'))	
				{							//当前字符是出现p之后的非#非T的字符
					jidu_now=ASC_To_Valu(str[i]);//把当前数字字符转化成数字的值
					motor_jidu=motor_jidu*10+jidu_now;
				}
				else  						//当前字符是#或者T，角度数据结束
				{
					flag_jidu=0;
					if(motor_jidu>2500)
						motor_jidu=2500;
					UartRec[motor_num]=motor_jidu;
					pwm_num=motor_num;
					motor_jidu=0;
					motor_num=0;
				}
			}

			if(flag_time==1)				//出现了T
			{
				time_now=ASC_To_Valu(str[i]);//把当前数字字符转化成数字的值
				motor_time=motor_time*10+time_now;
				UartRec[0]=motor_time;	   	//执行时间放在【0】位置
				if(str[i+1]=='\r')
				{	
					if(motor_time<=100)		 //如果给定的时间小于100us，则舵机不进行速度控制，实时的改变舵机角度
						RrcStr_to_pwm();
					else					 //如果时间大于100us进行速度控制
					{
						RecStr_to_pos(); 	//下一个字符是！，表示本行指令结束 ，将速度控制的时间值存放在数组的0位
					}
				}
			}
		
			if(str[i]=='#')
				flag_num=1;
			if(str[i]=='P')
				flag_jidu=1;
			if(str[i]=='T')
				flag_time=1;
			//if(str[i]=='M')
			//	flag_mp3=1;
			i++;
		}	 
 }
}
