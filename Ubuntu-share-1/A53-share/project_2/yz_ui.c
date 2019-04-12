#include <stdio.h>
#include <time.h>
#include "bmp.h"
#include "usart.h"
#include "touch_screen.h"
#define   TRUE 	"1\n"
#define   FAULT "0\n"

int yz_x[6]={120,220,320,420,520,620}; //验证码框的坐标

/***********************************
UI界面的定义、功能的确定及区域的规划
***********************************/

char define_xy(int x,int y)
{				
	int chr;
	if(x>=280&&x<=340 )
	{
	    if(y>=160&&y<=220)
		chr='1';
	}
	if(x>=370&&x<=430 )
	{
		if(y>=160&&y<=220)
		chr='2';
	}
	if(x>=460&&x<=520 )
	{
		if(y>=160&&y<=220)
		chr='3';
	}	
	if(x>=280&&x<=340 )
	{
		if(y>=240&&y<=300)
		chr='4';
	}
	if(x>=370&&x<=430 )
	{
		if(y>=240&&y<=300)
		chr='5';
	}
	if(x>=460&&x<=520 )
	{
		if(y>=240&&y<=300)
		chr='6';
	}		
	if(x>=280&&x<=340 )
	{
		if(y>=320&&y<=380)
		chr='7';
	}
	if(x>=370&&x<=430 )
	{
		if(y>=320&&y<=380)
		chr='8';
	}
	if(x>=460&&x<=520 )
	{
		if(y>=320&&y<=380)
		chr='9';
	}	
	if(x>=370&&x<=430 )
	{
		if(y>=400&&y<=460)
		chr='0';
	}
	//确认
	if(x>=240&&x<=340 )
	{
		if(y>=400&&y<=460)
		chr='A';
	}
	//删除
	if(x>=460&&x<=560 )
	{
		if(y>=400&&y<=460)
		chr='D';
	}	
	//验证码
	if(x>=60&&x<=200 )
	{
		if(y>=400&&y<=460)
		chr='Y';
	}		
	//二维码
	if(x>=600&&x<=740 )
	{
		if(y>=400&&y<=460)
		chr='E';
	}
	return chr;
}

void case_n(char*buf,char chr)
{
	int p=0;
	static int i= 0;
	buf[i]=chr;
	if( i< 6 || chr=='D' || chr=='A')			
		switch (chr)
		{
			printf("%d",i);
			case '1' :system("madplay -a +18 0004.mp3 &");show_bmp("yzm1.bmp",yz_x[i],60);i++;break;
			case '2' :system("madplay -a +18 0004.mp3 &");show_bmp("yzm2.bmp",yz_x[i],60);i++;break;
			case '3' :system("madplay -a +18 0004.mp3 &");show_bmp("yzm3.bmp",yz_x[i],60);i++;break;
			case '4' :system("madplay -a +18 0004.mp3 &");show_bmp("yzm4.bmp",yz_x[i],60);i++;break;
			case '5' :system("madplay -a +18 0004.mp3 &");show_bmp("yzm5.bmp",yz_x[i],60);i++;break;
			case '6' :system("madplay -a +18 0004.mp3 &");show_bmp("yzm6.bmp",yz_x[i],60);i++;break;
			case '7' :system("madplay -a +18 0004.mp3 &");show_bmp("yzm7.bmp",yz_x[i],60);i++;break;
			case '8' :system("madplay -a +18 0004.mp3 &");show_bmp("yzm8.bmp",yz_x[i],60);i++;break;
			case '9' :system("madplay -a +18 0004.mp3 &");show_bmp("yzm9.bmp",yz_x[i],60);i++;break;
			case '0' :system("madplay -a +18 0004.mp3 &");show_bmp("yzm0.bmp",yz_x[i],60);i++;break;
			case 'D' :            									 //删除
						if(i==0)break;
						else
						{
							system("madplay -a +18 0005.mp3 &");
							i--;
							show_bmp("yzm.bmp",yz_x[i],60);
							/* show_bmp("00.bmp",400,60); */
							break;
						}
			case 'A' :												//确认
					    if(i==6)
						{
							system("madplay -a +18 0005.mp3 &");//MP3播放
							show_bmp("1.bmp",0,0);
							buf[i]='\0';
							usleep(500);
							system("killall -9 madplay");
							{
								buf[i]='\n';
								buf[i+1]='\0';
								printf("%s\n",buf);
								printf("%d\n",strlen(buf));
								usart_send(buf);                    //串口发送验证信息
								//usart_send(TRUE);					//串口发送通知语音模块TRUE————‘1’
							}
							if(usart_rev()==1)
							{
								show_bmp("qjz.bmp",200,150);		//取件中
								show_bmp("00.bmp",370,400);
								system("madplay -a +10 0001.mp3 &");//MP3播放
								sleep(1);
								show_bmp("qjcg.bmp",200,150);		//取件成功
								show_bmp("00.bmp",370,400); 
								system("madplay -a +10 0002.mp3 &");//MP3播放
								sleep(3);
								i=0;
							    show_bmp("1.bmp",0,0);					//恢复验证UI
								break;
							}
							if(usart_rev()==0)
							{
								show_bmp("00.bmp",370,400);
								show_bmp("yzmcw.bmp",200,150);		//验证码错误
								//usart_send(FAULT); 					//串口发送通知语音模块FAULT————‘0’
								system("madplay -a +10 0003.mp3 &");
								sleep(3);
								i=0;
							    show_bmp("1.bmp",0,0);					//恢复验证UI
								break;
							}
						}
						else break;;
			case'E' :system("madplay -a +18 0006.mp3 &");show_bmp("ewm.bmp",0,0);i=0;break;
			case'Y'	:system("madplay -a +18 0006.mp3 &");show_bmp("1.bmp",0,0);i=0;break;
			default	:break;	
		}
}

int main()
{
	struct in 	xy;  				//定义坐标结构体
	char 		c;
	char 		buf[10];
	char*       B;
	int 		bol,boll=1;
	while(1)
	{
		show_bmp("2.bmp",0,0);    	//待机界面
		while(1)
		{
			bol=get_xy(&xy);
			if(!bol)
			{	
				if(boll==1)
				{
					system("madplay -a +18 0007.mp3 &");
					show_bmp("1.bmp",0,0);
					boll=0;					
				}
				c=define_xy(xy.x ,xy.y);
				case_n(buf, c);		
			}
		}
	}
	return 0;
}