#include <assert.h>
#include <fcntl.h> 
#include <unistd.h>
#include <termios.h> 
#include <sys/types.h>
#include <sys/select.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>


static struct timeval timeout;

#define DEV_PATH1   "/dev/ttySAC1"
#define DEV_PATH2   "/dev/ttySAC2"
#define DEV_PATH3   "/dev/ttySAC3"



/* 设置窗口参数:9600速率 */
void init_tty(int fd)
{    
	//声明设置串口的结构体
	struct termios termios_new;
	//先清空该结构体
	bzero( &termios_new, sizeof(termios_new));

	cfmakeraw(&termios_new);

	//设置波特率
	cfsetispeed(&termios_new, B115200);
	cfsetospeed(&termios_new, B115200);

	// CLOCAL和CREAD分别用于本地连接和接受使能
	// 首先要通过位掩码的方式激活这两个选项。    
	termios_new.c_cflag |= CLOCAL | CREAD;

	//通过掩码设置数据位为8位
	termios_new.c_cflag &= ~CSIZE;
	termios_new.c_cflag |= CS8; 

	//设置无奇偶校验
	termios_new.c_cflag &= ~PARENB;

	//一位停止位
	termios_new.c_cflag &= ~CSTOPB;

	// 可设置接收字符和等待时间，无特殊要求可以将其设置为0
	termios_new.c_cc[VTIME] = 0;
	termios_new.c_cc[VMIN] = 1;

	// 用于清空输入/输出缓冲区
	tcflush (fd, TCIFLUSH);

	//完成配置后，可以使用以下函数激活串口设置
	if(tcsetattr(fd, TCSANOW, &termios_new))
		printf("Setting the serial1 failed!\n");

}

void usart_send (char*send_buff) //参数字符串
{
	// 初始化串口1
	int fd = open(DEV_PATH2, O_RDWR | O_NOCTTY);
	init_tty(fd);
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	write(fd, send_buff, strlen(send_buff));
	close(fd);
	
}


/* char *usart_rev (void)
{
	char* rev_buff;
	int fd = open(DEV_PATH1, O_RDWR | O_NOCTTY);
	init_tty(fd);
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	read(fd, sent_buff, strlen(sent_buff));
} */