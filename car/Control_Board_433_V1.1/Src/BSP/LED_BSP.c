#include "main.h"
#include "BSP.h"
#include "LED_BSP.h"
#include "stm32f4xx_hal.h"
#include "systick_timer.h"

void LED_all_ctrl(unsigned char flag)
{
	switch(flag)
	{
		case 0:
		{
			/*Configure GPIO pin Output Level */
			HAL_GPIO_WritePin(GPIOB, LED_BACK_Pin, GPIO_PIN_RESET);

			/*Configure GPIO pin Output Level */
			HAL_GPIO_WritePin(LED_PWR_GPIO_Port, LED_PWR_Pin, GPIO_PIN_RESET);

			/*Configure GPIO pin Output Level */
			HAL_GPIO_WritePin(GPIOD, LED_GPS_Pin|LED_433MHz_Pin, GPIO_PIN_RESET);		
		}break;
		case 1:
		{
			/*Configure GPIO pin Output Level */
			HAL_GPIO_WritePin(GPIOB, LED_BACK_Pin, GPIO_PIN_SET);

			/*Configure GPIO pin Output Level */
			HAL_GPIO_WritePin(LED_PWR_GPIO_Port, LED_PWR_Pin, GPIO_PIN_SET);

			/*Configure GPIO pin Output Level */
			HAL_GPIO_WritePin(GPIOD, LED_GPS_Pin|LED_433MHz_Pin, GPIO_PIN_SET);		
		}break;			
	}

}

void LED_ctrl(unsigned char sel_led,unsigned char flag)
{
	if(sel_led==SEL_PWR_LED)
	{
		switch(flag)
		{
			case 0:
			{

				/*Configure GPIO pin Output Level */
				HAL_GPIO_WritePin(LED_PWR_GPIO_Port, LED_PWR_Pin, GPIO_PIN_RESET);

			}break;
			case 1:
			{
				/*Configure GPIO pin Output Level */
				HAL_GPIO_WritePin(LED_PWR_GPIO_Port, LED_PWR_Pin, GPIO_PIN_SET);
			}break;			
		}	
	}
	else if(sel_led==SEL_LTE_LED)
	{
		switch(flag)
		{
			case 0:
			{

				/*Configure GPIO pin Output Level */
				HAL_GPIO_WritePin(GPIOB, LED_BACK_Pin, GPIO_PIN_RESET);		
			}break;
			case 1:
			{
				/*Configure GPIO pin Output Level */
				HAL_GPIO_WritePin(GPIOB, LED_BACK_Pin, GPIO_PIN_SET);
			
			}break;			
		}	
	}
	
	else if(sel_led==SEL_GPS_LED)
	{
		switch(flag)
		{
			case 0:
			{
				/*Configure GPIO pin Output Level */
				HAL_GPIO_WritePin(GPIOD, LED_GPS_Pin, GPIO_PIN_RESET);		
			}break;
			case 1:
			{
				/*Configure GPIO pin Output Level */
				HAL_GPIO_WritePin(GPIOD, LED_GPS_Pin, GPIO_PIN_SET);		
			}break;			
		}	
	}
	else if(sel_led==SEL_RF_LED)
	{
	
			switch(flag)
			{
				case 0:
				{
					/*Configure GPIO pin Output Level */
					HAL_GPIO_WritePin(GPIOD, LED_433MHz_Pin, GPIO_PIN_RESET);		
				}break;
				case 1:
				{

					/*Configure GPIO pin Output Level */
					HAL_GPIO_WritePin(GPIOD, LED_433MHz_Pin, GPIO_PIN_SET);		
				}break;			
			}	
	}	

}

