#include <stdlib.h>
#include <string.h>
#include "Sensor.h"
#include "beep.h"
#include "delay.h"
#include "usart.h"
extern char redata[257];
extern unsigned char flag_RecFul;
extern unsigned char flag_connect;
extern char csb;
extern char yysb_flag;
extern char yysb_rmul;
extern u8 yysb_init[];	//初始化
extern u8 yysb_mul[];		//启动连续识别
extern u8 yysb_exit[];
void Sensor_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;   //PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_ResetBits(GPIOB,GPIO_Pin_11);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;   //PA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
}

void sensor_action(void)
{
	if(Sensor_hw==0)   //执行动作组
	{
		//csb=!csb;       //开启或关闭超声波检测
		yysb_flag=!yysb_flag;  //开启或者关闭语音识别
	if(yysb_flag) {
			yysb_rmul = 1;
			yysb_flag = 1;
		  Uart2_send_nbyte(yysb_init,3); Delay_ms(300);
		  Uart2_send_nbyte(yysb_mul,3); Delay_ms(300);
			strcpy(redata,"#1GC1\r\n");
			flag_connect = 1;
			flag_RecFul = 1;
		} else {
		  Uart2_send_nbyte(yysb_init,3); Delay_ms(300);
		  Uart2_send_nbyte(yysb_exit,3); Delay_ms(300);
			strcpy(redata,"#2GC1\r\n");
			flag_connect = 1;
			flag_RecFul = 1;
		}
		while(Sensor_hw==0) {}
		Beep_Test();
	}
	if(Sensor_gm==1)
	{
	  strcpy(redata,"#1GC1\r\n");
		flag_connect = 1;
		flag_RecFul = 1;
		while(Sensor_gm==1) {}
		Beep_Test();
	//	UART_PutStr(USART1,redata);

	}
}

