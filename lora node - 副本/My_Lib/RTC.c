  #include "stm8l15x.h"
#include "stm8_eval.h"
#include "rtc.h"
void LSE_StabTime(void);
void LSE_StabTime(void)
{

  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);

  /* Configure TIM4 to generate an update event each 1 s */
  TIM4_TimeBaseInit(TIM4_Prescaler_16384, 123);
  /* Clear update flag */
  TIM4_ClearFlag(TIM4_FLAG_Update);

  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);

  /* Wait 1 sec */
  while ( TIM4_GetFlagStatus(TIM4_FLAG_Update) == RESET );

  TIM4_ClearFlag(TIM4_FLAG_Update);

  /* Disable TIM4 */
  TIM4_Cmd(DISABLE);

  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, DISABLE);
}
void RTC_init()
{
  /* Enable RTC clock */
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);
  /* Wait for LSE clock to be ready */
  while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);
  /* wait for 1 second for the LSE Stabilisation */
  LSE_StabTime();
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);

  /* Configures the RTC wakeup timer_step = RTCCLK/16 = LSE/16 = 488.28125 us */
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div2);

  /* Enable wake up unit Interrupt */
  RTC_ITConfig(RTC_IT_WUT, ENABLE);

  /* Enable general Interrupt*/
  enableInterrupts();
  RTC_SetWakeUpCounter(127);
  RTC_WakeUpCmd(ENABLE);
}
