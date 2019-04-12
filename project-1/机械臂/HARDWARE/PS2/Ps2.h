#ifndef __PS2_H__
#define __PS2_H__
#include "delay.h"
#include "sys.h"

//#define DI    GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)   //读取输入
//#define DO_H  GPIO_SetBits(GPIOC,GPIOC_Pin_14)           //命令位置高
//#define DO_L  GPIO_ResSetBitsSetBits(GPIOC,GPIOC_Pin_14) //命令位置低

//#define CS_H  GPIO_SetBits(GPIOC,GPIOC_Pin_15)           //CS置高
//#define CS_L  GPIO_ResSetBitsSetBits(GPIOC,GPIOC_Pin_15) //CS置低

//#define CLK_H  GPIO_SetBits(GPIOB,GPIOB_Pin_9)          //时钟置高
//#define CLK_L  GPIO_ResSetBitsSetBits(GPIOB,GPIOB_Pin_9) //时钟置低


#define DI   PAin(14)           //PA11  输入

#define DO_H PAout(13)=1        //命令位高
#define DO_L PAout(13)=0        //命令位低

#define CS_H PAout(12)=1       //CS拉高
#define CS_L PAout(12)=0       //CS拉低

#define CLK_H PAout(11)=1      //时钟拉高
#define CLK_L PAout(11)=0      //时钟拉低

//These are our button constants
#define PSB_SELECT      1
#define PSB_L3          2
#define PSB_R3          3
#define PSB_START       4
#define PSB_PAD_UP      5
#define PSB_PAD_RIGHT   6
#define PSB_PAD_DOWN    7
#define PSB_PAD_LEFT    8
#define PSB_L2          9
#define PSB_R2          10
#define PSB_L1          11
#define PSB_R1          12
#define PSB_GREEN       13
#define PSB_RED         14
#define PSB_BLUE        15
#define PSB_PINK        16
#define PSB_TRIANGLE    13
#define PSB_CIRCLE      14
#define PSB_CROSS       15
#define PSB_SQUARE      26

#define PSS_RX 5                //遥感值
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8

extern u8 Data[9];
extern u16 MASK[16];
extern u16 Handkey;

void PS2_Init(void);
u8 PS2_RedLight(void);//判断是否为红灯模式
void PS2_ReadData(void);
void PS2_Cmd(u8 CMD);		  //
u8 PS2_DataKey(void);		  //键值读取
u8 PS2_AnologData(u8 button); //得到一个遥感的模拟量
void PS2_ClearData(void);	  //清楚数据缓冲区
#endif
