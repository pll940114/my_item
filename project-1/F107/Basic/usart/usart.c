#include "main.h"
#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
extern unsigned char flag_uart2_rev;

//使UASRT串口可用printf函数发送
//在usart.h文件里可更换使用printf函数的串口号	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE {
	int handle; 
}; 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x){ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f){      
	while((USART_n->SR&0X40)==0);//循环发送,直到发送完毕   
    USART_n->DR = (u8) ch;      
	return ch;
}
#endif 

/*
USART2串口相关程序
*/
#if EN_USART2   //USART2使用与屏蔽选择
u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART2_RX_STA=0;       //接收状态标记	  

/*
USART2专用的printf函数
当同时开启2个以上串口时，printf函数只能用于其中之一，其他串口要自创独立的printf函数
调用方法：USART2_printf("123"); //向USART2发送字符123
*/
void USART2_printf (char *fmt, ...){ 
	char buffer[USART2_REC_LEN+1];  // 数据长度
	u8 i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, USART2_REC_LEN+1, fmt, arg_ptr);
	while ((i < USART2_REC_LEN) && (i < strlen(buffer))){
        USART_SendData(USART2, (u8) buffer[i++]);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); 
	}
	va_end(arg_ptr);
}
void USART2_Init(u32 bound)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	USART_InitTypeDef   USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO,ENABLE);
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE ); // USART2时钟使能
  GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
    
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);//开启ENABLE/关闭DISABLE中断
	USART_Cmd(USART2, ENABLE); // USART2使能
	
	//Configure the USART2_Tx PD5 as Alternate function Push-Pull 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//Configure the USART2_Rx PD6 as input power-UP
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//Usart2 NVIC 配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器 
}


#endif	

/*
UART4串口相关程序
*/
#if EN_UART4   //UART4使用与屏蔽选择
u8 USART4_RX_BUF[USART4_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART4_RX_STA=0;       //接收状态标记	  

/*
USART4专用的printf函数
当同时开启2个以上串口时，printf函数只能用于其中之一，其他串口要自创独立的printf函数
调用方法：USART4_printf("123"); //向UART4发送字符123
*/
void USART4_printf (char *fmt, ...){ 
	char buffer[USART4_REC_LEN+1];  // 数据长度
	u8 i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, USART4_REC_LEN+1, fmt, arg_ptr);
	while ((i < USART4_REC_LEN) && (i < strlen(buffer))){
        USART_SendData(UART4, (u8) buffer[i++]);
        while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET); 
	}
	va_end(arg_ptr);
}
void USART4_Init(u32 bound)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	USART_InitTypeDef   USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE );
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE );
    
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启ENABLE/关闭DISABLE中断
	USART_Cmd(UART4, ENABLE); // UART4使能
	
	//Configure the UART4_Tx PC10 as Alternate function Push-Pull 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//Configure the UART4_Rx PC11 as input power-UP
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//Usart4 NVIC 配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器 
}
void UART4_IRQHandler(void){ //串口4中断服务程序（固定的函数名不能修改）	
	u8 a;
	if(USART_GetFlagStatus(UART4,USART_FLAG_RXNE) != RESET){
			a =USART_ReceiveData(UART4);
		  if(a=='A'){  
			GPIO_SetBits(GPIOC,GPIO_Pin_13);
		}
			 if(a=='B'){  
			GPIO_SetBits(GPIOC,GPIO_Pin_2);
		}
			  if(a=='C'){  
			GPIO_SetBits(GPIOC,GPIO_Pin_3);
		}
	}
} 

#endif	

/*
UART5串口相关程序
*/
#if EN_UART5   //UART5使用与屏蔽选择
u8 USART5_RX_BUF[USART5_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART5_RX_STA=0;       //接收状态标记	  

/*
UART5专用的printf函数
当同时开启2个以上串口时，printf函数只能用于其中之一，其他串口要自创独立的printf函数
调用方法：USART5_printf("123"); //向UART5发送字符123
*/
void USART5_printf (char *fmt, ...){ 
	char buffer[USART5_REC_LEN+1];  // 数据长度
	u8 i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, USART5_REC_LEN+1, fmt, arg_ptr);
	while ((i < USART5_REC_LEN) && (i < strlen(buffer))){
        USART_SendData(UART5, (u8) buffer[i++]);
        while (USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET); 
	}
	va_end(arg_ptr);
}
void USART5_Init(u32 bound)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	USART_InitTypeDef   USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE );
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE );

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART5, &USART_InitStructure);
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启ENABLE/关闭DISABLE中断
	USART_Cmd(UART5, ENABLE); // UART5使能
	
	//Configure the UART5_Tx PC12 as Alternate function Push-Pull 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//Configure the USART2_Rx PD2 as input power-UP
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//Usart5 NVIC 配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器 
}
void UART5_IRQHandler(void){ //串口5中断服务程序（固定的函数名不能修改）	
	u8 a;
	unsigned char tcp_data[]="A";
	struct tcp_pcb *pcb;
	if(USART_GetFlagStatus(UART5,USART_FLAG_RXNE) != RESET){
			a =USART_ReceiveData(UART5);
		  if(a=='A'){
				USART2_printf("#1GC1\r\n");   
				GPIO_SetBits(GPIOC,GPIO_Pin_13);
			/******************************/
				while(1){
				pcb = Check_TCP_Connect();//检查连接
				if(pcb != 0)
				{	
					TCP_Client_Send_Data(pcb,tcp_data,sizeof(tcp_data));	//向服务器发送数据
				}
				Delay_s(0xfffff);											//延时
			}
				System_Periodic_Handle();		
			/******************************/
		}
			 if(a=='B'){
			/******************************/
				USART2_printf("#2GC1\r\n");   
			  GPIO_SetBits(GPIOC,GPIO_Pin_2);
				while(1){
				pcb = Check_TCP_Connect();//检查连接
				if(pcb != 0)
				{	
					TCP_Client_Send_Data(pcb,tcp_data,sizeof(tcp_data));	//向服务器发送数据
				}
				Delay_s(0xfffff);											//延时
			}
				System_Periodic_Handle();		
			/******************************/
		}
			  if(a=='C'){
			/******************************/
				USART2_printf("#6GC1\r\n");   
			  GPIO_SetBits(GPIOC,GPIO_Pin_3);
				while(1){
				pcb = Check_TCP_Connect();//检查连接
				if(pcb != 0)
				{	
					TCP_Client_Send_Data(pcb,tcp_data,sizeof(tcp_data));	//向服务器发送数据
				}
				Delay_s(0xfffff);											//延时
			}
				System_Periodic_Handle();		
			/******************************/ 
		}
	}
} 
#endif	
