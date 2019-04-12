

/*
//杜洋工作室出品
//洋桃系列开发板应用程序
//关注微信公众号：洋桃电子
//洋桃开发板资料下载 www.DoYoung.net/YT 
//即可免费看所有教学视频，下载技术资料，技术疑难提问
//更多内容尽在 杜洋工作室主页 www.doyoung.net
*/

/*
TM1640数码管驱动芯片

《修改日志》
1-201708311855 创建。


*/

#include "TM1640.h"
#include "delay.h"

#define DEL  1   //宏定义 通信速率（默认为1，如不能通信可加大数值）

//地址模式的设置
//#define TM1640MEDO_ADD  0x40   //宏定义	自动加一模式
#define TM1640MEDO_ADD  0x44   //宏定义 固定地址模式（推荐）

//显示亮度的设置
//#define TM1640MEDO_DISPLAY  0x88   //宏定义 亮度  最小
//#define TM1640MEDO_DISPLAY  0x89   //宏定义 亮度
//#define TM1640MEDO_DISPLAY  0x8a   //宏定义 亮度
//#define TM1640MEDO_DISPLAY  0x8b   //宏定义 亮度
#define TM1640MEDO_DISPLAY  0x8c   //宏定义 亮度（推荐）
//#define TM1640MEDO_DISPLAY  0x8d   //宏定义 亮度
//#define TM1640MEDO_DISPLAY  0x8f   //宏定义 亮度 最大

#define TM1640MEDO_DISPLAY_OFF  0x80   //宏定义 亮度 关



void TM1640_start(){ //通信时序 启始（基础GPIO操作）（低层）
	GPIO_WriteBit(TM1640_GPIOPORT,TM1640_DIN,(BitAction)(1)); //接口输出高电平1	
	GPIO_WriteBit(TM1640_GPIOPORT,TM1640_SCLK,(BitAction)(1)); //接口输出高电平1	
	delay_us(DEL);
	GPIO_WriteBit(TM1640_GPIOPORT,TM1640_DIN,(BitAction)(0)); //接口输出0	
	delay_us(DEL);
	GPIO_WriteBit(TM1640_GPIOPORT,TM1640_SCLK,(BitAction)(0)); //接口输出0	
	delay_us(DEL);
}
void TM1640_stop(){ //通信时序 结束（基础GPIO操作）（低层）
	GPIO_WriteBit(TM1640_GPIOPORT,TM1640_DIN,(BitAction)(0)); //接口输出0	
	GPIO_WriteBit(TM1640_GPIOPORT,TM1640_SCLK,(BitAction)(1)); //接口输出高电平1	
	delay_us(DEL);
	GPIO_WriteBit(TM1640_GPIOPORT,TM1640_DIN,(BitAction)(1)); //接口输出高电平1	
	delay_us(DEL);
}
void TM1640_write(u8 date){	//写数据（低层）
	u8 i;
	u8 aa;
	aa=date;
	GPIO_WriteBit(TM1640_GPIOPORT,TM1640_DIN,(BitAction)(0)); //接口输出0	
	GPIO_WriteBit(TM1640_GPIOPORT,TM1640_SCLK,(BitAction)(0)); //接口输出0	
	for(i=0;i<8;i++){
		GPIO_WriteBit(TM1640_GPIOPORT,TM1640_SCLK,(BitAction)(0)); //接口输出0	
		delay_us(DEL);

		if(aa&0x01){
			GPIO_WriteBit(TM1640_GPIOPORT,TM1640_DIN,(BitAction)(1)); //接口输出高电平1	
			delay_us(DEL);
		}else{
			GPIO_WriteBit(TM1640_GPIOPORT,TM1640_DIN,(BitAction)(0)); //接口输出0	
			delay_us(DEL);
		}
		GPIO_WriteBit(TM1640_GPIOPORT,TM1640_SCLK,(BitAction)(1)); //接口输出高电平1	
		delay_us(DEL);
		aa=aa>>1;
   }
	GPIO_WriteBit(TM1640_GPIOPORT,TM1640_DIN,(BitAction)(0)); //接口输出0	
	GPIO_WriteBit(TM1640_GPIOPORT,TM1640_SCLK,(BitAction)(0)); //接口输出0	
}

void TM1640_Init(void){ //TM1640接口初始化
	GPIO_InitTypeDef  GPIO_InitStructure; 	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);       
    GPIO_InitStructure.GPIO_Pin = TM1640_DIN | TM1640_SCLK; //选择端口号（0~15或all）                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //选择IO接口工作方式       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）    
	GPIO_Init(TM1640_GPIOPORT, &GPIO_InitStructure);

	GPIO_WriteBit(TM1640_GPIOPORT,TM1640_DIN,(BitAction)(1)); //接口输出高电平1	
	GPIO_WriteBit(TM1640_GPIOPORT,TM1640_SCLK,(BitAction)(1)); //接口输出高电平1	
	TM1640_start();
	TM1640_write(TM1640MEDO_ADD); //设置数据，0x40,0x44分别对应地址自动加一和固定地址模式
	TM1640_stop();
	TM1640_start();
	TM1640_write(TM1640MEDO_DISPLAY); //控制显示，开显示，0x88,  0x89,  0x8a,  0x8b,  0x8c,  0x8d,  0x8e,  0x8f分别对应脉冲宽度为:
					 				  //------------------1/16,  2/16,  4/16,  10/16, 11/16, 12/16, 13/16, 14/16	 //0x80关显示
	TM1640_stop();	
				
}
void TM1640_led(u8 date){ //固定地址模式的显示输出8个LED控制
   TM1640_start();
   TM1640_write(TM1640_LEDPORT);	        //传显示数据对应的地址
   TM1640_write(date);	//传1BYTE显示数据
   TM1640_stop();
}
void TM1640_display(u8 address,u8 date){ //固定地址模式的显示输出
 	const u8 buff[21]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef,0x00};//数字0~9及0~9加点显示段码表
    //---------------   0    1    2    3    4    5    6    7    8    9    0.   1.   2.   3.   4.   5.   6.   7.   8.   9.   无   
   TM1640_start();
   TM1640_write(0xC0+address);	         //传显示数据对应的地址
   TM1640_write(buff[date]);				 //传1BYTE显示数据
   TM1640_stop();
}
void TM1640_display_add(u8 address,u8 date){	//地址自动加一模式的显示输出 
	u8 i;
 	const u8 buff[21]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef,0x00};//数字0~9及0~9加点显示段码表
    //---------------   0    1    2    3    4    5    6    7    8    9    0.   1.   2.   3.   4.   5.   6.   7.   8.   9.   无   
  TM1640_start();
   TM1640_write(0xC0+address);	         //设置起始地址
   for(i=0;i<16;i++){
      TM1640_write(buff[date]); 
   }
   TM1640_stop(); 
}


/*********************************************************************************************
 * 杜洋工作室 www.DoYoung.net
 * 洋桃电子 www.DoYoung.net/YT 
*********************************************************************************************/


