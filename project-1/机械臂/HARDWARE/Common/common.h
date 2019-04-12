#ifndef  _COMMON_H_
#define  _COMMON_H_
#define uint8  unsigned char
#define uint32    unsigned int   
#define uint16    unsigned short int  
#define CMD_FLASH_ADDR 32000UL
#define FILE_FLASH_ADDR 32016UL
#define PS2_FLASH_ADDR 32240UL

#define WRITE_SIZE 256
#define WRITE_BIT_DEPTH 8
#define MOTOR_NUM 9
#define SEND_A 0x01
#define SEND_DOWN_OK 0x02
#define SEND_DOWN_ALL_OK 0x04
#define SEND_CC 0x08
#define SEND_START_OK 0x10
#define SEND_READ_FILE 0x20
#define SEND_SEND_FILE 0x40
#define SEND_SET_OFFLINE_OK 0x80
#define SEND_SET_DISABLEOFFLINE_OK 0x100
#define SEND_SET_ONLINE_OK 0x200
#define SEND_SET_DELETE_ONE_FILE_OK 0x400
#define SEND_SET_DELETE_ALL_FILE_OK 0x800
#define SEND_SET_PS2_OK 0x1000
#define SEND_SET_READ_UART_MOTOR_ANGLE 0x2000
#define SEND_SET_SET_UART_MOTOR_PULK 0x4000
#define SEND_SET_SET_UART_MOTOR_ANGLE 0x8000
#define SEND_SET_BEEP_ON 0x10000
#define SEND_SET_BEEP_OFF 0x20000
#define ERROR_FLASH_FULL 0x01
#define ERROR_FLASH_FILE_FULL 0x02 //写入的文件数字大于 MAX_SUPPORT_FILE_SAVE
#define ERROR_FLASH_WRITE 0x04
#define ERROR_FLASH_WRITE1 0x08
#define PS_SUPPORT 1
#define MAX_SUPPORT_FILE_SAVE 32//修改值的时候注意必须是16的整数倍
//#define PS2_FLASH_ADDR (FILE_FLASH_ADDR + MAX_SUPPORT_FILE_SAVE )//这个值不能超过最大32768
#define DEBUG 0

void SendUartState(void);
void ReadMoterInfor(void);
void WriteMoterInfor(void);
void GetOneMotorCMD(void);
//void scan_ps2();
void ReadOneCmdInfor(unsigned int addr);
void WriteOneCmdInfor(unsigned int addr);
void u32tostr(unsigned long dat,char *str);

typedef struct Motor
{
	uint8 duoji_count;
	uint8 file_flag[MAX_SUPPORT_FILE_SAVE];//最多支持50个文件
	unsigned long sum;//最大的存储舵机一行命令
	unsigned long CRC1;
	unsigned char filecount;
	uint8 stop_tuoji_flag;
	uint8 beep_mode;
}MotorData;
typedef struct Motor_One_Cmd
{
	unsigned char cur_file_num;
	unsigned char name[40];
	uint16 tuoji_count;//脱机执行循环次数
	unsigned int start;
	unsigned int end;
	
}MotorOneCmd;
typedef struct Cur_Item
{
	unsigned int cur_num;
	unsigned int file_num;
	unsigned char tuoji_count;
	unsigned char delete_num;
	unsigned char read_num;
	unsigned char pulk_num;
	unsigned char angle_mode;
}CurrentItem;
unsigned char ASC_To_Valu(unsigned char  asc);
#endif
