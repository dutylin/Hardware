#ifndef RTC_H
#define RTC_H
#include "stm32l1xx.h"
typedef struct 
{
	uint32_t wakeup_time;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t date;
	uint8_t month;
	uint8_t week;
	uint8_t year;
	uint8_t am_pm;
	uint8_t date_format;
}RTC_SetTypeDef;
typedef struct 
{

	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t date;
	uint8_t month;
	uint8_t year;
	uint8_t week;	
}RTC_TimeDateTypeDef;

void RTC_ClkConfig(void);
void RTC_Config(RTC_SetTypeDef RTC_SetStructure);
void RTC_WakeUp_IRQInit(void);
void RTC_SetTimeDate(RTC_SetTypeDef RTC_SetStructure);
void RTC_ClkConfig(void);
RTC_TimeDateTypeDef RTC_GetTimeDate(void);
void RTC_WakeUp_init(uint16_t wakeup_time);
void SYS_RTCInit2(void);
void RtcWakeUpConfig(void);
void RTC_IRQConfig(void);
void IWDG_INIT(void);
#endif 

