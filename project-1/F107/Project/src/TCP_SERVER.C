/***********************************************************************
文件名称：TCP_SERVER.C
功    能：完成TCP的数据收发
编写时间：2013.4.25
编 写 人：赵
注    意：
***********************************************************************/
#include "stm32_eth.h"
#include "lwip/tcp.h"
#include "TCP_SERVER.h"

/***********************************************************************
函数名称：tcp_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
功    能：TCP数据接收和发送
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：这是一个回调函数，当一个TCP段到达这个连接时会被调用
***********************************************************************/
static err_t tcp_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{
	char *data;
	char *data_temp;

	if(p != NULL)
	{
		tcp_recved(pcb, p->tot_len);				//获取数据长度 tot_len：tcp数据块的长度
		/******将数据原样返回*******************/
		tcp_write(pcb,p->payload,p->tot_len,0); 	// payload为TCP数据块的起始位置       
		pbuf_free(p); 											/* 释放该TCP段 */
	}
	else
	{
		tcp_close(pcb); 											/* 作为TCP服务器不应主动关闭这个连接？ */
	}
	err = ERR_OK;
	return err;
}


/***********************************************************************
函数名称：tcp_server_accept(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
功    能：回调函数
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：这是一个回调函数，当一个连接已经接受时会被调用
***********************************************************************/
static err_t tcp_server_accept(void *arg,struct tcp_pcb *pcb,err_t err)
{
	tcp_setprio(pcb, TCP_PRIO_MIN); 		/* 设置回调函数优先级，当存在几个连接时特别重要,此函数必须调用*/
	tcp_recv(pcb,tcp_server_recv); 				/* 设置TCP段到时的回调函数 */
	err = ERR_OK;
	return err;
}


/***********************************************************************
函数名称：TCP_server_init(void)
功    能：完成TCP服务器的初始化，主要是使得TCP通讯快进入监听状态
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/
void TCP_server_init(void)
{
	struct tcp_pcb *pcb;

	/*****************************************************/
	pcb = tcp_new(); 								/* 建立通信的TCP控制块(pcb) */
	tcp_bind(pcb,IP_ADDR_ANY,TCP_LOCAL_PORT); 	    /* 绑定本地IP地址和端口号（作为tcp服务器） */
	pcb = tcp_listen(pcb); 							/* 进入监听状态 */
	tcp_accept(pcb,tcp_server_accept); 			    /* 设置有连接请求时的回调函数 */

}