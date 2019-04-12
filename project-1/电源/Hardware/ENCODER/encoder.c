

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
1-201709052329 创建。


*/



#include "encoder.h"


u8 KUP;//旋钮锁死标志（1为锁死）
u16 cou;

void ENCODER_Init(void){ //接口初始化
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO的初始化枚举结构	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);       
    GPIO_InitStructure.GPIO_Pin = ENCODER_L | ENCODER_D; //选择端口号                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //选择IO接口工作方式 //上拉电阻       
	GPIO_Init(ENCODER_PORT_A,&GPIO_InitStructure);	

    GPIO_InitStructure.GPIO_Pin = ENCODER_R; //选择端口号                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //选择IO接口工作方式 //上拉电阻       
	GPIO_Init(ENCODER_PORT_B,&GPIO_InitStructure);				
}

u8 ENCODER_READ(void){ //编码器状态读取
	u8 a;//存放按键的值
	u8 kt;
	a=0;
	if(GPIO_ReadInputDataBit(ENCODER_PORT_A,ENCODER_L))KUP=0;	//判断旋钮是否解除锁死
	if(!GPIO_ReadInputDataBit(ENCODER_PORT_A,ENCODER_L)&&KUP==0){ //判断是否旋转旋钮，同时判断是否有旋钮锁死
		delay_us(100);
		kt=GPIO_ReadInputDataBit(ENCODER_PORT_B,ENCODER_R);	//把旋钮另一端电平状态记录
		delay_ms(3); //延时
		if(!GPIO_ReadInputDataBit(ENCODER_PORT_A,ENCODER_L)){ //去抖
			if(kt==0){ //用另一端判断左或右旋转
				a=1;//右转
			}else{
				a=2;//左转
			}
			cou=0; //初始锁死判断计数器
			while(!GPIO_ReadInputDataBit(ENCODER_PORT_A,ENCODER_L)&&cou<60000){ //等待放开旋钮，同时累加判断锁死
				cou++;KUP=1;delay_us(20); //
			}
		}
	}
	if(!GPIO_ReadInputDataBit(ENCODER_PORT_A,ENCODER_D)&&KUP==0){ //判断旋钮是否按下  
		delay_ms(20);
		if(!GPIO_ReadInputDataBit(ENCODER_PORT_A,ENCODER_D)){ //去抖动
			a=3;//在按键按下时加上按键的状态值
			//while(ENCODER_D==0);	等等旋钮放开
		}
	}
	return a;
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
