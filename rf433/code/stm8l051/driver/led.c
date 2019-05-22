#include "led.h"

void LED_Init(void)
{
 GPIO_Init(LED_PORT,LED_PIN1|LED_PIN1, GPIO_Mode_Out_PP_High_Slow);
}