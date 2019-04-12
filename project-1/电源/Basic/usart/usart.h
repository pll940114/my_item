#ifndef __USART_H
#define __USART_H
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "stdio.h"	
#include "sys.h" 


#define USART_n		USART1  //定义使用printf函数的串口，其他串口要使用USART_printf专用函数发送

#define USART1_REC_LEN  			200  	//定义USART1最大接收字节数
#define USART2_REC_LEN  			200  	//定义USART2最大接收字节数
#define USART3_REC_LEN  			200  	//定义USART3最大接收字节数

//不使用某个串口时要禁止此串口，以减少编译量
#define EN_USART1 			1		//使能（1）/禁止（0）串口1
#define EN_USART2 			1		//使能（1）/禁止（0）串口2
#define EN_USART3 			1		//使能（1）/禁止（0）串口3
	  	
extern u8  USART1_RX_BUF[USART1_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u8  USART3_RX_BUF[USART3_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
 
extern u16 USART1_RX_STA;         		//接收状态标记	
extern u16 USART2_RX_STA;         		//接收状态标记	
extern u16 USART3_RX_STA;         		//接收状态标记	

//函数声明
void USART1_Init(u32 bound);//串口1初始化并启动
void USART2_Init(u32 bound);//串口2初始化并启动
void USART3_Init(u32 bound);//串口3初始化并启动
void USART1_printf(char* fmt,...); //串口1的专用printf函数
void USART2_printf(char* fmt,...); //串口2的专用printf函数
void USART3_printf(char* fmt,...); //串口3的专用printf函数

#endif


