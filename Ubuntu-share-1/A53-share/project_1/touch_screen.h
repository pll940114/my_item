#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/input.h>



struct in{
	int x;
	int y;
};

//获取X和Y的坐标
int get_xy(struct in *xy)  //falid   -1      sucess   0
{
	//触摸屏文件
	int inputfd=open("/dev/input/event0",O_RDWR);
	if(inputfd==-1)
	{
		printf("open inputfd falid\n");
		return -1;
	}
	
	//定义输入子系统结构体
	struct input_event M;
	while(1)
	{
		//从触摸屏读取一组输入的数据
		read(inputfd,&M,sizeof(struct input_event ));
		//printf("(%d,%d,%d)\n",M.type,M.code,M.value);
		if(M.type==3 && M.code==0)
			xy->x=M.value;
		if(M.type==3 && M.code==1)
			xy->y=M.value;
		if(M.type==1 && M.code==330 &&M.value==1)
			break;
	}
	
	printf("(%d,%d)\n",xy->x,xy->y);
	
	
	close(inputfd);	
	return 0;
}