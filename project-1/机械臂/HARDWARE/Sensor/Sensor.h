
#ifndef __SENSOR_H__
#define __SENSOR_H__
#include "sys.h"

#define Sensor_gm GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)
#define Sensor_hw    GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)
void Sensor_init(void);
void sensor_action(void);


#endif

