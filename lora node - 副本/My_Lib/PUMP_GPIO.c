#include "stm8l15x.h"
#include "PUMP_GPIO.h"
void PUMP_GPIO_INIT()
{
  GPIO_Init(GPIOD, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast);         //推挽输出

GPIO_Init(GPIOD, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast);         //推挽输出
GPIO_ResetBits(GPIOD,GPIO_Pin_4);
}
void PUMP_CTRL(u8 flag)
{
  if(flag==1)
  {
    GPIO_SetBits(GPIOD,GPIO_Pin_4);
  }
  else if(flag=0)
    GPIO_ResetBits(GPIOD,GPIO_Pin_4);
  else
   GPIO_ToggleBits(GPIOD,GPIO_Pin_4);
}