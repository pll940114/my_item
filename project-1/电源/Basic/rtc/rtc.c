
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
1-201708091539 修改了第2次上电设置时间会卡死的问题（在写函数里写入之前调用初始化函数）。


*/


/* 
	//时间读写与设置说明//
1，在mani函数开头放入RTC_Config();就可以使能时钟了。
在RTC_Config();函数中自带判断是不是首次使用RTC
2，使用 RTC_Get();读出时间。读出的数据存放在：
年 ryear （16位）
月 rmon	 （以下都是8位）
日 rday
时 rhour
分 rmin
秒 rsec
周 rweek

3，使用 RTC_Set(4位年,2位月,2位日,2位时,2位分,2位秒); 写入时间。例如：RTC_Get(2017,08,06,21,34,00);

其他函数都是帮助如上3个函数的，不需要调用。 
注意要使用RTC_Get和RTC_Set的返回值，为0时表示读写正确。

*/


#include "sys.h"
#include "rtc.h"


//以下2条全局变量--用于RTC时间的读取
u16 ryear; //4位年
u8 rmon,rday,rhour,rmin,rsec,rweek;//2位月日时分秒周



void RTC_First_Config(void){ //首次启用RTC的设置
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);//启用PWR和BKP的时钟（from APB1）
    PWR_BackupAccessCmd(ENABLE);//后备域解锁
    BKP_DeInit();//备份寄存器模块复位
    RCC_LSEConfig(RCC_LSE_ON);//外部32.768KHZ晶振开启   
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);//等待稳定    
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//RTC时钟源配置成LSE（外部低速晶振32.768KHZ）    
    RCC_RTCCLKCmd(ENABLE);//RTC开启    
    RTC_WaitForSynchro();//开启后需要等待APB1时钟与RTC时钟同步，才能读写寄存器    
    RTC_WaitForLastTask();//读写寄存器前，要确定上一个操作已经结束
    RTC_SetPrescaler(32767);//设置RTC分频器，使RTC时钟为1Hz,RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)   
    RTC_WaitForLastTask();//等待寄存器写入完成	
    //当不使用RTC秒中断，可以屏蔽下面2条
//    RTC_ITConfig(RTC_IT_SEC, ENABLE);//使能秒中断   
//    RTC_WaitForLastTask();//等待写入完成
}

void RTC_Config(void){ //实时时钟初始化
    //在BKP的后备寄存器1中，存了一个特殊字符0xA5A5
    //第一次上电或后备电源掉电后，该寄存器数据丢失，表明RTC数据丢失，需要重新配置
    if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5){//判断寄存数据是否丢失       
        RTC_First_Config();//重新配置RTC        
        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);//配置完成后，向后备寄存器中写特殊字符0xA5A5
    }else{
		//若后备寄存器没有掉电，则无需重新配置RTC
        //这里我们可以利用RCC_GetFlagStatus()函数查看本次复位类型
        if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET){
            //这是上电复位
        }
        else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET){
            //这是外部RST管脚复位
        }       
        RCC_ClearFlag();//清除RCC中复位标志

        //虽然RTC模块不需要重新配置，且掉电后依靠后备电池依然运行
        //但是每次上电后，还是要使能RTCCLK
        RCC_RTCCLKCmd(ENABLE);//使能RTCCLK        
        RTC_WaitForSynchro();//等待RTC时钟与APB1时钟同步

        //当不使用RTC秒中断，可以屏蔽下面2条
//        RTC_ITConfig(RTC_IT_SEC, ENABLE);//使能秒中断        
//        RTC_WaitForLastTask();//等待操作完成
    }
	#ifdef RTCClockOutput_Enable   
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	    PWR_BackupAccessCmd(ENABLE);   
	    BKP_TamperPinCmd(DISABLE);   
	    BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
	#endif
}

void RTC_IRQHandler(void){ //RTC时钟1秒触发中断函数（名称固定不可修改）
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET){

	}
	RTC_ClearITPendingBit(RTC_IT_SEC); 
	RTC_WaitForLastTask();
}

void RTCAlarm_IRQHandler(void){	//闹钟中断处理（启用时必须调高其优先级）
	if(RTC_GetITStatus(RTC_IT_ALR) != RESET){
	
	}
	RTC_ClearITPendingBit(RTC_IT_ALR);
	RTC_WaitForLastTask();
}

//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
u8 Is_Leap_Year(u16 year){                    
	if(year%4==0){ //必须能被4整除
		if(year%100==0){		
			if(year%400==0)return 1;//如果以00结尾,还要能被400整除          
			else return 0;  
		}else return 1;  
	}else return 0;
}                           
//设置时钟
//把输入的时钟转换为秒钟
//以1970年1月1日为基准
//1970~2099年为合法年份

//月份数据表                                                                       
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表  
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};//平年的月份日期表

//写入时间
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec){ //写入当前时间（1970~2099年有效），
	u16 t;
	u32 seccount=0;
	if(syear<2000||syear>2099)return 1;//syear范围1970-2099，此处设置范围为2000-2099       
	for(t=1970;t<syear;t++){ //把所有年份的秒钟相加
		if(Is_Leap_Year(t))seccount+=31622400;//闰年的秒钟数
		else seccount+=31536000;                    //平年的秒钟数
	}
	smon-=1;
	for(t=0;t<smon;t++){         //把前面月份的秒钟数相加
		seccount+=(u32)mon_table[t]*86400;//月份秒钟数相加
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数        
	}
	seccount+=(u32)(sday-1)*86400;//把前面日期的秒钟数相加
	seccount+=(u32)hour*3600;//小时秒钟数
	seccount+=(u32)min*60;      //分钟秒钟数
	seccount+=sec;//最后的秒钟加上去
	RTC_First_Config(); //重新初始化时钟
	BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);//配置完成后，向后备寄存器中写特殊字符0xA5A5
	RTC_SetCounter(seccount);//把换算好的计数器值写入
	RTC_WaitForLastTask(); //等待写入完成
	return 0; //返回值:0,成功;其他:错误代码.    
}

//读出时间
u8 RTC_Get(void){//读出当前时间值 //返回值:0,成功;其他:错误代码.
	static u16 daycnt=0;
	u32 timecount=0;
	u32 temp=0;
	u16 temp1=0;
	timecount=RTC_GetCounter();		
	temp=timecount/86400;   //得到天数(秒钟数对应的)
	if(daycnt!=temp){//超过一天了
		daycnt=temp;
		temp1=1970;  //从1970年开始
		while(temp>=365){
		     if(Is_Leap_Year(temp1)){//是闰年
			     if(temp>=366)temp-=366;//闰年的秒钟数
			     else {temp1++;break;} 
		     }
		     else temp-=365;       //平年
		     temp1++; 
		}  
		ryear=temp1;//得到年份
		temp1=0;
		while(temp>=28){//超过了一个月
			if(Is_Leap_Year(ryear)&&temp1==1){//当年是不是闰年/2月份
				if(temp>=29)temp-=29;//闰年的秒钟数
				else break;
			}else{
	            if(temp>=mon_table[temp1])temp-=mon_table[temp1];//平年
	            else break;
			}
			temp1++; 
		}
		rmon=temp1+1;//得到月份
		rday=temp+1;  //得到日期
	}
	temp=timecount%86400;     //得到秒钟数      
	rhour=temp/3600;     //小时
	rmin=(temp%3600)/60; //分钟     
	rsec=(temp%3600)%60; //秒钟
	rweek=RTC_Get_Week(ryear,rmon,rday);//获取星期  
	return 0;
}    

u8 RTC_Get_Week(u16 year,u8 month,u8 day){ //按年月日计算星期(只允许1901-2099年)//已由RTC_Get调用    
	u16 temp2;
	u8 yearH,yearL;
	yearH=year/100;     
	yearL=year%100;
	// 如果为21世纪,年份数加100 
	if (yearH>19)yearL+=100;
	// 所过闰年数只算1900年之后的 
	temp2=yearL+yearL/4;
	temp2=temp2%7;
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7); //返回星期值
}

/*********************************************************************************************
 * 杜洋工作室 www.DoYoung.net
 * 洋桃电子 www.DoYoung.net/YT 
*********************************************************************************************/




