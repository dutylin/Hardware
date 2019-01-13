#include "stm32l1xx.h"
#include "rtc.h"
extern RTC_HandleTypeDef hrtc;
 unsigned char rtc_wakeup_flag;
/******************************************************************************
函数名：RTC_GetTimeDate
函数功能：读取rtc时间和日期
******************************************************************************/

RTC_TimeDateTypeDef RTC_GetTimeDate()
{
	RTC_TimeTypeDef  RTC_TimeStruct;
	RTC_DateTypeDef  RTC_DateStruct;
	RTC_TimeDateTypeDef  RTC_TimeDateStructure;

	HAL_RTC_GetTime( &hrtc,   &RTC_TimeStruct,RTC_FORMAT_BCD);
	HAL_RTC_GetDate( &hrtc,   &RTC_DateStruct,RTC_FORMAT_BCD);

	RTC_TimeDateStructure.date = RTC_DateStruct.Date;
	RTC_TimeDateStructure.month= RTC_DateStruct.Month;
	RTC_TimeDateStructure.year = RTC_DateStruct.Year;

	RTC_TimeDateStructure.hour =RTC_TimeStruct.Hours;
	RTC_TimeDateStructure.minute = RTC_TimeStruct.Minutes;
	RTC_TimeDateStructure.second  = RTC_TimeStruct.Seconds;	
	return RTC_TimeDateStructure;
}
void SYS_RTCInit(RTC_SetTypeDef RTC_SetStructure)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /**Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
    
  /* USER CODE END Check_RTC_BKUP */

  /**Initialize RTC and set the Time and Date 
  */
  sTime.Hours = RTC_SetStructure.hour;
  sTime.Minutes = RTC_SetStructure.minute;
  sTime.Seconds = RTC_SetStructure.second;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_SetStructure.week ;
  sDate.Month = RTC_SetStructure.month;
  sDate.Date = RTC_SetStructure.date;
  sDate.Year = RTC_SetStructure.year;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /**Enable the WakeUp 
  */
  if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, RTC_SetStructure.wakeup_time, RTC_WAKEUPCLOCK_CK_SPRE_16BITS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

 void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
	rtc_wakeup_flag=1;
}
