#ifndef UART_Process_H_
#define UART_Process_H_
typedef  struct{
	unsigned int   len;
	unsigned char *dev_sn;//设备的序列号  
	unsigned char  uart_pbuff[10];
}UART_INFO_DEF;

unsigned char Uart_Param(unsigned char *pbuff, UART_INFO_DEF UART_INFO);

#endif
