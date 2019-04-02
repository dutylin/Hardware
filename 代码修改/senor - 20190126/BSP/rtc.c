#include "stm32l1xx.h"
#include "rtc.h"
#include "oled.h"
RTC_InitTypeDef   RTC_InitStructure;;
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
 
	RTC_GetTime(RTC_Format_BCD,  &RTC_TimeStruct);
	RTC_GetDate(RTC_Format_BCD,  &RTC_DateStruct);	


	RTC_TimeDateStructure.date = RTC_DateStruct.RTC_Date;
	RTC_TimeDateStructure.month= RTC_DateStruct.RTC_Month;
	RTC_TimeDateStructure.year = RTC_DateStruct.RTC_Year;

	RTC_TimeDateStructure.hour =RTC_TimeStruct.RTC_Hours;
	RTC_TimeDateStructure.minute = RTC_TimeStruct.RTC_Minutes;
	RTC_TimeDateStructure.second  = RTC_TimeStruct.RTC_Seconds;	
	return RTC_TimeDateStructure;
}
void RTC_Config(RTC_SetTypeDef RTC_SetStructure)
{
	
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  NVIC_InitTypeDef  NVIC_InitStructure;
  EXTI_InitTypeDef  EXTI_InitStructure;	
  /* USER CODE BEGIN RTC_Init 0 */
  /* Enable the PWR clock */   
RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /*!< Allow access to RTC */
  PWR_RTCAccessCmd(ENABLE);

  /*!< Reset RTC Domain */
  RCC_RTCResetCmd(ENABLE);
  RCC_RTCResetCmd(DISABLE);

  /*!< LSE Enable */
  RCC_LSICmd(ENABLE);   
    
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)   
  {        
	;  //初始化时钟失败,晶振有问题，32768晶振起振极限时间1-5S     
  }   
  /* Select the RTC Clock Source */    
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);   
  /* Enable the RTC Clock */    
  RCC_RTCCLKCmd(ENABLE);   
  /* Wait for RTC APB registers synchronisation */    
  RTC_WaitForSynchro();   
  /* Configure the RTC data register and RTC prescaler */    

  /* USER CODE END RTC_Init 0 */



  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /**Initialize RTC Only 
  */
  RTC_InitStructure.RTC_AsynchPrediv = 127;
  RTC_InitStructure.RTC_SynchPrediv	=  255;
  RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
  RTC_Init(&RTC_InitStructure);
  

  /* USER CODE BEGIN Check_RTC_BKUP */
    
  /* USER CODE END Check_RTC_BKUP */

  /**Initialize RTC and set the Time and Date 
  */
  sTime.RTC_Hours= RTC_SetStructure.hour;
  sTime.RTC_Minutes= RTC_SetStructure.minute;
  sTime.RTC_Seconds= RTC_SetStructure.second;
  sTime.RTC_H12=RTC_SetStructure.am_pm;
  
  RTC_SetTime(RTC_Format_BCD, &sTime);
  
  sDate.RTC_WeekDay= RTC_SetStructure.week ;
  sDate.RTC_Month= RTC_SetStructure.month;
  sDate.RTC_Date= RTC_SetStructure.date;
  sDate.RTC_Year= RTC_SetStructure.year;
  RTC_SetDate(RTC_Format_BCD, &sDate);
 /* EXTI configuration *******************************************************/
  EXTI_ClearITPendingBit(EXTI_Line20);
  EXTI_InitStructure.EXTI_Line = EXTI_Line20;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable the RTC Wakeup Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 
  /* RTC Wakeup Interrupt Generation: Clock Source: RTCDiv_16, Wakeup Time Base: 4s */
  RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
  RTC_SetWakeUpCounter(RTC_SetStructure.wakeup_time);

  /* Enable the Wakeup Interrupt */
  RTC_ITConfig(RTC_IT_WUT, ENABLE); 
}



void RTC_WKUP_IRQHandler(void)
{
 
	
	
    if(RTC_GetITStatus(RTC_IT_WUT) != RESET) 
    {
			  rtc_wakeup_flag=1;

	       PWR_ClearFlag(PWR_FLAG_WU);		  
        EXTI_ClearITPendingBit(EXTI_Line20); 
      //  PWR_RTCAccessCmd(ENABLE);//如果之前禁止了，要重新打开
       // RTC_ClearITPendingBit(RTC_IT_WUT);
        //PWR_RTCAccessCmd(DISABLE);//可以再关上
        RTC_ClearITPendingBit(RTC_IT_WUT);   //清除中断标志
    }

}
void IWDG_INIT()
{
	//NVIC_InitTypeDef   NVIC_InitStructure;
	// TIM_ICInitTypeDef	TIM_ICInitStructure;
	 RCC_ClocksTypeDef	RCC_ClockFreq;
	
	 /* Enable the LSI oscillator ************************************************/
	 RCC_LSICmd(ENABLE);
	 
	 /* Wait till LSI is ready */
	 while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	 {}
	
	 /* TIM10 configuration *******************************************************/ 
	 /* Enable TIM10 clocks */
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	 
	 /* Connect internally the TIM10_CH1 Input Capture to the LSI clock output */
	// TIM_RemapConfig(TIM10, TIM10_LSI);
	
	 /* Configure TIM10 presclaer */
	// TIM_PrescalerConfig(TIM10, 0, TIM_PSCReloadMode_Immediate);
	 
	 /* TIM10 configuration: Input Capture mode ---------------------
		The LSI oscillator is connected to TIM10 CH1
		The Rising edge is used as active edge,
		The TIM10 CCR1 is used to compute the frequency value 
	 ------------------------------------------------------------ */
		
	 /* Compute the LSI frequency, depending on TIM10 input clock frequency (PCLK1)*/
	 /* Get SYSCLK, HCLK and PCLKx frequency */
	 RCC_GetClocksFreq(&RCC_ClockFreq);
	
	 /* Get PCLK1 prescaler */
//	 if ((RCC->CFGR & RCC_CFGR_PPRE1) == 0)
//	 { 
//	   /* PCLK1 prescaler equal to 1 => TIMCLK = PCLK1 */
//	   return ((RCC_ClockFreq.PCLK1_Frequency / PeriodValue) * 8);
//	 }
//	 else
//	 { /* PCLK1 prescaler different from 1 => TIMCLK = 2 * PCLK1 */
//	   return (((2 * RCC_ClockFreq.PCLK1_Frequency) / PeriodValue) * 8) ;
//	 }


	 /* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
		 dispersion) */
	  /* Enable write access to IWDG_PR and IWDG_RLR registers */
	  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	 
	  /* IWDG counter clock: LSI/32 */
	  IWDG_SetPrescaler(IWDG_Prescaler_32);
	 
	  /* Set counter reload value to obtain 250ms IWDG TimeOut.
		 Counter Reload Value = 250ms/IWDG counter clock period
							  = 250ms / (LSI/32)
							  = 0.25s / (LsiFreq/32)
							  = LsiFreq/(32 * 4)
							  = LsiFreq/128
	   */
	  IWDG_SetReload(0xfff);
	 
	  /* Reload IWDG counter */
	  IWDG_ReloadCounter();
	 
	  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	  IWDG_Enable();

}

