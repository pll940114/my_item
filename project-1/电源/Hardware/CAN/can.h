#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"


#define CAN_INT_ENABLE	0	//1 开接收中断，0 关接收中断

//设置模式和波特率
//波特率=(pclk1/((1+8+7)*9)) = 36Mhz/16/9 = 250Kbits设定了一个时间单位的长度9
#define tsjw	CAN_SJW_1tq	//设置项目（1~4）
#define tbs1	CAN_BS1_8tq	//设置项目（1~16）
#define tbs2	CAN_BS2_7tq	//设置项目（1~8）
#define brp		9	//设置项目



u8 CAN1_Configuration(void);//初始化
u8 CAN_Send_Msg(u8* msg,u8 len);//发送数据
u8 CAN_Receive_Msg(u8 *buf);//接收数据
		 				    
#endif
