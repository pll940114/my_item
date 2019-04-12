#include "Ps2.h"
#include "usart.h"
 #include "stm32f10x_pwr.h"
/*********************************************************
File：PS2驱动程序
Description: PS2驱动程序
**********************************************************/	 
u16 Handkey;
u8 Comd[2]={0x01,0x42};	//开始命令。请求数据
u8 Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //数据存储数组
u16 MASK[]={
    PSB_SELECT,
    PSB_L3,
    PSB_R3 ,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1 ,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
	};	//按键值与按键明



void PS2_Init(void)
{
    //输入  DI->PB9   		
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;//PA14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //下拉模式
	//PWR_BackupAccessCmd(ENABLE);
	//RCC_LSEConfig(RCC_LSE_OFF);
	//BKP_TamperPinCmd(DISABLE);
	//PWR_BackupAccessCmd(DISABLE);
 	GPIO_Init(GPIOA,&GPIO_InitStructure);
    //  DO->PA12    CS->PA8  CLK->PB9
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 	
		
	
}


//向手柄发送命令
void PS2_Cmd(u8 CMD)
{
	volatile u16 ref=0x01;
	Data[1] = 0;
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD)
		{
			DO_H;                   //输出以为控制位
		}
		else DO_L;

		CLK_H;                        //时钟拉高
		//delay_us(50);
		Delay_us(50);
		CLK_L;
		//delay_us(50);
		Delay_us(50);
		CLK_H;
		if(DI)
			Data[1] = ref|Data[1];
	}
}
//判断是否为红灯模式
//返回值；0，红灯模式
//		  其他，其他模式
u8 PS2_RedLight(void)
{
	CS_L;
	PS2_Cmd(Comd[0]);  //开始命令
	PS2_Cmd(Comd[1]);  //请求数据
	CS_H;
	if( Data[1] == 0X73)   return 0 ;
	else return 1;

}
//读取手柄数据
void PS2_ReadData(void)
{
	volatile u8 byte=0;
	volatile u16 ref=0x01;

	CS_L;

	PS2_Cmd(Comd[0]);  //开始命令
	PS2_Cmd(Comd[1]);  //请求数据

	for(byte=2;byte<9;byte++)          //开始接受数据
	{
		for(ref=0x01;ref<0x100;ref<<=1)
		{
			CLK_H;
			CLK_L;
			//delay_us(50);
			Delay_us(50);
			CLK_H;
		      if(DI)
		      Data[byte] = ref|Data[byte];
		}
       //delay_us(50);
		   Delay_us(50);
	}
	CS_H;	
}

//对读出来的PS2的数据进行处理      只处理了按键部分         默认数据是红灯模式  只有一个按键按下时
//按下为0， 未按下为1

u8 PS2_DataKey()
{
	static u8 temp=1;
	u8 index;
	PS2_ClearData();
	PS2_ReadData();

	Handkey=(Data[4]<<8)|Data[3];     //这是16个按键  按下为0， 未按下为1
	for(index=0;index<16;index++)
	{	    
		if((Handkey&(1<<(MASK[index]-1)))==0)
		{
			temp=(Handkey&(1<<(MASK[index]-1)));
			//Delay_ms(1000);  
		//	printf("  \r\n !%d! \r\n",temp);
			return index+1;
		}
		
	}

	return 0;          //没有任何按键按下
}

//得到一个摇杆的模拟量	 范围0~255
u8 PS2_AnologData(u8 button)
{
	return Data[button];
}

//清除数据缓冲区
void PS2_ClearData()
{
	u8 a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}




