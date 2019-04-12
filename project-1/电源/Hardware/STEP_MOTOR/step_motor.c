

/*
//杜洋工作室出品
//洋桃系列开发板应用程序
//关注微信公众号：洋桃电子
//洋桃开发板资料下载 www.DoYoung.net/YT 
//即可免费看所有教学视频，下载技术资料，技术疑难提问
//更多内容尽在 杜洋工作室主页 www.doyoung.net
*/

/*
《修改日志》
1-201708202309 创建。


*/


#include "step_motor.h"

u8 STEP; 


void STEP_MOTOR_Init(void){ //接口初始化
	GPIO_InitTypeDef  GPIO_InitStructure; 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE); //APB2外设GPIO时钟使能      
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//启动AFIO重映射功能时钟    
    GPIO_InitStructure.GPIO_Pin = STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D; //选择端口                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //选择IO接口工作方式       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）    
	GPIO_Init(STEP_MOTOR_PORT, &GPIO_InitStructure);
	//必须将禁用JTAG功能才能做GPIO使用
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);// 改变指定管脚的映射,完全禁用JTAG+SW-DP	
	STEP_MOTOR_OFF(); //初始状态是断电状态 			
}

void STEP_MOTOR_OFF (void){//电机断电
	GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D);//各接口置0
}

void STEP_MOTOR_8A (u8 a,u16 speed){//电机单步8拍
	switch (a){
		case 0:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A);//1
			break;
		case 1:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);//1
			break;
		case 2:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_C | STEP_MOTOR_D);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B);//1
			break;
		case 3:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);//1
			break;
		case 4:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_D);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C);//1
			break;
		case 5:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);//1
			break;
		case 6:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_D);//1
			break;
		case 7:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);//1
			break;
		default:
			break;
	}
	delay_ms(speed); //延时
	STEP_MOTOR_OFF(); //进入断电状态，防电机过热
}

void STEP_MOTOR_NUM (u8 RL,u16 num,u8 speed){//电机按步数运行
	u16 i;
	for(i=0;i<num;i++){	
		if(RL==1){ //当RL=1右转，RL=0左转
			STEP++;
			if(STEP>7)STEP=0;
		}else{
			if(STEP==0)STEP=8;
			STEP--;
		}
		STEP_MOTOR_8A(STEP,speed);
	}
}

void STEP_MOTOR_LOOP (u8 RL,u8 LOOP,u8 speed){//电机按圈数运行
	STEP_MOTOR_NUM(RL,LOOP*4076,speed); 
}
 
/*********************************************************************************************
 * 杜洋工作室 www.DoYoung.net
 * 洋桃电子 www.DoYoung.net/YT 
*********************************************************************************************/


/*
选择IO接口工作方式：
GPIO_Mode_AIN 模拟输入
GPIO_Mode_IN_FLOATING 浮空输入
GPIO_Mode_IPD 下拉输入
GPIO_Mode_IPU 上拉输入
GPIO_Mode_Out_PP 推挽输出
GPIO_Mode_Out_OD 开漏输出
GPIO_Mode_AF_PP 复用推挽输出
GPIO_Mode_AF_OD 复用开漏输出
*/
