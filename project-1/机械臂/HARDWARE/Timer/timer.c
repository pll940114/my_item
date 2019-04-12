/**********************************************************************************/
#include "timer.h"
/**********************定时器2中断配置函数*****************************************/
void TIM2_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**************************************************************************************/
/**********************定时器3中断配置函数*********************************************/
void TIM3_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**************************************************************************************/
/**********************定时器4中断配置函数*********************************************/
void TIM4_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**************************************************************************************/

/************************定时器2配置函数***********************************************/
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    
	  TIM_DeInit(TIM2);											/* 重新启动定时器 */

    TIM_TimeBaseStructure.TIM_Period=2505;		 				/* 自动重装载寄存器周期的值(计数值) */
    TIM_TimeBaseStructure.TIM_Prescaler=71;						/* 时钟预分频数 72M/72 */
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* 外部时钟采样分频 */
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; 
	  TIM_TimeBaseStructure.TIM_ClockDivision=0;  
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	/* 向上计数模式 */
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM2,TIM_FLAG_Update);						/* 清除溢出中断标志 */

    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);					/* 开启中断触发*/   
	
	  TIM_Cmd(TIM2,DISABLE);	


}
/**************************************************************************************/
/************************定时器3配置函数***********************************************/
void TIM3_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    
	  TIM_DeInit(TIM3);											/* 重新启动定时器 */

    TIM_TimeBaseStructure.TIM_Period=9;		 				/* 自动重装载寄存器周期的值(计数值) */
    TIM_TimeBaseStructure.TIM_Prescaler=71;						/* 时钟预分频数 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=0; 		/* 外部时钟采样分频 */
	  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; 
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	/* 向上计数模式 */
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM3,TIM_FLAG_Update);						/* 清除溢出中断标志 */

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);					/* 开启中断触发*/   
	
	   TIM_Cmd(TIM3,DISABLE);	
}
/**************************************************************************************/
/************************定时器4配置函数***********************************************/
void TIM4_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    
	  TIM_DeInit(TIM4);											/* 重新启动定时器 */

    TIM_TimeBaseStructure.TIM_Period=2000;		 				/* 自动重装载寄存器周期的值(计数值) */
    TIM_TimeBaseStructure.TIM_Prescaler=71;						/* 时钟预分频数 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* 外部时钟采样分频 */
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; 
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	/* 向上计数模式 */
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM4,TIM_FLAG_Update);						/* 清除溢出中断标志 */

    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);					/* 开启中断触发*/   
	
	  TIM_Cmd(TIM4,DISABLE);	
}
/**************************************************************************************/
/************************定时器配置函数************************************************/
void TIM5_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);
    
	  TIM_DeInit(TIM5);											/* 重新启动定时器 */

    TIM_TimeBaseStructure.TIM_Period=65535;		 				/* 自动重装载寄存器周期的值(计数值) */
    TIM_TimeBaseStructure.TIM_Prescaler=71;						/* 时钟预分频数 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* 外部时钟采样分频 */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	/* 向上计数模式 */
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM5, TIM_FLAG_Update);						/* 清除溢出中断标志 */

    TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);					/* 开启中断触发*/    
	
	  TIM_Cmd(TIM5,DISABLE);									    /* 关闭定时器 */ 
}
/**************************************************************************************/
/************************定时器初始化函数*********************************************/
void Timer_Init(void)
{
	TIM2_NVIC_Configuration();
	TIM2_Configuration();

//TIM3_NVIC_Configuration();
	//TIM3_Configuration();

	TIM4_NVIC_Configuration();
	TIM4_Configuration();

	//TIM5_NVIC_Configuration();
	//TIM5_Configuration();
}
/**************************************************************************************/
/************************开启定时器函数************************************************/
void Timer_ON(void)
{	
	TIM_Cmd(TIM2,ENABLE);
	//TIM_Cmd(TIM3,ENABLE);
	TIM_Cmd(TIM4,ENABLE);	
}
/**************************************************************************************/
/************************关闭定时器函数************************************************/
void Timer_OFF(void)
{	
	TIM_Cmd(TIM2,DISABLE);
	//TIM_Cmd(TIM3,DISABLE);
	TIM_Cmd(TIM4,DISABLE);	
}
/**************************************************************************************/

