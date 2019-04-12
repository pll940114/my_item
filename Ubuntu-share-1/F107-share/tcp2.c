#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#define random_1(a,b) ((rand()%(b-a))+a)
#define random_2(a,b) ((rand()%(b-a+1))+a)
 
struct n_l
{
	int n;
	int l;
};

int search_SQL(int fd,char Key)
{
	struct n_l p;
	int i=0;
	unsigned char buff[10]={0};
	read(fd,buff,10);
	while(1)
	{
		if(buff[i]=='!')
		{
			buff[i]='\0';
			break;
		}
		i++;	
	}
	p.n=buff[0]-'0';
	p.l=buff[i-1]-'0';
	//printf("%d %d\n",p.n,p.l);
	
	char buf[20]={0};
	for(i=0;i<p.n;i++)
	{
		lseek(fd,6+i*p.l,SEEK_SET);
		read(fd,buf,1);
		if(buf[0]==Key)
		{
			printf("%c:sucess!\n",buf[0]);
			return 0;
		}
	}
	printf("%c:failure!\n",buf[0]);
	return 1;
}

int SQL(char*p)
{   
	char a[10]="\r\n";
	int fd=open("SQL.txt",O_RDWR);
	if(fd==-1)
	{
		printf("open falid\n");
		return -1;
	}
	printf("open succes\n");
	int bol=search_SQL(fd,p[0]);
	close(fd);
	return bol;
}

int get_code( char* str )
{
    int i=RAND_MAX,nm;
    srand((int)time(NULL));
	nm=random_1(100000,999999);
	printf( "%d\n",nm);
    sprintf(str, "%d", nm);
    printf("%s\n",str);
    return 0;
}

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
	char str[20];
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock < 0) {
        printf("sock() error\n");
        exit(1);
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    int optval = 1;
    // setting address and port reusable
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("bind() error\n");
        exit(1);
    }
    if (listen(serv_sock, 5) < 0) {
        printf("listen() error\n");
        exit(1);
    }
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
    if (clnt_sock < 0) {
        printf("accept() error\n");
        exit(1);
    }
    char buf[1024];
    printf("connected with ip: %s and port: %d\n",
           inet_ntop(AF_INET, &clnt_addr.sin_addr, buf, 1024),
           ntohs(clnt_addr.sin_port));
		   
    while (1) {
        memset(buf, 0, sizeof(buf));
        ssize_t size = read(clnt_sock, buf, sizeof(buf) - 1);
        if (size < 0) {
            printf("read() error\n");
            break;
        } else {
            //printf("client#: %s\n", buf);
			int bol=SQL(buf);
			if(bol==0)
			{
				get_code(str);
				str[6]=buf[0];
				str[7]='\0';
				write(clnt_sock, str, strlen(str));
			}
			else if(bol==1);
        }	
    }
    close(clnt_sock);
    close(serv_sock);
    return 0;
}