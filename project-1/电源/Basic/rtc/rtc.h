#ifndef __RTC_H
#define __RTC_H	 
#include "sys.h" 


//全局变量的声明，在rtc.c文件中定义
//以下2条是使用extern语句声明全局变量
//注意：这里不能给变量赋值
extern u16 ryear;
extern u8 rmon,rday,rhour,rmin,rsec,rweek;



void RTC_First_Config(void);//首次启用RTC的设置
void RTC_Config(void);//实时时钟初始化
u8 Is_Leap_Year(u16 year);//判断是否是闰年函数                    
u8 RTC_Get(void);//读出当前时间值	
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//写入当前时间
u8 RTC_Get_Week(u16 year,u8 month,u8 day);//按年月日计算星期




#endif


















