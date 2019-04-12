#ifndef __TM1640_H
#define __TM1640_H	 
#include "sys.h"

#define TM1640_GPIOPORT	GPIOA	//定义IO接口
#define TM1640_DIN	GPIO_Pin_12	//定义IO接口
#define TM1640_SCLK	GPIO_Pin_11	//定义IO接口

#define TM1640_LEDPORT	0xC8	//定义IO接口


void TM1640_Init(void);//初始化
void TM1640_led(u8 date);//
void TM1640_display(u8 address,u8 date);//
void TM1640_display_add(u8 address,u8 date);//

		 				    
#endif
