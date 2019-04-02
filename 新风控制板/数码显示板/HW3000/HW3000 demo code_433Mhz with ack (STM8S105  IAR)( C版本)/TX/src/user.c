#include "stm8s.h"
#include "user.h"
#include "delay.h"
#include <string.h>
#include <stdbool.h>
#include "hw3000.h"


extern hw3000_data_t _hw3000_data_buf;
extern volatile hw3000_mode_t 	_hw3000_mode;
extern  uint8_t 		_hw3000_irq_request;


/**
  * @brief  Configure TIM4 to generate an update interrupt each 1ms 
  * @param  None
  * @retval None
  */
void TIM4_Config(void)
{
  /* TIM4 configuration:
   - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
   clock used is 16MHz / 128 = 125000 Hz
  - With 62500 Hz we can generate time base:
      max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 =2.048  ms
      min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
  - In this example we need to generate a time base equal to 2 ms
   so TIM4_PERIOD = (0.002 * 125000 - 1) = 249 */

  /* Time base configuration */
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, 249);
  /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  /* Enable update interrupt */
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  

  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);
}

