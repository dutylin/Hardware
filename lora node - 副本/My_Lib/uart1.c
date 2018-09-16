#include "stm8l15x.h"
#include "stm8_eval.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "uart1.h"
#include "Delay.h"
#include "RF_Process.h"
#include "CRC.h"
extern u8   uart1_it_tx_flag;
extern u8  uart_receive_flag;
void u1_printf(char* fmt,...)
{
  u16 i,j;
  va_list ap;
  va_start(ap,fmt);
  vsprintf((char*)USART1_TX_BUF,fmt,ap);
  va_end(ap);
  i=strlen((const char*)USART1_TX_BUF);//此次发送数据的长度
  for(j=0; j<i; j++)                   //循环发送数据
  {
    //while((USART1->SR&0X40)==0);     //循环发送,直到发送完毕
    //USART1->DR=USART1_TX_BUF[j];
    USART_SendData8(EVAL_COM1, (uint8_t)USART1_TX_BUF[j]);
  }
}
void uart1_tx(uint8_t *pbuff,unsigned int len)
{
  unsigned int i;
  for(i=0; i<len; i++)
  {
    while(uart1_it_tx_flag==0)
      ;
    uart1_it_tx_flag=0;
    USART_SendData8(EVAL_COM1, (uint8_t)pbuff[i]);
    //delay_ms(1);
  }
}
u8 uart_deal_cnt;
extern u8 uart_receive_index;
 u8 uart_query_index;

u8 uart1_Process()
{
  u8 res=255,i;
  unsigned int crc_sum;
    if(uart_query_index!=uart_receive_index)
    {
    for(i=0;i<USART1_RX_STA;i++)
    {
               uart_query_index=uart_query_index+1;
		if((USART1_RX_BUF[i]*256+USART1_RX_BUF[i+1])==SER_HEAD)
		{
		  crc_sum=CRC_decode(&USART1_RX_BUF[i+LEN_INDEX],USART1_RX_STA-HEAD_CRC);
		  if(USART1_RX_BUF[i+CRC_INDEX]==(unsigned char)(crc_sum/256)&&USART1_RX_BUF[i+CRC_INDEX+1]==(unsigned char)(crc_sum%256))
                  {
                    USART1_RX_STA=0;
                    res=i;
                  }
		  else
			res=255;
		 // USART1_RX_STA=0;
		  break;
		  
		}
		//if(USART1_RX_STA)
		//USART1_RX_STA=0;
           
	}
	  if(uart_query_index>=USART1_MAX_RECV_LEN)
	  	uart_query_index=0;
	  	
    }

return res;
}

