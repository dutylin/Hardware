#include "main.h"
#include "BSP.h"
#include "LTE_BSP.h"
#include "stm32f4xx_hal.h"
#include "systick_timer.h"
extern unsigned char uart5_dma_state;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_uart5_rx;
extern DMA_HandleTypeDef hdma_uart5_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;
extern UART_HandleTypeDef huart5;
extern unsigned char LTE_RX_BUFF[buffer_size];

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
	//__HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);

	HAL_GPIO_WritePin(MCU_LTE_RST_GPIO_Port, MCU_LTE_RST_Pin, GPIO_PIN_RESET);
	delay_ms(500);
	HAL_GPIO_WritePin(MCU_LTE_RST_GPIO_Port, MCU_LTE_RST_Pin, GPIO_PIN_SET);
	delay_ms(500);
	HAL_GPIO_WritePin(MCU_LTE_RST_GPIO_Port, MCU_LTE_RST_Pin, GPIO_PIN_RESET);
}


//lte开关机函数
void LET_power_ctrl()
{
	HAL_GPIO_WritePin(GPIOC, MCU_LTE_PWR_Pin, GPIO_PIN_RESET);
	delay_ms(500);
	HAL_GPIO_WritePin(GPIOC, MCU_LTE_PWR_Pin, GPIO_PIN_SET);
	delay_ms(500);
	HAL_GPIO_WritePin(GPIOC, MCU_LTE_PWR_Pin, GPIO_PIN_RESET);
}


//gps天线电源使能函数
void LET_gps_power_ctrl(unsigned char  flag)
{
	switch(flag)
	{
	case 0:
		HAL_GPIO_WritePin(GPIOC, GPS_PWR_CE_Pin, GPIO_PIN_RESET);
	case 1:
		HAL_GPIO_WritePin(GPIOC, GPS_PWR_CE_Pin, GPIO_PIN_SET);
	}
}


void LTE_power_on()
{
	//电源不使能，GPS天线不供电，关机
	HAL_GPIO_WritePin(GPIOC, LTE_PWR_EN_Pin, GPIO_PIN_SET);

}
extern Uart_Types UsartType5;
/******************************************************************************/
void Send_Comm(unsigned char* comm,unsigned short len)
{

	HAL_UART_DMAStop(&huart5);
	HAL_UART_DMAResume(&huart5);
       UsartType5.real_index=0;
	UsartType5.rx_len=0;
	UsartType5.rx_len_var=0;
	UsartType5.loop_index=0;
	HAL_UART_Receive_DMA(&huart5,UsartType5.usartDMA_rxBuf,buffer_size);
	//delay_ms(100);
	uart5_dma_state=0;
	HAL_UART_Transmit(&huart5,comm,len,100);

}

void dma_clear_flag()
{
	;
}



