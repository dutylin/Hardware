
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */
#include "BSP.h"
#include "Module_ctrl.h"
#include "ds18b20.h"
#include "LED_BSP.h"
#include "Flash_BSP.h"
#include "systick_timer.h"
#include "SIM7600SA.h"
#include "Module_ctrl.h"
#include "stmflash.h"
//#include "spi_flash2.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/


extern UART_HandleTypeDef huart5;


/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define  FLASH_WriteAddress     0x000F0000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress
#define  M25P64_FLASH_ID        0x202016
#define  BufferSize (countof(Tx_Buffer)-1)
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
//extern UART_HandleTypeDef huart5;
//extern UART_HandleTypeDef huart2;
//extern UART_HandleTypeDef huart3;
//extern DMA_HandleTypeDef hdma_uart5_rx;
//extern DMA_HandleTypeDef hdma_uart5_tx;
//extern DMA_HandleTypeDef hdma_usart2_rx;
//extern DMA_HandleTypeDef hdma_usart2_tx;
//extern DMA_HandleTypeDef hdma_usart3_rx;
//extern DMA_HandleTypeDef hdma_usart3_tx;



unsigned char  Tx_Buffer[] = "STM32F10x SPI Firmware Library Example: communication with an M25P64 SPI FLASH";
unsigned char  Index, Rx_Buffer[BufferSize];
uint32_t  FLASH_ID;
unsigned char uart5_dma_state=1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/


/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void system_reset()
{
  NVIC_SystemReset();
}

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
const unsigned char TEXT_Buffer[]= {"Apollo STM32F4 SPI TEST"};
#define SIZE sizeof(TEXT_Buffer)
unsigned char datatemp[SIZE];
unsigned int Float_temperature;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint32_t tmp;
//	uint32_t FLASH_SIZE;

	HW_init();
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */


  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */

  /* USER CODE BEGIN 2 */
        GetCPU_ID();
       STMFLASH_Read(FLASH_SAVE_ADDR,&tmp,1);
//	if(tmp==0x0000fefe)
//	{
//		STMFLASH_Read_Init_Parameters();
//	}
	LED_all_ctrl(1);
	LED_ctrl(SEL_PWR_LED,PWR_LED_ON);
	uart_init();
	LTE_M_init();
	Sample_Timer_Init();//reloader timer counter
//IO_TEST();
//M25PXX_Init();

	/* Get SPI Flash ID */
	//FLASH_ID = SPI_FLASH_ReadID();
	// W25QXX_Erase_Chip();
	//FLASH_SIZE=MemoryByte_size*1024*1024;	//FLASH 大小为32M字节

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		;
		//M25PXX_Write_Test();
		// DS18B20_Get_Temp();	
		// GetTemp();
		Modue_Run_Process();
	}
  /* USER CODE END 3 */

}



/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
     PC2   ------> ADCx_IN12
*/


/* USER CODE BEGIN 4 */

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	 
  if(huart==&huart5)
	{
 // __HAL_DMA_DISABLE(huart5.hdmatx);		
		//uart5_dma_state=1;

	}	
	//UsartType1.dmaSend_flag = USART_DMA_SENDOVER;
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==GPIO_PIN_4)
	{
		//nIRQ=0;
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	while(1)
	{
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	   tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
