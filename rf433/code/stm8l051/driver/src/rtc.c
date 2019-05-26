#include "stm8l15x.h"
#include "rtc.h"
void RTC_Config(uint16_t time) 
{
  RTC_DeInit(); //初始化默认状态 
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //允许RTC时钟 
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_2); //选择RTC时钟源LSI 38K、2=19K
  while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET);
  
  RTC_WakeUpCmd(DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //允许RTC时钟
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
  RTC_ITConfig(RTC_IT_WUT, ENABLE); //开启中断
  RTC_SetWakeUpCounter(time); //设置RTC Weakup计算器初值
  RTC_WakeUpCmd(ENABLE);
}
void RTC_WAKEUP_ENABLE()
{
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  RTC_WakeUpCmd(ENABLE);
  
}
void RTC_WAKEUP_DISABLE()
{
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  RTC_WakeUpCmd(DISABLE);
  
}