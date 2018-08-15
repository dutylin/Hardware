#include "main.h"
#include "BSP.h"
#include "stm32f4xx_hal.h" 
#include "ds18b20_BSP.h"
void DS18B20_DQ_OUT(unsigned char flag)
{
    switch(flag)
    {
        case 0:HAL_GPIO_WritePin(GPIOA, DQ_Pin, GPIO_PIN_RESET);break;
        case 1:HAL_GPIO_WritePin(GPIOA, DQ_Pin, GPIO_PIN_SET);break;
        
    }
}
unsigned char DS18B20_DQ_IN()
{
   unsigned char Pin_status;
   Pin_status=HAL_GPIO_ReadPin(GPIOA, DQ_Pin);
   return Pin_status;
}
void DS18B20_IO_IN()
{

}	//PB5输入模式
void DS18B20_IO_OUT() 
{

} 	//PB5输出模式
