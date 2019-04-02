#ifndef __OTHER_H
#define __OTHER_H
#ifdef __cplusplus
extern "C" {
#endif
#include "stm32l1xx.h"


void NVIC_Config(void);

void Power_Control_GPIO(void);
void Power_Control(char state);
void Led_Control_GPIO(void);
void LED_Control(char state);
void Timer_Config(uint16_t per);

void Storage_Data(void);
void Led_Display_Data(uint8_t channel,uint16_t data);
void Led_Display_XX(uint8_t sensor_id);
void Led_Display_Set(uint8_t i,uint8_t data);
void LED_Display_Clear(void);
#ifdef __cplusplus
}
#endif
#endif

/*******************************************************************************/
