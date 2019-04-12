#ifndef __USART_H
#define __USART_H

#include "stdio.h"	
#include "sys.h" 
#include "common.h"

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	


void USART_Config(USART_TypeDef* TUSARTx,u32 bound); 
void USART3_Config(u32 bound);
void Uart_Init(u16 uart_num);
void UART_PutChar(USART_TypeDef* USARTx, uint8_t Data);
void UART_PutStr (USART_TypeDef* USARTx, char *str);
void UART_Put_Num(uint16 dat);
void UART_Put_Inf(char *inf,uint16 dat);
void DealRec(void);
void uart_init(u32 bound);
#endif



