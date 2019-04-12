#include "delay.h"

u32 a;
static __IO u32 TimingDelay;
/**************************中断周期设置函数*********************************************/
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	if (SysTick_Config(SystemCoreClock/1000000))	// ST3.5.0库版本
	{ 
		while (1);
	}
	
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}
/***************************************************************************************/
/*************************微秒延时函数**************************************************/
void Delay_us(u32 nTime)           
{ 
	TimingDelay = nTime;	
  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;

	while(TimingDelay != 0);
	
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}
/***************************************************************************************/
/*************************毫秒延时函数**************************************************/
void Delay_ms(u32 nTime)           
{ 
	TimingDelay = nTime*1000;	
  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;

	while(TimingDelay != 0);
	
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}
/***************************************************************************************/
/**************************延时过程处理*************************************************/
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}
/***************************************************************************************/
