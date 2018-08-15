#ifndef SYSTICK_TIMER_H_
#define SYSTICK_TIMER_H_
#include "stm32f4xx_hal.h"

#define systick_count  7

#define GPS_INTERVAL_INDEX		 0
#define TXRX_INDEX			         1
#define RF_INTERVAL_INDEX		 2 
#define LTE_DELAY_INDEX		         3
#define LTE_Heart_INDEX		         4 
#define LTE_Power_INDEX		         5 
#define LTE_Battery_INDEX		     6 

typedef struct{
unsigned char GPS_sample_flag;
unsigned char TXRX_sample_flag;
unsigned char RF_sample_flag;
unsigned char LTE_delay_flag;
unsigned char LTE_heart_flag;
unsigned char LTE_Power_flag;
unsigned char LTE_Battery_flag;

}Module_Sample_Time;

void delay_ms(uint32_t nms);
void delay_us(uint32_t nus);
void noblock_delay_ms(unsigned char index,uint32_t nms);
uint32_t get_Time_delay(unsigned char index);
void Sample_Time_Interval_Setting(unsigned char index,uint32_t value);
//void Sample_get_Time_delay(unsigned char index);
//void Sample_Time_Interval_Setting(unsigned char index,uint32_t value);
void Sample_Timer_Reload(void);
Module_Sample_Time Get_Sample_flag_Status(void);
void Reset_Sample_flag(unsigned char index);
void Sample_Timer_Init(void);

#endif
