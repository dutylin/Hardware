#ifndef RTC_H
#define RTC_H
struct 
{
	uint8_t 
}
#endif 
{
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t date;
	uint8_t month;
	uint8_t week;
	uint8_t year;
	uint8_t am_pm;
	uint8_t 
}
#endif 
	uint8_t date_format;
}RTC_SetTypeDef;
void SYS_RTCInit(RTC_SetTypeDef RTC_SetStructure);

#endivovoid RTC_SetTimeDate(RTC_Sevoid RTC_SetTimeDate(RTC_SetTypeDef RTC_SetStructure);
void RTC_ClkConfig(void);void SYS_RTCInit(RTC_SetTypeDef RTC_SetStructure);
void RTC_WakeUp_init(void);
void RTC_SetTimeDate(RTC_SetTypeDef RTC_SetStructure);
void RTC_ClkConfig(void);
void RTC_GetTimeDate(RTC_TimeDateTypevoid RTC_WakeUp_init(uint16_t wakeup_time);

#endif 

