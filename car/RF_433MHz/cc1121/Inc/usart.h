#ifndef __USART_H
#define __USART_H

#define Burate_SETTING      57600
#include "stm32f0xx_hal.h"

extern unsigned char uart_accept_arry[buffer_size];//存放返回的信息
extern unsigned char uart_accept_end_flag,uart_accept_start_flag,uart_accept_count;//用于存储信息
extern unsigned int uart_time;//用于存储信息
 void USART_Config(void);
 void UART_send_byte(uint8_t byte) ;
 void UART_Send(uint8_t *Buffer);
 void USART1_Rec(void);
#endif


