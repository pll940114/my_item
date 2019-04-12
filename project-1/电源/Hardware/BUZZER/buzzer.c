

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
1-201709012309 创建。


*/


#include "buzzer.h"
#include "delay.h"

void BUZZER_Init(void){ //蜂鸣器的接口初始化
	GPIO_InitTypeDef  GPIO_InitStructure; 	
    GPIO_InitStructure.GPIO_Pin = BUZZER; //选择端口号                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //选择IO接口工作方式       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）    
	GPIO_Init(BUZZERPORT, &GPIO_InitStructure);	
	
	GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(1)); //蜂鸣器接口输出高电平1		
}

void BUZZER_BEEP1(void){ //蜂鸣器响一声
	u16 i;
	for(i=0;i<200;i++){
		GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(0)); //蜂鸣器接口输出0
		delay_us(500); //延时		
		GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(1)); //蜂鸣器接口输出高电平1
		delay_us(500); //延时		
	}
}

uc16 music1[78]={ //音乐1的数据表（奇数是音调，偶数是长度）
330,750,
440,375,
494,375,
523,750,
587,375,
659,375,
587,750,
494,375,
392,375,
440,1500,
330,750,
440,375,
494,375,
523,750,
587,375,
659,375,
587,750,
494,375,
392,375,
784,1500,
659,750,
698,375,
784,375,
880,750,
784,375,
698,375,
659,750,
587,750,
659,750,
523,375,
494,375,
440,750,
440,375,
494,375,
523,750,
523,750,
494,750,
392,750,
440,3000
};

void MIDI_PLAY(void){ //MIDI音乐
	u16 i,e;
	for(i=0;i<39;i++){
		for(e=0;e<music1[i*2]*music1[i*2+1]/1000;e++){
			GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(0)); //蜂鸣器接口输出0
			delay_us(500000/music1[i*2]); //延时		
			GPIO_WriteBit(BUZZERPORT,BUZZER,(BitAction)(1)); //蜂鸣器接口输出高电平1
			delay_us(500000/music1[i*2]); //延时	
		}	
	}
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
