#include "main.h"
#include "BSP.h"
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */
#include "SIM7600SA.h"
#include "Module_ctrl.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_uart5_rx;
DMA_HandleTypeDef hdma_uart5_tx;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;
DMA_HandleTypeDef hdma_adc1;
/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
Uart_Types_2 UsartType2;
Uart_Types_2 UsartType3;
Uart_Types UsartType5;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_ADC1_Init(void);
static void MX_UART5_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);

extern unsigned char LTE_RX_BUFF[buffer_size];//let数据接收缓冲器
void HW_init()
{
	/* MCU Configuration----------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStruct;
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_SPI1_Init();
	MX_ADC1_Init();
	MX_UART5_Init();
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
  GPIO_InitStruct.Pin = 	PWR_KEY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PWR_KEY_GPIO_Port, &GPIO_InitStruct);



  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);  
	__HAL_RCC_PWR_CLK_ENABLE();
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 108;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Activate the Over-Drive mode 
    */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC1 init function */
static void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_112CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}


/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* UART5 init function */
static void MX_UART5_Init(void)
{

  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART3 init function */
static void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  /* DMA1_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
     PC2   ------> ADCx_IN12
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LTE_PWR_EN_Pin|GPS_PWR_CE_Pin|MCU_LTE_PWR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DQ_Pin|FLASH_nCS1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RF_433_EN_Pin|LED_BACK_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_PWR_GPIO_Port, LED_PWR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LED_GPS_Pin|LED_433MHz_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MCU_LTE_RST_GPIO_Port, MCU_LTE_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : POWER_STATE_Pin CHRG_Pin */
  GPIO_InitStruct.Pin = POWER_STATE_Pin|CHRG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : LTE_PWR_EN_Pin GPS_PWR_CE_Pin MCU_LTE_PWR_Pin */
  GPIO_InitStruct.Pin = LTE_PWR_EN_Pin|GPS_PWR_CE_Pin|MCU_LTE_PWR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : BATTERY_SUP_Pin */
  GPIO_InitStruct.Pin = BATTERY_SUP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BATTERY_SUP_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DQ_Pin */
  GPIO_InitStruct.Pin = DQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DQ_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : FLASH_nCS1_Pin */
  GPIO_InitStruct.Pin = FLASH_nCS1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(FLASH_nCS1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : MOTO_SENSE_Pin POWER_STATEC9_Pin */
  GPIO_InitStruct.Pin = MOTO_SENSE_Pin|POWER_STATEC9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : RF_433_EN_Pin */
  GPIO_InitStruct.Pin = RF_433_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RF_433_EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_BACK_Pin */
  GPIO_InitStruct.Pin = LED_BACK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_BACK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MCU_433_ACK_Pin */
  GPIO_InitStruct.Pin = MCU_433_ACK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MCU_433_ACK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_PWR_Pin LED_GPS_Pin LED_433MHz_Pin */
  GPIO_InitStruct.Pin = LED_PWR_Pin|LED_GPS_Pin|LED_433MHz_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : MCU_LTE_RST_Pin */
  GPIO_InitStruct.Pin = MCU_LTE_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MCU_LTE_RST_GPIO_Port, &GPIO_InitStruct);

	
  GPIO_InitStruct.Pin = 	PWR_KEY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PWR_KEY_GPIO_Port, &GPIO_InitStruct);



  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn); 
  /* EXTI interrupt init*/
//  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}


void uart_init()
{

//	HAL_UART_Receive_DMA(&huart5,UsartType5.usartDMA_rxBuf,buffer_size);
//	__HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);    //??????
//	__HAL_UART_CLEAR_IDLEFLAG(&huart5);
	__HAL_UART_DISABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断
	HAL_UART_DMAStop(&huart2);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断
	__HAL_UART_CLEAR_IDLEFLAG(&huart2);
	HAL_UART_Receive_DMA(&huart2,UsartType2.usartDMA_rxBuf,buffer_size);

	__HAL_UART_DISABLE_IT(&huart3, UART_IT_IDLE);	//使能空闲中断
	HAL_UART_DMAStop(&huart3);
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);	//使能空闲中断
	__HAL_UART_CLEAR_IDLEFLAG(&huart3);
	HAL_UART_Receive_DMA(&huart3,UsartType3.usartDMA_rxBuf,buffer_size);



	__HAL_UART_DISABLE_IT(&huart5, UART_IT_IDLE);	//使能空闲中断
	HAL_UART_DMAStop(&huart5);
	__HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);	//使能空闲中断
	__HAL_UART_CLEAR_IDLEFLAG(&huart5);
	HAL_UART_Receive_DMA(&huart5,UsartType5.usartDMA_rxBuf,buffer_size);
	//HAL_UART_Receive_DMA(&huart2,UsartType2.usartDMA_rxBuf,buffer_size);
}

//串口接收空闲中断
void UsartReceive_IDLE(unsigned char uart_num)
{
	uint32_t temp;

	if(uart_num==2)
	{
		temp=__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE);
		if((temp!=RESET))
		{
			__HAL_UART_CLEAR_IDLEFLAG(&huart2);

			//HAL_UART_DMAStop(&huart2);
			temp=huart2.Instance->SR;
			temp=huart2.Instance->DR;
			//HAL_UART_DMAResume(&huart2);
			HAL_UART_DMAPause(&huart2);
			temp=hdma_usart2_rx.Instance->NDTR;
			UsartType2.rx_len=buffer_size-temp;
			UsartType2.receive_flag=1;
			UsartType2.real_index=UsartType2.real_index+UsartType2.rx_len;
			if(UsartType2.real_index>=buffer_size)
				UsartType2.real_index=UsartType2.real_index-buffer_size;
			HAL_UART_DMAPause(&huart2);
			HAL_UART_Receive_DMA(&huart2,UsartType2.usartDMA_rxBuf,buffer_size);

		}
	}
	if(uart_num==3)
	{
		temp=__HAL_UART_GET_FLAG(&huart3,UART_FLAG_IDLE);
		if((temp!=RESET))
		{
			__HAL_UART_CLEAR_IDLEFLAG(&huart3);


			temp=huart3.Instance->SR;
			temp=huart3.Instance->DR;
			HAL_UART_DMAPause(&huart3);
			temp=hdma_usart3_rx.Instance->NDTR;
			UsartType3.rx_len=buffer_size-temp;
			UsartType3.receive_flag=1;
			UsartType3.real_index=UsartType3.real_index+UsartType3.rx_len_var;
			if(UsartType3.real_index>=buffer_size)
				UsartType3.real_index=UsartType3.real_index-buffer_size;
			UsartType3.rx_len_var=UsartType3.rx_len;
			UsartType3.rx_len=buffer_size-temp;
			if(UsartType3.rx_len<UsartType3.rx_len_var)
			{
				UsartType3.rx_len_var=UsartType3.rx_len+buffer_size-UsartType3.rx_len_var;
			}
			else
				UsartType3.rx_len_var=UsartType3.rx_len-UsartType3.rx_len_var;

			HAL_UART_DMAResume(&huart3);
			//   UsartType3.real_index=0;
			//UsartType3.rx_len=0;
			//UsartType3.rx_len_var=0;
			//UsartType3.loop_index=0;
			HAL_UART_Receive_DMA(&huart3,UsartType3.usartDMA_rxBuf,buffer_size);

		}
	}
	if(uart_num==5)
	{
		temp=__HAL_UART_GET_FLAG(&huart5,UART_FLAG_IDLE);
		if((temp!=RESET))
		{
			__HAL_UART_CLEAR_IDLEFLAG(&huart5);

			temp=huart5.Instance->SR;
			temp=huart5.Instance->DR;
			HAL_UART_DMAPause(&huart5);
			temp=hdma_uart5_rx.Instance->NDTR;
			UsartType5.real_index=UsartType5.real_index+UsartType5.rx_len_var;
			if(UsartType5.real_index>=buffer_size)
				UsartType5.real_index=UsartType5.real_index-buffer_size;
			UsartType5.rx_len_var=UsartType5.rx_len;
			UsartType5.rx_len=buffer_size-temp;
			if(UsartType5.rx_len<UsartType5.rx_len_var)
			{
				UsartType5.rx_len_var=UsartType5.rx_len+buffer_size-UsartType5.rx_len_var;
			}
			else
				UsartType5.rx_len_var=UsartType5.rx_len-UsartType5.rx_len_var;
			UsartType5.receive_flag=1;
			//if(get_connection_step()>=MAX_STEP)
			GetServer_Info_Poll();//服务器下发指令

			//SIMCOM_Register_Network();


			HAL_UART_DMAResume(&huart5);
			HAL_UART_Receive_DMA(&huart5,UsartType5.usartDMA_rxBuf,buffer_size);

		}
	}


}
void uart_dma_resume(unsigned char uart_num)
{
	if(uart_num==1)
	{
		HAL_UART_DMAStop(&huart2);
		HAL_UART_DMAResume(&huart2);
		UsartType2.real_index=0;
		UsartType2.rx_len=0;
		UsartType2.rx_len_var=0;
		UsartType2.loop_index=0;
		HAL_UART_Receive_DMA(&huart2,UsartType2.usartDMA_rxBuf,buffer_size);

	}
	if(uart_num==3)
	{
		HAL_UART_DMAStop(&huart3);
		HAL_UART_DMAResume(&huart3);
		UsartType3.real_index=0;
		UsartType3.rx_len=0;
		UsartType3.rx_len_var=0;
		UsartType3.loop_index=0;
		HAL_UART_Receive_DMA(&huart3,UsartType3.usartDMA_rxBuf,buffer_size);

	}
	if(uart_num==5)
	{
		HAL_UART_DMAStop(&huart5);
		HAL_UART_DMAResume(&huart5);
		UsartType5.real_index=0;
		UsartType5.rx_len=0;
		UsartType5.rx_len_var=0;
		UsartType5.loop_index=0;
		HAL_UART_Receive_DMA(&huart5,UsartType5.usartDMA_rxBuf,buffer_size);

	}


}
uint32_t tmp1,tmp2;


ADC_Types Get_Battery_Gas_adc(unsigned char count)

{
	ADC_Types adc_value_tmp;
	unsigned char i,j;
        float tmp;

	for(j=0; j<count; j++)
	{
		for(i=0; i<2; i++)
		{

			/*##-3- Start the conversion process ######*/
			if(HAL_ADC_Start(&hadc1) != HAL_OK)
			{ Error_Handler(); }
			/*##-4- Wait for the end of conversion ######*/
			HAL_ADC_PollForConversion(&hadc1, 10);
//			if(HAL_ADC_GetState(&hadc1) == HAL_ADC_STATE_EOC_REG)
//			{
//			/*##-5- Get the converted value of channel ##*/
//			tmp1 = HAL_ADC_GetValue(&hadc1);
//			}

		
			//HAL_ADC_Start(&hadc1);
		//	HAL_ADC_PollForConversion(&hadc1,0xffff);
			if(i==0)
				tmp1=HAL_ADC_GetValue(&hadc1)+tmp1;
			if(i==1)
				tmp2=HAL_ADC_GetValue(&hadc1)+tmp2;
		}


	}
	tmp1=tmp1/count;
	tmp2=tmp2/count;
  tmp=tmp1;
	tmp=tmp/4096*33;
	  tmp1=(uint32_t) tmp;
	adc_value_tmp.battery[0]=tmp1>>24;

	adc_value_tmp.battery[1]=tmp1>>16;
	adc_value_tmp.battery[2]=tmp1>>8;
	adc_value_tmp.battery[3]=tmp1;
  tmp=tmp2;
	tmp=tmp/4096*33;
  tmp2=(uint32_t) tmp;
	adc_value_tmp.gas[0]=tmp2>>24;

	adc_value_tmp.gas[1]=tmp2>>16;
	adc_value_tmp.gas[2]=tmp2>>8;
	adc_value_tmp.gas[3]=tmp2;




	HAL_ADC_Stop(&hadc1);
	return adc_value_tmp;
}

unsigned char Get_Moto_Presense()
{

	if(HAL_GPIO_ReadPin(MOTO_SENSE_GPIO_Port,MOTO_SENSE_Pin)==1)
		return 1;
	else
		return 0;

}
void IO_TEST()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = FLASH_nCS1_Pin|FLASH_SDO_Pin|FLASH_SDI_Pin|FLASH_SCLK_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOA, FLASH_nCS1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, FLASH_nCS1_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOA, FLASH_SDO_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, FLASH_SDO_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOA, FLASH_SDI_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, FLASH_SDI_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOA, FLASH_SCLK_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, FLASH_SCLK_Pin, GPIO_PIN_SET);



}

unsigned char PWR_KEY_Dectection()
{
	if(HAL_GPIO_ReadPin(PWR_KEY_GPIO_Port,PWR_KEY_Pin)==0)
	{
		delay_ms(10);
		if(HAL_GPIO_ReadPin(PWR_KEY_GPIO_Port,PWR_KEY_Pin)==1)
			return 0;
		while(HAL_GPIO_ReadPin(PWR_KEY_GPIO_Port,PWR_KEY_Pin)==0)
			;
		return 1;
	}
	else
		return 0;
}

void Halt_Init()
{
 //GPIO_InitTypeDef GPIO_InitStruct; 
 // MX_GPIO_Init();
//  GPIO_InitStruct.Pin = GPIO_PIN_All;
//  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//  
//  GPIO_InitStruct.Pin = GPIO_PIN_All;
//  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//  
//  GPIO_InitStruct.Pin = GPIO_PIN_All;
//  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

//  GPIO_InitStruct.Pin = GPIO_PIN_All;
//  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);	
//	
//  GPIO_InitStruct.Pin = GPIO_PIN_All;
//  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);	


//	
//	
//  __HAL_RCC_GPIOE_CLK_ENABLE();
//  __HAL_RCC_GPIOH_CLK_ENABLE();
//  __HAL_RCC_GPIOC_CLK_ENABLE();
//  __HAL_RCC_GPIOA_CLK_ENABLE();
//  __HAL_RCC_GPIOB_CLK_ENABLE();
//  __HAL_RCC_GPIOD_CLK_ENABLE();

//  GPIO_InitStruct.Pin = 	PWR_KEY_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(PWR_KEY_GPIO_Port, &GPIO_InitStruct);



//  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(EXTI4_IRQn);  
	__HAL_RCC_PWR_CLK_ENABLE();


 // HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI);
 // HAL_PWR_EnterSTANDBYMode();
//HAL_PWR_DisableSleepOnExit();
HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);



 // wake_up_config();   

} 

