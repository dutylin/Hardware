#include "stm8l15x.h"
#include "stm8_eval.h"
#include "uart1.h"
#include "Delay.h"
#include "lora.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"

extern u8  USART1_RX_BUF[USART1_MAX_RECV_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
extern u8  USART1_TX_BUF[USART1_MAX_SEND_LEN]; 		//发送缓冲,最大USART3_MAX_SEND_LEN字节
extern u16 USART1_RX_STA;
//lora发送命令
//cmd:发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 lora_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0;
	USART1_RX_STA=0;
	if((u32)*cmd<=0XFF)
	{
		//while((USART3->SR&0X40)==0);//等待上一次数据发送完成
		//USART3->DR=(u32)cmd;
		USART_SendData8(EVAL_COM1, (uint8_t)*cmd);


	}
	else u1_printf("%s\r\n",cmd); //发送命令

	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART1_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(lora_check_cmd(ack))
				{
					USART1_RX_STA=0;
					break;//得到有效数据
				}
			}
		}
		if(waittime==0)res=1;
	}
	return res;
}
u8* lora_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART1_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART1_RX_BUF[USART1_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART1_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}

unsigned char  lora_init()
{
	u8 retry;
	u8 temp=1;
	retry=3;
	while(retry--)
	{
		if(!lora_send_cmd("AT","OK",70))
		{
			temp=0;//检测成功
			break;
		}
	}
	if(retry==0) temp=1;//检测失败
	return temp;
}


