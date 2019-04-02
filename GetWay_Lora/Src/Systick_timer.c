#include "main.h"
#include "systick_timer.h"
#include "stm32f1xx_hal.h"



uint32_t TimingDelay;
//系统延时函数，单位：ms
void delay_us(uint32_t time)
{    
   uint32_t i=0;  
   while(time--)
   {
      i=10;  
      while(i--) ;    
   }
}
//系统延时函数，单位：ms
void delay_ms(uint32_t nms)
{

	TimingDelay = nms;
	while(TimingDelay != 0);
}

void HAL_SYSTICK_Callback(void)
{

	if (TimingDelay != 0x00000000)
	{
		TimingDelay--;
	}

	

}
