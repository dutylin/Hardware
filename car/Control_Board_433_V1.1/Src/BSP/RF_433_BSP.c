#include "main.h"
#include "BSP.h"
#include "stm32f4xx_hal.h"
#include "systick_timer.h"
void LTE_hw_init()
{
	//电源不使能，GPS天线不供电，关机
   HAL_GPIO_WritePin(GPIOC, LTE_PWR_EN_Pin|GPS_PWR_CE_Pin|MCU_LTE_PWR_Pin, GPIO_PIN_RESET);
	//lte复位无效
   HAL_GPIO_WritePin(MCU_LTE_RST_GPIO_Port, MCU_LTE_RST_Pin, GPIO_PIN_RESET);
}


//lte复位函数
void LET_reset()
{
	HAL_GPIO_WritePin(MCU_LTE_RST_GPIO_Port, MCU_LTE_RST_Pin, GPIO_PIN_SET);
	delay_ms(500);
	HAL_GPIO_WritePin(MCU_LTE_RST_GPIO_Port, MCU_LTE_RST_Pin, GPIO_PIN_RESET);
}


//lte开关机函数
void LET_power_ctrl()
{
	HAL_GPIO_WritePin(GPIOB, RF_433_EN_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(GPIOC, MCU_LTE_PWR_Pin, GPIO_PIN_SET);
	delay_ms(500);
	HAL_GPIO_WritePin(GPIOC, MCU_LTE_PWR_Pin, GPIO_PIN_RESET);
}


//gps天线电源使能函数
void LET_gps_power_ctrl(unsigned char  flag)
{
	switch(flag)
	{
		case 0:   HAL_GPIO_WritePin(GPIOC, GPS_PWR_CE_Pin, GPIO_PIN_RESET);
		case 1:   HAL_GPIO_WritePin(GPIOC, GPS_PWR_CE_Pin, GPIO_PIN_SET);	
	}

}


void LTE_power_on()
{
	//电源不使能，GPS天线不供电，关机
   HAL_GPIO_WritePin(GPIOC, LTE_PWR_EN_Pin|GPS_PWR_CE_Pin|MCU_LTE_PWR_Pin, GPIO_PIN_RESET);
	//lte复位无效
   HAL_GPIO_WritePin(MCU_LTE_RST_GPIO_Port, MCU_LTE_RST_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, RF_433_EN_Pin, GPIO_PIN_SET);
}
