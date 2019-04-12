#include "common.h"

unsigned char ASC_To_Valu(unsigned char  asc)
{	
	uint8 valu;
	switch(asc)
	{
		case 0x30:valu=0;break;
		case 0x31:valu=1;break;
		case 0x32:valu=2;break;
		case 0x33:valu=3;break;
		case 0x34:valu=4;break;
		case 0x35:valu=5;break;
		case 0x36:valu=6;break;
		case 0x37:valu=7;break;
		case 0x38:valu=8;break;
		case 0x39:valu=9;break;
	}
	return valu;
}


void u32tostr(unsigned long dat,char *str) 
{
 char  temp[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
 unsigned char  i=0,j=0;
 i=0;
 while(dat)
 {
  temp[i]=dat%10+0x30;
  i++;
  dat/=10;
 }
 j=i;
 for(i=0;i<j;i++)
 {
  str[i]=temp[j-i-1];
 } 
 str[i]='\0';
}

