#ifndef BSP_H_
#define BSP_H_
#include "LTE_BSP.h"
//#define debug_mode 0
typedef struct
{
unsigned char 	gas[4];
unsigned char 	battery[4];
}ADC_Types;

typedef struct
{
uint32_t 	rx_len;
uint32_t 	rx_len_var;
unsigned char 	receive_flag;
unsigned int      real_index;
unsigned int      loop_index;
unsigned char	usartDMA_rxBuf[buffer_size];

}Uart_Types;
typedef struct
{
uint32_t 	rx_len;
uint32_t 	rx_len_var;
unsigned char 	receive_flag;
unsigned int      real_index;
unsigned int      loop_index;
unsigned char	 usartDMA_rxBuf[buffer_size2];

}Uart_Types_2;


void HW_init(void);
void UsartReceive_IDLE(unsigned char uart_num);
void uart_init(void);
ADC_Types Get_Battery_Gas_adc(unsigned char count);
unsigned char PWR_KEY_Dectection(void);
void uart_dma_resume(unsigned char uart_num);

#endif 
