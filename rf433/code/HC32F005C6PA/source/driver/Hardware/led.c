#include "led.h"
#include "delay.h"
#include "main.h"
void LED_Init(void)
{
 GPIO_Init(LED_PORT,LED_PIN1|LED_PIN1, GPIO_Mode_Out_PP_High_Slow);
}
void LED_blink(unsigned char led_sel)
{
  switch(led_sel)
  {
  case LED_RED:
    {
    	GPIO_ToggleBits(LED_PORT, LED_PIN1);
	Delay_ms(200);		
    }break;
  case LED_GREEN:
     {
    	GPIO_ToggleBits(LED_PORT, LED_PIN2);
	Delay_ms(200);		
    }break; 	
  default :
  {
    	GPIO_ToggleBits(LED_PORT, LED_PIN1);
	Delay_ms(200);		
    }break;
  }
}

void LED_ctrl(unsigned char led_sel,bool status)
{
  switch(led_sel)
  {
  case LED_RED:
    { 
	if(status == ON)
    	GPIO_SetBits(LED_PORT, LED_PIN1);
       else
    	GPIO_ResetBits(LED_PORT, LED_PIN1);	   	
    }break;
  case LED_GREEN:
    { 
	if(status == ON)
    	GPIO_SetBits(LED_PORT, LED_PIN2);
       else
    	GPIO_ResetBits(LED_PORT, LED_PIN2);	   	
    }break;	
  default :
    { 
	if(status == ON)
    	GPIO_SetBits(LED_PORT, LED_PIN1);
       else
    	GPIO_ResetBits(LED_PORT, LED_PIN1);	   	
    }break;
  }
}