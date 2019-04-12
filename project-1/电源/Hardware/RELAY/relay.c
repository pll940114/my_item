

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





注意：
本程序所占用的GPIO接口PA13、PA14上电后为JTAG功能，
需要在RCC程序里启动AFIO时钟，再在RELAY_Init函数里加入：
GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
// 改变指定管脚的映射,完全禁用JTAG+SW-DP才能将JATG接口重定义为GPIO

*/




#include "relay.h"

void RELAY_Init(void){ //继电器的接口初始化
	GPIO_InitTypeDef  GPIO_InitStructure; 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE); //APB2外设时钟使能      
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//启动AFIO重映射功能时钟    
    GPIO_InitStructure.GPIO_Pin = RELAY1 | RELAY2; //选择端口号（0~15或all）                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //选择IO接口工作方式       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）    
	GPIO_Init(RELAYPORT, &GPIO_InitStructure);
	//必须将禁用JTAG功能才能做GPIO使用
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);// 改变指定管脚的映射,完全禁用JTAG+SW-DP
	GPIO_ResetBits(RELAYPORT,RELAY1 | RELAY2); //都为低电平（0） 初始为关继电器							
}

void RELAY_1(u8 c){ //继电器的控制程序（c=0继电器放开，c=1继电器吸合）
	GPIO_WriteBit(RELAYPORT,RELAY1,(BitAction)(c));//通过参数值写入接口
}
void RELAY_2(u8 c){ //继电器的控制程序（c=0继电器放开，c=1继电器吸合）
	GPIO_WriteBit(RELAYPORT,RELAY2,(BitAction)(c));//通过参数值写入接口
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
