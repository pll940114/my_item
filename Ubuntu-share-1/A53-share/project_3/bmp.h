#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

//从屏幕（W，H）坐标的位置显示一张图片，图片名是str的数值

int show_bmp(char *str,int W,int H) //falid——return -1  sucess——return 0 
{
	//打开bmp图片
	int bmpfd=open(str,O_RDWR);
	if(bmpfd==-1)
	{
		//printf("open picture.bmp falid\n");
		return -1;
	}
	
	//读取头子节数据
	char headbuf[54];
	read(bmpfd,headbuf,54);
	
	//获取图片宽度和高度
	int with=headbuf[18]|headbuf[19]<<8|headbuf[20]<<16|headbuf[21]<<24;
	int high=headbuf[22]|headbuf[23]<<8|headbuf[24]<<16|headbuf[25]<<24;
	//printf("with=%d  high=%d\n",with,high);
	
	//需要处理的像素点个数确定为 宽*高的实际值
	char RGBbuf[with*high*3];
	read(bmpfd,RGBbuf,with*high*3);
	//字节对齐
	int bmpbuf[with*high];
	int i;
	for(i=0;i<with*high;i++)
		bmpbuf[i]=RGBbuf[3*i] |RGBbuf[3*i+1] <<8|RGBbuf[3*i+2] <<16;
		//RGB32 		蓝色		绿色			红色
	
	//打开lcd屏幕文件	/dev/fb0
	int fd=open("/dev/fb0",O_RDWR);
	if(fd==-1)
	{
		printf("open falid\n");
		return -1;
	}
	//printf("open succes\n");
	
	//内存映射---这里的800*480是LCD屏幕，与BMP图片无关
	int *lcdmem=mmap(NULL,	800*480*4,PROT_READ |PROT_WRITE,	
					MAP_SHARED, fd,0);
	if(lcdmem==MAP_FAILED)					
	{
		printf("mmap fialed\n");
		return -1;
	}
	//H+y<480 和 W+x<800  是LCD屏幕限制
	// y<high 和 x<with   是bmp图片限制
	int x,y;
	for(y=0;H+y<480&&y<high;y++)
		for(x=0;W+x<800&&x<with;x++)
			*(lcdmem+(W+x)+800*(H+y))=bmpbuf[x+(high-1-y)*with];
											//x+(479-y)*800
	//关闭文件
	close(fd);
	close(bmpfd);
	return 0;
}