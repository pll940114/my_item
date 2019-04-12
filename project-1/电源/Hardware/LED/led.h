#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LEDPORT	GPIOB	//定义IO接口
#define JI	GPIO_Pin_0	//定义IO接口
#define AC	GPIO_Pin_1	//定义IO接口

void LED_Init(void);//初始化
		 				    
#endif