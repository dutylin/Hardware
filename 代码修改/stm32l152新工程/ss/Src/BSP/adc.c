#include "global.h"
#include "adc.h"
 extern ADC_HandleTypeDef hadc;
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

unsigned int Alfter_filter[4];
/***************************传感器变量相关****************************/
extern uint32_t CV_Value[5]= {1,1,1,1};
/**************************************************************
函数：void ADC_Config(void)
功能：ADC
**************************************************************/
void ADC_Config(void)
{
   
ADC_ChannelConfTypeDef sConfig = {0};
  GPIO_InitTypeDef GPIO_InitStruct = {0};	
__HAL_RCC_GPIOA_CLK_ENABLE();

#if (defined Liceng)||(defined Weiyan)||(defined Yali)
    /* 通道1、2对应PA4/PA5 */
  
  /*Configure GPIO pin : PC4 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

#elif (defined Maogan)||(defined Maosuo)
    /* 通道1、2对应PA4/PA5 3、4通道对应PA6/PA7*/
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif
    /*RCC_HSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
    {
    }*/
//ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */
  /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc.Init.LowPowerAutoWait = ADC_AUTOWAIT_UNTIL_DATA_READ;
  hadc.Init.LowPowerAutoPowerOff = ADC_AUTOPOWEROFF_IDLE_DELAY_PHASES;
  hadc.Init.ChannelsBank = ADC_CHANNELS_BANK_A;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.NbrOfConversion = 1;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_4CYCLES;
  HAL_ADC_ConfigChannel(&hadc, &sConfig);
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
}
uint32_t CV_Value[5],adc_tmp;
/**************************************************************
函数：void Get_ADC(uint8_t channel)
功能：ADC转换获取
**************************************************************/
void Get_ADC(uint8_t channel)
{
    ADC_ChannelConfTypeDef  sConfig;
    switch(channel)
    {
    case 0:
		sConfig.Channel= 1;
		sConfig.Rank = 1;
		sConfig.SamplingTime = ADC_SAMPLETIME_192CYCLES;
        //ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_192Cycles);
		    //CV_Value[channel]=0x00;
	 HAL_ADC_ConfigChannel(& hadc,&sConfig );
	    
        break;
    case 1:
        //ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_192Cycles);
		    //CV_Value[channel]=0x00;
		sConfig.Channel = 2;
		sConfig.Rank = 1;
		sConfig.SamplingTime = ADC_SAMPLETIME_192CYCLES;
	        HAL_ADC_ConfigChannel(& hadc,&sConfig );

			
        break;
    case 2:
		sConfig.Channel = 3;
		sConfig.Rank = 1;
		sConfig.SamplingTime = ADC_SAMPLETIME_192CYCLES;
	        HAL_ADC_ConfigChannel(& hadc,&sConfig );
        break;
    //case 3:
    //    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_192Cycles);
     //   break;
    }
	HAL_ADC_Start(& hadc);
	 HAL_ADC_PollForConversion(&hadc,10);;
    //ADC_Cmd(ADC1,ENABLE);
    //ADC_SoftwareStartConv(ADC1); //使能指定的ADC1的软件转换启动功能
   // while(HAL_ADC_GetState(& hadc)!=HAL_ADC_STATE_EOC_REG);//等待转换结束
#ifdef Liceng  //基准电压3300mV 离层0-300mm    采集电压=AD值*3300/4096  0~2500mv->0~300
    //CV_Value[channel]=ADC_GetConversionValue(ADC1);
		//CV_Value[channel]=(unsigned int)((194061*(ADC_GetConversionValue(ADC1)-0x0140))>>19);
//CV_Value[channel]=(unsigned int)((6300*(ADC_GetConversionValue(ADC1)-0x0263))>>15)-0x02;
//		if(channel==1)
//		{
		adc_tmp = HAL_ADC_GetValue(&hadc);
		CV_Value[channel]=(unsigned int)((9200*(adc_tmp-0x1B9))>>15)+0x10;//为了测试数据暂时注释掉
		if(CV_Value[channel]>0x258) 
		{
			CV_Value[channel]=0;
		}
		else if(CV_Value[channel]>0x00&CV_Value[channel]<0x20) 
		{
			CV_Value[channel]=0;
		}
	
//		}
//		else
//		{
//		CV_Value[channel]=(unsigned int)((8813*(ADC_GetConversionValue(ADC1)-0x1B9))>>15)+0x76;
//		}
			////CV_Value[channel]=(unsigned int)((3150*(ADC_GetConversionValue(ADC1)-0x0131))>>15)-0x04;
		 //CV_Value[channel]=(unsigned int)((1645*(ADC_GetConversionValue(ADC1)+0x0F21))>>10);
		
		
#elif (defined Maogan)||(defined Weiyan)||(defined Yali)           //0.5~4.5->0~600
    CV_Value[channel]=(unsigned int)((825*HAL_ADC_GetValue(&hadc))>>13-37.5);
#elif defined Maosuo
    CV_Value[channel]=(unsigned int)((825*HAL_ADC_GetValue(&hadc))>>12-75);
#endif
    //ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
    //ADC_SoftwareStartConv(ADC1);
    //ADC_Cmd(ADC1,DISABLE);
	HAL_ADC_Stop(& hadc);
	//ADC_ConversionStop_Disable(ADC_HandleTypeDef* hadc)
	//ADC_Enable(& hadc);	
}

//300/2500*(AD*3300/4096)



