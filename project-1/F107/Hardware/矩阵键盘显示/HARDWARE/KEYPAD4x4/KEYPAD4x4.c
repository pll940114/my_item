

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
1-201708202312 创建。


*/



#include "KEYPAD4x4.h"

void KEYPAD4x4_Init(void){ //微动开关的接口初始化
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO的初始化枚举结构	
    GPIO_InitStructure.GPIO_Pin = KEYa | KEYb | KEYc | KEYd; //选择端口号（0~15或all）                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //选择IO接口工作方式 //上拉电阻       
	GPIO_Init(KEYPAD4x4PORT,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = KEY1 | KEY2 | KEY3 | KEY4; //选择端口号（0~15或all）                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //选择IO接口工作方式   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）     
	GPIO_Init(KEYPAD4x4PORT,&GPIO_InitStructure);

	GPIO_ResetBits(KEYPAD4x4PORT,KEY1|KEY2|KEY3|KEY4);
	GPIO_SetBits(KEYPAD4x4PORT,KEYa|KEYb|KEYc|KEYd);
}
void KEYPAD4x4_Init2(void){ //微动开关的接口初始化2（用于IO工作方式反转）
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO的初始化枚举结构	
    GPIO_InitStructure.GPIO_Pin = KEY1 | KEY2 | KEY3 | KEY4; //选择端口号（0~15或all）                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //选择IO接口工作方式 //上拉电阻       
	GPIO_Init(KEYPAD4x4PORT,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = KEYa | KEYb | KEYc | KEYd; //选择端口号（0~15或all）                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //选择IO接口工作方式 //上拉电阻  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）     
	GPIO_Init(KEYPAD4x4PORT,&GPIO_InitStructure);

	GPIO_SetBits(KEYPAD4x4PORT,KEY1|KEY2|KEY3|KEY4);
	GPIO_ResetBits(KEYPAD4x4PORT,KEYa|KEYb|KEYc|KEYd);
}
u8 KEYPAD4x4_Read (void){//键盘处理函数
	u8 a=0,b=0;//定义变量
	KEYPAD4x4_Init();//初始化IO
	if(!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYa) ||  	//查寻键盘口的值是否变化
		!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYb) || 
		!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYc) || 
		!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYd)){
		delay_ms (20);//延时20毫秒
		if(!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYa) ||  	//查寻键盘口的值是否变化
			!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYb) || 
			!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYc) || 
			!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEYd)){
	  		a = GPIO_ReadInputData(KEYPAD4x4PORT)&0xff;//键值放入寄存器a
		}
		KEYPAD4x4_Init2();//IO工作方式反转
		b = GPIO_ReadInputData(KEYPAD4x4PORT)&0xff;//将第二次取得值放入寄存器b
		a = a|b;//将两个数据相或
		switch(a){//对比数据值
			case 0xee: b = 36; break;//对比得到的键值给b一个应用数据
			case 0xed: b = 13; break;
			case 0xeb: b = 10; break;
			case 0xe7: b = 20; break;
			case 0xde: b = 35; break;
			case 0xdd: b = 9; break;
			case 0xdb: b = 8; break;
			case 0xd7: b = 7; break;
			case 0xbe: b = 34; break;
			case 0xbd: b = 6; break;
			case 0xbb: b = 5; break;
			case 0xb7: b = 4; break;
			case 0x7e: b = 33; break;
			case 0x7d: b = 3; break;
			case 0x7b: b = 2; break;
			case 0x77: b = 1; break;
			default: b = 0; break;//键值错误处理
		}
		while(!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEY1) ||  	//等待按键放开
			!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEY2) || 
			!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEY3) || 
			!GPIO_ReadInputDataBit(KEYPAD4x4PORT,KEY4));
		delay_ms (20);//延时20毫秒
	}
return (b);//将b作为返回值
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
