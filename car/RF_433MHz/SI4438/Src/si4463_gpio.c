// ========================================================
/// @file       DK_SPI.c
/// @brief      RF IO Configuration
/// @version    V1.0
/// @date       2016/08/19
/// @company    WooZoom Co., Ltd.
/// @website    http://www.woozoom.net
/// @author     ZhangMeng
/// @mobile     +86-13804023611
// ========================================================
#include "si4463_gpio.h"
extern SPI_HandleTypeDef hspi1;

void Si4463_GpioInt()
{
	
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();	

	//RF Rst configuration
	GPIO_InitStruct.Pin =  RF_RST;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;	
	GPIO_InitStruct.Speed =GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init( GPIOA, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(GPIOA, RF_RST, GPIO_PIN_SET);	


	// SPI Cs IO Configuration
	GPIO_InitStruct.Pin = nCS ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;	
	GPIO_InitStruct.Speed =GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init( GPIOA	, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(GPIOA, nCS, GPIO_PIN_SET);

}



