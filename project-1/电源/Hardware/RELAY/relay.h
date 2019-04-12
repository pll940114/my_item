#ifndef __RELAY_H
#define __RELAY_H	 
#include "sys.h"


#define RELAYPORT	GPIOA	//定义IO接口
#define RELAY1	GPIO_Pin_14	//定义IO接口
#define RELAY2	GPIO_Pin_13	//定义IO接口



void RELAY_Init(void);//继电器初始化
void RELAY_1(u8 c);//继电器控制1
void RELAY_2(u8 c);//继电器控制2
		 				    
#endif
