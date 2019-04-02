#ifndef BSP_H_
#define BSP_H_
#define buffer_size		128
#define ON		1
#define OFF   0
#include "stm32f1xx_hal.h"
typedef struct
{
uint32_t 	rx_len;
uint32_t 	rx_len_var;
unsigned char 	tx_len;

unsigned char 	receive_flag;
unsigned int      real_index;
unsigned int      loop_index;
unsigned char	usartDMA_rxBuf[buffer_size];
unsigned char	usartDMA_txBuf[buffer_size];

}Uart_Types;


void Pump_Ctrl(unsigned char flag);
void UsartReceive_IDLE(unsigned char uart_num);
void uart_tx(unsigned char num,unsigned char* comm,unsigned short len);
void uart_init(void);
#endif 
