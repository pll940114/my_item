#include "servor.h"
#include "usart.h"
#include "delay.h"
#include "common.h"

#define MAXPWM 2505		 //舵机最大PWM控制脉宽2.5ms宏定义

extern u8 date[3];

u8 count1,count2,count3;				 //每个变量用作8路舵机先后赋值控制
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	 //????? TIM??? ????????????
TIM_TimeBaseInitTypeDef TIM_Time1BaseStructure;

extern uint16 CPWM[MOTOR_NUM];
extern uint8 flag_vpwm;				//表示到达了该更新pwm[]的时间

void Servor_GPIO_Config(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;	
 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable , ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}
/************************GPIO电平反转函数1*****************************************/ 

void Flip_GPIO_One(void)
{
	switch(count1)								 //将20ms的舵机控制周期分成8份，每2.5ms控制一个舵机运转
	{  											      //每个定时器控制8路舵机运转，3个定时器控制24路舵机运转
		case 1: 
			 TIM2->ARR=CPWM[1];				 //将第一个舵机脉冲宽度值赋值给定时器2			
    	 GPIO_SetBits(GPIOB,GPIO_Pin_15);  //同时拉高控制舵机1的引脚的电
  	  // flag_vpwm=1;	
				break;
		
		case 2:
   	    TIM2->ARR=MAXPWM-CPWM[1]; 		 //将2.5ms减去PWM脉宽值的数据赋值定时器2
				GPIO_ResetBits(GPIOB,GPIO_Pin_15);//同时拉低控制舵机1引脚的电平 
       // flag_vpwm=1;	
				break;							 //控制舵机1的引脚在剩下20ms-CPM[0]时间内将一直保持低电平，舵机1按照CPWM值转动

		case 3:	
			TIM2->ARR=CPWM[2]; 
				GPIO_SetBits(GPIOA,GPIO_Pin_8);
        //flag_vpwm=1;			
				break;
		
		case 4:	
			TIM2->ARR=MAXPWM-CPWM[2];  
				GPIO_ResetBits(GPIOA,GPIO_Pin_8); 
		   // flag_vpwm=1;	
				break;

		case 5:	
			TIM2->ARR=CPWM[3];  
				GPIO_SetBits(GPIOB,GPIO_Pin_5); 
				//flag_vpwm=1;	
				break;
		
		case 6:	
			TIM2->ARR=MAXPWM-CPWM[3];  
				GPIO_ResetBits(GPIOB,GPIO_Pin_5);
				//flag_vpwm=1;	
				break;

		case 7:	
			 TIM2->ARR=CPWM[4];  
			  GPIO_SetBits(GPIOB,GPIO_Pin_4); 
				//flag_vpwm=1;	
				break;
		
		case 8:	
			 TIM2->ARR=MAXPWM-CPWM[4];  
				GPIO_ResetBits(GPIOB,GPIO_Pin_4);
				//flag_vpwm=1;	
				break;

		case 9:	
			TIM2->ARR=CPWM[5];  
				GPIO_SetBits(GPIOB,GPIO_Pin_3); 
				//flag_vpwm=1;	
				break;
		
		case 10:
			TIM2->ARR=MAXPWM-CPWM[5];  
				GPIO_ResetBits(GPIOB,GPIO_Pin_3);
				//flag_vpwm=1;	
				break;

		case 11:
			TIM2->ARR=CPWM[6];  
				GPIO_SetBits(GPIOA,GPIO_Pin_15); 
				//flag_vpwm=1;	
				break;
		
		case 12:
			TIM2->ARR=MAXPWM-CPWM[6];  
				GPIO_ResetBits(GPIOA,GPIO_Pin_15);
			//	flag_vpwm=1;	
				break;

		case 13:
			TIM2->ARR=CPWM[7];  

			//	flag_vpwm=1;	
				break;
		
		case 14:
			  TIM2->ARR=MAXPWM-CPWM[7];  

			//	flag_vpwm=1;	
				break;

		case 15:
			TIM2->ARR=CPWM[8];  

			//	flag_vpwm=1;	
				break;
		
		case 16:
			TIM2->ARR=MAXPWM-CPWM[8]; 

				//flag_vpwm=1;	
				count1=0; 
				break;
		default:break;
	}	
	//count1++;
}


#if 0
/**********************************************************************************/
/************************GPIO电平反转函数2*****************************************/ 
void Flip_GPIO_Two(void)
{
	switch(count2)
	{  		
		case 1: TIM3->ARR=CPWM[8];				 //功能同上
				GPIO_SetBits(GPIOA,GPIO_Pin_8);  
				break;
		
		case 2:	TIM3->ARR=MAXPWM-CPWM[8]; 
				GPIO_ResetBits(GPIOA,GPIO_Pin_8); 
				break;

		case 3:	TIM3->ARR=CPWM[9]; 
				GPIO_SetBits(GPIOA,GPIO_Pin_9); 
				break;
		
		case 4:	TIM3->ARR=MAXPWM-CPWM[9];  
				GPIO_ResetBits(GPIOA,GPIO_Pin_9); 
				break;

		case 5:	TIM3->ARR=CPWM[10];  
				GPIO_SetBits(GPIOB,GPIO_Pin_9); 
				break;
		
		case 6:	TIM3->ARR=MAXPWM-CPWM[10];  
				GPIO_ResetBits(GPIOB,GPIO_Pin_9);
				break;

		case 7:	TIM3->ARR=CPWM[11];  
				GPIO_SetBits(GPIOB,GPIO_Pin_8); 
				break;
		
		case 8:	TIM3->ARR=MAXPWM-CPWM[11];  
				GPIO_ResetBits(GPIOB,GPIO_Pin_8);
				break;

		case 9:	TIM3->ARR=CPWM[12];  
				GPIO_SetBits(GPIOB,GPIO_Pin_7); 
				break;
		
		case 10:TIM3->ARR=MAXPWM-CPWM[12];  
				GPIO_ResetBits(GPIOB,GPIO_Pin_7);
				break;

		case 11:TIM3->ARR=CPWM[13];  
				GPIO_SetBits(GPIOB,GPIO_Pin_6); 
				break;
		
		case 12:TIM3->ARR=MAXPWM-CPWM[13];  
				GPIO_ResetBits(GPIOB,GPIO_Pin_6);
				break;

		case 13:TIM3->ARR=CPWM[14];  
				GPIO_SetBits(GPIOB,GPIO_Pin_5); 
				break;
		
		case 14:TIM3->ARR=MAXPWM-CPWM[14];  
				GPIO_ResetBits(GPIOB,GPIO_Pin_5);
				break;

		case 15:TIM3->ARR=CPWM[15];  
				GPIO_SetBits(GPIOB,GPIO_Pin_4); 
				break;
		
		case 16:TIM3->ARR=MAXPWM-CPWM[15]; 
				GPIO_ResetBits(GPIOB,GPIO_Pin_4); 
				count2=0; 
				break;
	}	
}
#endif

/**********************************************************************************/   
/************************舵机控制函数1*********************************************/
void Servo1(void)
{		
	count1++; 
	Flip_GPIO_One();						 //反转IO电平

}
/**********************************************************************************/

#if 0
/************************舵机控制函数2*********************************************/
void Servo2(void)
{		
	count2++;	
	Flip_GPIO_Two();
}
/**********************************************************************************/
#endif

