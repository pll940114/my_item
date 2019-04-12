#include "beep.h"
#include "delay.h"


void Beep_Init(void)
{
 
	GPIO_InitTypeDef GPIO_InitStructure;    
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;//PB13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置为推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB13	
	GPIO_SetBits(GPIOB,GPIO_Pin_13);				 //PA.8 输出高

}
 
void Beep_Test(void)
{
		BEEP(ON);
	  Delay_ms(500);
		BEEP(OFF);
	  Delay_ms(500);

}