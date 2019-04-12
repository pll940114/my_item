#ifndef __BUZZER_H
#define __BUZZER_H	 
#include "sys.h"

#define BUZZERPORT	GPIOB	//定义IO接口
#define BUZZER	GPIO_Pin_5	//定义IO接口



void BUZZER_Init(void);//初始化
void BUZZER_BEEP1(void);//响一声
void MIDI_PLAY(void);
		 				    
#endif
