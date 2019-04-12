#ifndef __STEP_MOTOR_H
#define __STEP_MOTOR_H	 
#include "sys.h"
#include "delay.h"

extern u8 STEP; //定义单步计数 全局变量

#define STEP_MOTOR_PORT	GPIOB	//定义IO接口所在组
#define STEP_MOTOR_A	GPIO_Pin_3	//定义IO接口
#define STEP_MOTOR_B	GPIO_Pin_4	//定义IO接口
#define STEP_MOTOR_C	GPIO_Pin_8	//定义IO接口
#define STEP_MOTOR_D	GPIO_Pin_9	//定义IO接口



void STEP_MOTOR_Init(void);//初始化
void STEP_MOTOR_OFF (void);//断电状态
void STEP_MOTOR_8A (u8 a,u16 speed);
void STEP_MOTOR_NUM (u8 RL,u16 num,u8 speed);//电机按步数运行
void STEP_MOTOR_LOOP (u8 RL,u8 LOOP,u8 speed);//电机按圈数运行


		 				    
#endif
