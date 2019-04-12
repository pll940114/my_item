

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


#include "sys.h"
#include "usart.h"
#include "rs485.h"

void RS485_Init(void){ //RS485接口初始化
	GPIO_InitTypeDef  GPIO_InitStructure; 	
    GPIO_InitStructure.GPIO_Pin = RS485_RE; //选择端口号（0~15或all）                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //选择IO接口工作方式       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）    
	GPIO_Init(RS485PORT, &GPIO_InitStructure);
	GPIO_ResetBits(RS485PORT,RS485_RE); //RE端控制接收/发送状态，RE为1时发送，为0时接收。
	
}
 
/*
RS485总线通信，使用USART3，这是RS485专用的printf函数
调用方法：RS485_printf("123"); //向USART3发送字符123
*/
void RS485_printf (char *fmt, ...){ 
	char buffer[USART3_REC_LEN+1];  // 数据长度
	u8 i = 0;
	va_list arg_ptr;
	GPIO_SetBits(RS485PORT,RS485_RE); //为高电平（发送）//RS485收发选择线	
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, USART3_REC_LEN+1, fmt, arg_ptr);
	while ((i < USART3_REC_LEN) && (i < strlen(buffer))){
        USART_SendData(USART3, (u8) buffer[i++]);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); 
	}
	va_end(arg_ptr);
	GPIO_ResetBits(RS485PORT,RS485_RE); //为低电平（接收）//RS485收发选择线	
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
