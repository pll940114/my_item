#include "main.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "HX711.h"
#include <string.h>

__IO uint32_t LocalTime = 0; /* 此变量用于创建时间引用，增量为10毫秒 */
uint32_t timingdelay;
u8 a;
extern char c[100][8];
extern MB member[100];

/***********LWIP所需调用函数***************/
void Delay(uint32_t nCount)
{
  timingdelay = LocalTime + nCount;  
  while(timingdelay > LocalTime)
  {     
  }
}
/*更新系统本地时钟*/
void Time_Update(void)
{
  LocalTime += SYSTEMTICK_PERIOD_MS;
}
/*处理系统的定期任务*/
void System_Periodic_Handle(void)
{
  Display_Periodic_Handle(LocalTime);
  LwIP_Periodic_Handle(LocalTime);
}
/*****************************************/

int main(void)
{
		int flag=0;  //接收标志位
		int nm,p=0;  //判断有无标志位
		unsigned char flag_uart2_rev=0;
		char str[100]={1};  //缓存
		unsigned char tcp_data[100];
		struct tcp_pcb *pcb;
    /* 初始化系统 */
  	System_Setup();       
    /* 初始化LwIP  satck ip地址设置，mac设置，*/
    LwIP_Init();
	  /* tcp客户端初始化 */
	  TCP_Client_Init(TCP_LOCAL_PORT,TCP_SERVER_PORT,TCP_SERVER_IP); 
		USART2_Init(115200);
	  USART4_Init(9600);     //SIM8000规定的通信波特率1
	  USART5_Init(115200);
		LED_Init1();
		LED_Init2();
		LED_Init3();
		/*短信模块配置*/
		NVIC_Configuration();
		
	while(1)
	{  
		static unsigned int i=0;
		u8 Res;
		USART2_printf("123\n");
		
/******************************************************************************************/
/*************************接收OpenMV传来的数据并发送给数据库*******************************/
/******************************************************************************************/	
	if(USART_GetFlagStatus(UART5,USART_FLAG_RXNE) != RESET)
	{
			a =USART_ReceiveData(UART5);
		  if(a=='A'){
				tcp_data[0] = 'A';
				tcp_data[1] = '\0';
				printf("#1GC1\r\n");   
				GPIO_SetBits(GPIOC,GPIO_Pin_13);
			/******************************/
				pcb = Check_TCP_Connect();//检查连接
				if(pcb != 0)
				{	
					TCP_Client_Send_Data(pcb,tcp_data,sizeof(tcp_data));	//向服务器发送数据
				}
				Delay_s(0xfffff);											//延时
			/******************************/
		}
			 if(a=='B'){
				tcp_data[0] = 'B';
				tcp_data[1] = '\0';
				printf("#2GC1\r\n");   
			  GPIO_SetBits(GPIOC,GPIO_Pin_2);
				 /******************************/
				pcb = Check_TCP_Connect();//检查连接
				if(pcb != 0)
				{	
					TCP_Client_Send_Data(pcb,tcp_data,sizeof(tcp_data));	//向服务器发送数据
				}
				Delay_s(0xfffff);											//延时
			/******************************/
		}
			  if(a=='C'){
			  tcp_data[0] = 'C';
				tcp_data[1] = '\0';
				printf("#3GC1\r\n");   
			  GPIO_SetBits(GPIOC,GPIO_Pin_3);
				/******************************/
				pcb = Check_TCP_Connect();//检查连接
				if(pcb != 0)
				{	
					TCP_Client_Send_Data(pcb,tcp_data,sizeof(tcp_data));	//向服务器发送数据
				}
				Delay_s(0xfffff);											//延时
			/******************************/ 
		}
	}
/******************************************************************************************/
/******************************************************************************************/
		
/*********************************************************************************/
/*********************接收A53传来的数据并与缓存的数据进行匹配*********************/
/*********************************************************************************/	
	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET)  
	{ 
		Res =USART_ReceiveData(USART2);
		if(Res=='E')  //判断结束标志位
		{
			flag_uart2_rev=1;
			flag=0;
			str[i]='\0';
			i=0;
		}
		if(flag==1)
		{
				str[i]=Res;
				i++;
		}
		if(Res=='S')  //判断开始标志位
			flag=1;
	}	
	if(flag_uart2_rev==1)
	{ 
//		printf("%s %d\n",str,strlen(str));
		flag_uart2_rev=0;
		for(nm=0;nm<3;nm++)
		{
			if(!strncmp(str,member[nm].number,6))
			{
				if(member[nm].name=='A')
				{
					printf("#4GC1\r\n");
					GPIO_ResetBits(GPIOC,GPIO_Pin_13); 
				} 
				if(member[nm].name=='B')
				{
					printf("#5GC1\r\n");
					GPIO_ResetBits(GPIOC,GPIO_Pin_2); 
				}
				if(member[nm].name=='C')
				{
					printf("#6GC1\r\n");
					GPIO_ResetBits(GPIOC,GPIO_Pin_3); 
				}
//				printf("sucess\n");  //功能测试
				p=1;
//				printf("%s\n",str);
//				printf("%s",&c[nm][0]);
				break;
			}else{
				p=0;
			}
		}
//		printf("%d\n",p);
		delay_ms(1000);
		if(p==1)printf("1Z");  //用户验证码和数据库进行匹配，有则为1，反之为0
		if(p==0)printf("00");
	}
/*********************************************************************************/
/*********************************************************************************/	

		System_Periodic_Handle();									//作为服务器的例行事件服务，主要是更新TCP timers
	}
}

