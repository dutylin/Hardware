#ifndef SYSTICK_TIMER_H_
#define SYSTICK_TIMER_H_
#include "stm32f0xx_hal.h"
#include "stm32f0xx.h"


#define systick_count  3

#define RF_DELAY_INDEX	        0
#define TxRx_Start_INDEX		1 

typedef struct{

unsigned char TXRX_sample_flag;
unsigned char RF_delay_flag;

}Module_Sample_Time;

void delay_ms(uint32_t nms);
void delay_us(uint32_t nus);
void noblock_delay_ms(unsigned char index,uint32_t nms);
uint32_t get_Time_delay(unsigned char index);
void Sample_Time_Interval_Setting(unsigned char index,uint32_t value);
//void Sample_get_Time_delay(unsigned char index);
//void Sample_Time_Interval_Setting(unsigned char index,uint32_t value);
void Sample_Timer_Reload();
Module_Sample_Time Get_Sample_flag_Status(void);
void Reset_Sample_flag(unsigned char index);
void Sample_Timer_Init(void);

#endif
