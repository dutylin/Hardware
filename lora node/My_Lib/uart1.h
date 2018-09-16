#ifndef __USART1_H
#define __USART1_H	
#include "stm8l15x.h"
#include "stm8_eval.h"

//#include "sys.h"  
#include "stdio.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.csom
//修改日期:2016/6/17
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.0修改说明 
////////////////////////////////////////////////////////////////////////////////// 	
#define u8 unsigned char
#define u32 uint32_t
#define USART1_MAX_RECV_LEN		50					//最大接收缓存字节数
#define USART1_MAX_SEND_LEN		50					//最大发送缓存字节数
#define USART1_RX_EN 			1					//0,不接收;1,接收.

extern u8  USART1_RX_BUF[USART1_MAX_RECV_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
extern u8  USART1_TX_BUF[USART1_MAX_SEND_LEN]; 		//发送缓冲,最大USART3_MAX_SEND_LEN字节
extern u16 USART1_RX_STA;   						//接收数据状态

//extern UART_HandleTypeDef UART1_Handler;//UART句柄


void usart1_set(u8 bps,u8 parity);
void usart1_init(u32 bound);				//串口3初始化 
void u1_printf(char* fmt, ...);
void usart1_set(u8 bps,u8 parity);    
void usart1_rx(u8 enable);
void uart1_tx(uint8_t *pbuff,unsigned int len);
u8 uart1_Process(void);
#endif

