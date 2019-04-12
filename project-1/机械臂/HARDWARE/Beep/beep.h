#ifndef __BEEP_H__
#define __BEEP_H__
#include "sys.h"

#define ON  0
#define OFF 1
#define BEEP(a) if(a) \
											GPIO_SetBits(GPIOB,GPIO_Pin_13); \
								else \
											GPIO_ResetBits(GPIOB,GPIO_Pin_13)
								
#define	Beep PBout(13)  //Ñ¡ÖÐFLASH										

void Beep_Init(void);
void Beep_Test(void);
#endif
