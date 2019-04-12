#include "stm32f10x.h" 
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "usart.h"

unsigned char flag_uart2_rev=0;
char str[10];
int a=0;

int main (void){//主程序
	RCC_Configuration(); //系统时钟初始化 
	USART2_Init(115200);
	LED_Init();

	while(1){
		static unsigned int i=0;
		u8 Res;
		/*测试*/		
		GPIO_WriteBit(LEDPORT,JI,(BitAction)(1)); //LED控制
		GPIO_WriteBit(LEDPORT,AC,(BitAction)(1)); //LED控制

		if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET)  
		{ 
		Res =USART_ReceiveData(USART2);	
		USART2_printf("%c",Res);
		}
		if(Res==49&&(a%2))
		{
			GPIO_WriteBit(LEDPORT,AC,(BitAction)(0));
			delay_ms(1000);
			GPIO_WriteBit(LEDPORT,JI,(BitAction)(0));
			a=a+1;
			delay_ms(11000);
		}	
			if(Res==49&&!(a%2))
		{
			GPIO_WriteBit(LEDPORT,AC,(BitAction)(1));
			delay_ms(1000);
			GPIO_WriteBit(LEDPORT,JI,(BitAction)(1));
			a=a+1;
			delay_ms(11000);
		}
		if(Res==48){
			flag_uart2_rev=0;
		}
	}
}
