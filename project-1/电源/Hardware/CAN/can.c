

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




#include "can.h"



u8 CAN1_Configuration(void){ //CAN初始化（返回0表示设置成功，返回其他表示失败）
    GPIO_InitTypeDef        GPIO_InitStructure; 
    CAN_InitTypeDef         CAN_InitStructure;
    CAN_FilterInitTypeDef   CAN_FilterInitStructure;

#if CAN_INT_ENABLE
    NVIC_InitTypeDef        NVIC_InitStructure;	 
#endif
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PORTA时钟                                                                
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);    //使能CAN1时钟  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化IO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化IO
    //CAN单元设置
    CAN_InitStructure.CAN_TTCM=DISABLE;         //非时间触发通信模式  
    CAN_InitStructure.CAN_ABOM=DISABLE;         //软件自动离线管理   
    CAN_InitStructure.CAN_AWUM=DISABLE;         //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
    CAN_InitStructure.CAN_NART=ENABLE;          //禁止报文自动传送 
    CAN_InitStructure.CAN_RFLM=DISABLE;         //报文不锁定,新的覆盖旧的  
    CAN_InitStructure.CAN_TXFP=DISABLE;         //优先级由报文标识符决定 
    CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;  //模式设置：CAN_Mode_Normal 普通模式，CAN_Mode_LoopBack 回环模式; 
    //设置波特率
    CAN_InitStructure.CAN_SJW=tsjw;             //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位  CAN_SJW_1tq    CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
    CAN_InitStructure.CAN_BS1=tbs1;             //Tbs1=tbs1+1个时间单位CAN_BS1_1tq ~ CAN_BS1_16tq
    CAN_InitStructure.CAN_BS2=tbs2;             //Tbs2=tbs2+1个时间单位CAN_BS2_1tq ~ CAN_BS2_8tq
    CAN_InitStructure.CAN_Prescaler=brp;        //分频系数(Fdiv)为brp+1  
    CAN_Init(CAN1, &CAN_InitStructure);         //初始化CAN1 
	//设置过滤器
    CAN_FilterInitStructure.CAN_FilterNumber=0; //过滤器0
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;   //屏蔽位模式
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;  //32位宽 
    CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;    //32位ID
    CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//激活过滤器0
    CAN_FilterInit(&CAN_FilterInitStructure);           //滤波器初始化

#if CAN_INT_ENABLE 	//以下是用于CAN中断方式接收的设置
    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);              //FIFO0消息挂号中断允许.            
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif
    return 0;		
}
 
//CAN发送一组数据(固定格式:ID为0X12,标准帧,数据帧)   
//msg:数据指针,最大为8个字节，len:数据长度(最大为8) 
//返回值:0,成功; 其他,失败;
u8 CAN_Send_Msg(u8* msg,u8 len){   
    u8 mbox;
    u16 i=0;
    CanTxMsg TxMessage;
    TxMessage.StdId=0x12;           // 标准标识符 
    TxMessage.ExtId=0x00;           // 设置扩展标识符
    TxMessage.IDE=CAN_Id_Standard;  // 标准帧
    TxMessage.RTR=CAN_RTR_Data;     // 数据帧
    TxMessage.DLC=len;              // 要发送的数据长度
    for(i=0;i<len;i++)
    TxMessage.Data[i]=msg[i];       //写入数据              
    mbox= CAN_Transmit(CAN1,&TxMessage);   
    i=0; 
    while((CAN_TransmitStatus(CAN1,mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++; //等待发送结束
    if(i>=0XFFF)return 1;
    return 0;    
}

//can口接收数据查询
//buf:数据缓存区;     
//返回值:0,无数据被收到，其他,接收的数据长度;
u8 CAN_Receive_Msg(u8 *buf){                  
    u32 i;
    CanRxMsg RxMessage;
    if(CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;//没有接收到数据,直接退出 
    CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);//读取数据 
    for(i=0;i<8;i++) //把8个数据放入参数数组
    buf[i]=RxMessage.Data[i];  
    return RxMessage.DLC;  //返回数据数量 
}

//CAN的中断接收程序（中断处理程序）	
//必须在can.h文件里CAN_INT_ENABLE为1才能使用中断
//数据处理尽量在中断函数内完成，外部处理要在处理前关CAN中断，防止数据覆盖
void USB_LP_CAN1_RX0_IRQHandler(void){
    CanRxMsg RxMessage;
	vu8 CAN_ReceiveBuff[8]; //CAN总线中断接受的数据寄存器
    vu8 i = 0;
    vu8 u8_RxLen = 0;
  	CAN_ReceiveBuff[0] = 0;	//清空寄存器
    RxMessage.StdId = 0x00;
    RxMessage.ExtId = 0x00;
    RxMessage.IDE = 0;
    RxMessage.RTR = 0;
    RxMessage.DLC = 0;
    RxMessage.FMI = 0;
    for(i=0;i<8;i++){
        RxMessage.Data[i]=0x00; 
    } 
    CAN_Receive(CAN1,CAN_FIFO0,&RxMessage); //读出FIFO0数据
    u8_RxLen = RxMessage.DLC; //读出数据数量
    if(RxMessage.StdId==0x12){//判断ID是否一致  
		CAN_ReceiveBuff[0] = RxMessage.DLC; //将收到数据数量放到数组0的位置
        for( i=0;i<u8_RxLen; i++){ //将收到的数据存入CAN寄存器
            CAN_ReceiveBuff[i] = RxMessage.Data[i]; //将8位数据存入CAN接收寄存器
        }
    }
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
