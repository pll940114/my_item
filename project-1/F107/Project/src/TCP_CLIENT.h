#ifndef  _TCP_CLIENT_H_
#define  _TCP_CLIENT_H_
#include "lwip/tcp.h"

/***************开发板ip及MAC定义*************************/
#define BOARD_IP  			 192,168,1,128   		//开发板ip 
#define BOARD_NETMASK   	 255,255,255,0   		//开发板子网掩码
#define BOARD_WG		   	 192,168,1,1  			//开发板子网关
#define BOARD_MAC_ADDR       0,0,0,3,2,1			//开发板MAC地址
#define TCP_LOCAL_PORT     		8088  //开发板端口
#define TCP_SERVER_PORT    		8086  //电脑端口
#define TCP_SERVER_IP   192,168,1,192//设置成自己电脑真实的IP地址

extern struct tcp_pcb *tcp_client_pcb;

void TCP_Client_Init(u16_t local_port,u16_t remote_port,unsigned char a,unsigned char b,unsigned char c,unsigned char d);
err_t TCP_Client_Send_Data(struct tcp_pcb *cpcb,unsigned char *buff,unsigned int length);
struct tcp_pcb *Check_TCP_Connect(void);
#endif

