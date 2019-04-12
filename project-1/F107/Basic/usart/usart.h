#ifndef __USART_H
#define __USART_H
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "stdio.h"	
#include "sys.h" 
#include "stm32f10x.h"	 

#define USART_n		UART5  //¶¨ÒåÊ¹ÓÃprintfº¯ÊıµÄ´®¿Ú£¬ÆäËû´®¿ÚÒªÊ¹ÓÃUSART_printf×¨ÓÃº¯Êı·¢ËÍ

#define USART1_REC_LEN  			200  	//¶¨ÒåUSART1×î´ó½ÓÊÕ×Ö½ÚÊı
#define USART2_REC_LEN  			200  	//¶¨ÒåUSART2×î´ó½ÓÊÕ×Ö½ÚÊı
#define USART3_REC_LEN  			200  	//¶¨ÒåUSART3×î´ó½ÓÊÕ×Ö½ÚÊı
#define USART4_REC_LEN  			200  	//¶¨ÒåUSART4×î´ó½ÓÊÕ×Ö½ÚÊı
#define USART5_REC_LEN  			200  	//¶¨ÒåUSART5×î´ó½ÓÊÕ×Ö½ÚÊı

//²»Ê¹ÓÃÄ³¸ö´®¿ÚÊ±Òª½ûÖ¹´Ë´®¿Ú£¬ÒÔ¼õÉÙ±àÒëÁ¿
#define EN_USART1 			1		//Ê¹ÄÜ£¨1£©/½ûÖ¹£¨0£©´®¿Ú1
#define EN_USART2 			1		//Ê¹ÄÜ£¨1£©/½ûÖ¹£¨0£©´®¿Ú2
#define EN_USART3 			1		//Ê¹ÄÜ£¨1£©/½ûÖ¹£¨0£©´®¿Ú3
#define EN_UART4 			  1		//Ê¹ÄÜ£¨1£©/½ûÖ¹£¨0£©´®¿
#define EN_UART5 			  1		//Ê¹ÄÜ£¨1£©/½ûÖ¹£¨0£©´®¿Ú4
	  	
extern u8  USART1_RX_BUF[USART1_REC_LEN]; //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.Ä©×Ö½ÚÎª»»ĞĞ·û 
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.Ä©×Ö½ÚÎª»»ĞĞ·û
extern u8  USART3_RX_BUF[USART3_REC_LEN]; //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.Ä©×Ö½ÚÎª»»ĞĞ·û
extern u8  USART4_RX_BUF[USART3_REC_LEN]; //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.Ä©×Ö½ÚÎª»»ĞĞ·û
extern u8  USART5_RX_BUF[USART3_REC_LEN]; //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.Ä©×Ö½ÚÎª»»ĞĞ·û
 
extern u16 USART1_RX_STA;         		//½ÓÊÕ×´Ì¬±ê¼Ç	
extern u16 USART2_RX_STA;         		//½ÓÊÕ×´Ì¬±ê¼Ç	
extern u16 USART3_RX_STA;         		//½ÓÊÕ×´Ì¬±ê¼Ç
extern u16 USART4_RX_STA;         		//½ÓÊÕ×´Ì¬±ê¼Ç
extern u16 USART5_RX_STA;         		//½ÓÊÕ×´Ì¬±ê¼Ç

//º¯ÊıÉùÃ÷
void USART1_Init(u32 bound);//´®¿Ú1³õÊ¼»¯²¢Æô¶¯
void USART2_Init(u32 bound);//´®¿Ú2³õÊ¼»¯²¢Æô¶¯
void USART3_Init(u32 bound);//´®¿Ú3³õÊ¼»¯²¢Æô¶¯
void USART4_Init(u32 bound);//´®¿Ú4³õÊ¼»¯²¢Æô¶¯
void USART5_Init(u32 bound);//´®¿Ú5³õÊ¼»¯²¢Æô¶¯
void USART1_printf(char* fmt,...); //´®¿Ú1µÄ×¨ÓÃprintfº¯Êı
void USART2_printf(char* fmt,...); //´®¿Ú2µÄ×¨ÓÃprintfº¯Êı
void USART3_printf(char* fmt,...); //´®¿Ú3µÄ×¨ÓÃprintfº¯Êı
void USART4_printf(char* fmt,...); //´®¿Ú4µÄ×¨ÓÃprintfº¯Êı
void USART5_printf(char* fmt,...); //´®¿Ú5µÄ×¨ÓÃprintfº¯Êı

#endif
