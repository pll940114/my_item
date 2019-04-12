#ifndef __ENCODER_H
#define __ENCODER_H	 
#include "sys.h"
#include "delay.h"

#define ENCODER_PORT_A	GPIOA		//定义IO接口组
#define ENCODER_L	GPIO_Pin_6	//定义IO接口
#define ENCODER_D	GPIO_Pin_7	//定义IO接口

#define ENCODER_PORT_B	GPIOB		//定义IO接口组
#define ENCODER_R	GPIO_Pin_2	//定义IO接口


void ENCODER_Init(void);//初始化
u8 ENCODER_READ(void);


		 				    
#endif
