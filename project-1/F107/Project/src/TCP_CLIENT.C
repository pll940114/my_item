#include "main.h"
#include <string.h>
#include "usart.h"
#include "HX711.h"
#include <string.h>
#include "delay.h"

char c[100][8];
char C[8];
MB member[100];

unsigned char connect_flag = 0;
/*******************************延时函数****************************************/
/*******************************************************************************/

void Delay_s(unsigned long ulVal) /* 利用循环产生一定的延时 */
{
	while ( --ulVal != 0 );
}

/****************************TCP客户端发送数据函数****************************************/
/*****************************************************************************************/
err_t TCP_Client_Send_Data(struct tcp_pcb *cpcb,unsigned char *buff,unsigned int length)
{
	err_t err;
	err = tcp_write(cpcb,buff,length,TCP_WRITE_FLAG_COPY);	//发送数据
	tcp_output(cpcb);
	//tcp_close(cpcb);				//发送完数据关闭连接,根据具体情况选择使用	
	return err;					
}

/*********************************检查连接*******************************************/
/************************************************************************************/
struct tcp_pcb *Check_TCP_Connect(void)
{
	struct tcp_pcb *cpcb = 0;
	connect_flag = 0;
	for(cpcb = tcp_active_pcbs;cpcb != NULL; cpcb = cpcb->next)
	{
		if(cpcb -> state == ESTABLISHED) 
		{
			connect_flag = 1;  						//连接标志
			break;							   	
		}
	}
	if(connect_flag == 0)  							// TCP_LOCAL_PORT指定的端口未连接或已断开
	{
		TCP_Client_Init(TCP_LOCAL_PORT,TCP_SERVER_PORT,TCP_SERVER_IP); //重新连接
		cpcb = 0;
	}
	return cpcb;	
}


/*************这是一个回调函数，当TCP客户端请求的连接建立时被调用*******************/
/***********************************************************************************/
err_t TCP_Connected(void *arg,struct tcp_pcb *pcb,err_t err)
{
	return ERR_OK;
}

/***************tcp客户端接收数据回调函数****这是一个回调函数，当TCP服务器发来数据时调用**************/
/*****************************************************************************************************/
err_t  TCP_Client_Recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{
  static int i=0;
	int j,k,l=0,a;
	struct pbuf *q;
	if(p!= NULL)
	{
		tcp_recved(pcb,p->tot_len);				//获取数据长度 tot_len：tcp数据块的长度
//		tcp_write(pcb,p->payload,p->tot_len,TCP_WRITE_FLAG_COPY);
//		tcp_output(pcb);	
		for(q=p;q!=NULL;q=q->next) 
		{
			if(q->tot_len>100)break;
			{
				for(k=0;k<100;k++)
				{
					if(member[k].name==*((char*) p->payload+6)) //覆盖的情况
					{
						for(a=0;a<6;a++)
						{
						member[k].number[a]=*((char*) p->payload+a);
						}
						member[k].name=*((char*) p->payload+6);
						member[k].number[6]='\0';
						l=1;
						break;
					}
				}
				if(l==0)
				{
					for(k=0;k<100;k++)
					{
						if(member[k].name!=*((char*) p->payload+6)) //没被覆盖的情况
						{                                      
							for(a=0;a<6;a++)
							{
							member[i].number[a]=*((char*) p->payload+a);
							}
							member[i].name=*((char*) p->payload+6);
							member[i].number[6]='\0';
							l=0;
							i++;
							break;
						}
					}
				}	
		}
/***********打印数据验证***********/
			for(j=0;j<=i;j++)
//			USART2_printf("%s %c",member[j].number,member[j].name);
//			USART2_printf("%d",i);
//			USART2_printf("\n");
			if(i==100)i=0;
			/*************************************************************************/
			{
				strncpy(C,member[0].number,6);
				USART4_printf("AT\r\n");            //串口四发送给短信模块
				delay_ms(10);
				USART4_printf("AT+CMGF=1\r\n");
				delay_ms(10);
				USART4_printf("AT+CSCS=\"GSM\"\r\n");
				delay_ms(500);
				USART4_printf("AT+CMGS=\"13559725210\"\r\n");	//15994997557欣锐 13559725210傻屌  13242353288毅营 18988746231Peng
				delay_ms(500);
				USART4_printf("%s",C);		
				delay_ms(10);
				USART4_printf("%c",0x1a);
				delay_ms(10);
			}
			/*************************************************************************/
		}
	}
	else	 										//如果服务器断开连接，则客户端也应断开
	{
		tcp_close(pcb); 						
	}
	pbuf_free(p); 				
	err = ERR_OK;
	return err;	
}

/*************************tcp客户端初始化**********************************************/
/**************************************************************************************/
void TCP_Client_Init(u16_t local_port,u16_t remote_port,unsigned char a,unsigned char b,unsigned char c,unsigned char d)
{

	struct ip_addr ipaddr;
	struct tcp_pcb *tcp_client_pcb;
	err_t err;
	IP4_ADDR(&ipaddr,a,b,c,d);           //服务器IP地址
	tcp_client_pcb = tcp_new(); /* 建立通信的TCP控制块(Clipcb) */
	if (!tcp_client_pcb)
	{
		return ;
	}	
	err = tcp_bind(tcp_client_pcb,IP_ADDR_ANY,local_port); /* 绑定本地IP地址和端口号 ，本地ip地址在LwIP_Init()中已经初始化*/
    if(err != ERR_OK)
	{
		return ;
	}
	tcp_connect(tcp_client_pcb,&ipaddr,remote_port,TCP_Connected);//注册回调函数
	tcp_recv(tcp_client_pcb,TCP_Client_Recv); 				/* 设置tcp接收回调函数 */
}
