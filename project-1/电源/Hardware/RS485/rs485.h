#ifndef __RS485_H
#define __RS485_H	 
#include "sys.h"

#define RS485PORT	GPIOA	//定义IO接口
#define RS485_RE	GPIO_Pin_8	//定义IO接口



void RS485_Init(void);//初始化
void RS485_printf (char *fmt, ...); //RS485发送
		 				    
#endif
