#include "stm8l15x.h"
#include "adc.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADC1_DR_ADDRESS        ((uint16_t)0x5344)
#define BUFFER_SIZE            ((uint8_t) 0x01)
#define BUFFER_ADDRESS         ((uint16_t)(&Buffer))
#define ASCII_NUM_0            ((uint8_t)   48)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t Buffer[BUFFER_SIZE]  = {0};
uint32_t PotVoltage = 0;
uint32_t BNCVoltage = 0;
uint8_t LCDString[16] = " 0.00V   0.00V ";

/**
  * @brief  Configure ADC peripheral 
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  /* Initialize and configure ADC1 */
  ADC_Init(ADC1, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_2);
  ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);
 // ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_384Cycles);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 Channels 3 */
  ADC_ChannelCmd(ADC1, ADC_Channel_15, ENABLE); /* connected to Potentiometer RV */
  /* Enable ADC1 Channels 24 */
  //ADC_ChannelCmd(ADC1, ADC_Channel_24, ENABLE); /* connected to BNC */
}
/**
  * @brief  Configure DMA peripheral 
  * @param  None
  * @retval None
  */
static void DMA_Config(void)
{
  /* Connect ADC to DMA channel 0 */
  SYSCFG_REMAPDMAChannelConfig(REMAP_DMA1Channel_ADC1ToChannel0);

  DMA_Init(DMA1_Channel0, BUFFER_ADDRESS,
           ADC1_DR_ADDRESS,
           BUFFER_SIZE,
           DMA_DIR_PeripheralToMemory,
           DMA_Mode_Circular,
           DMA_MemoryIncMode_Inc,
           DMA_Priority_High,
           DMA_MemoryDataSize_HalfWord);

  /* DMA Channel0 enable */
  DMA_Cmd(DMA1_Channel0, ENABLE);

  /* Enable DMA1 channel0 Transfer complete interrupt */
  DMA_ITConfig(DMA1_Channel0, DMA_ITx_TC, ENABLE);

  /* DMA enable */
  DMA_GlobalCmd(ENABLE);
}
/**
  * @brief  Enables or disables the specified ADC DMA request.
  * @param  ADCx where x can be 1 to select the specified ADC peripheral.
  * @param  NewState : new state of the specified ADC DMA transfer.
    *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
//void ADC_DMACmd(ADC_TypeDef* ADCx, FunctionalState NewState)
//{
//  /* Check the parameters */
//  assert_param(IS_FUNCTIONAL_STATE(NewState));
//
//  if (NewState != DISABLE)
//  {
//    /* Enable the specified ADC DMA request */
//    ADCx->SQR[0] &= (uint8_t)~ADC_SQR1_DMAOFF;
//  }
//  else
//  {
//    /* Disable the specified ADC DMA request */
//    ADCx->SQR[0] |= ADC_SQR1_DMAOFF;
//  }
//}
///**
//  * @brief  Configures the ADC conversion through external trigger.
//  * @param  ADCx where x can be 1 to select the specified ADC peripheral.
//  * @param  ADC_ExtEventSelection : Specifies the external trigger.
//  *          This parameter can be one of the following values:
//  *            @arg ADC_ExtEventSelection_None: Conversion starts only by software start
//  *            @arg ADC_ExtEventSelection_Trigger1: Trigger 1  Enables conversion
//  *            @arg ADC_ExtEventSelection_Trigger2: Trigger 2  Enables conversion
//  *            @arg ADC_ExtEventSelection_Trigger3: Trigger 3  Enables conversion      
//  * @param  ADC_ExtTRGSensitivity : Specifies the external trigger sensitivity.
//  *          This parameter can be one of the following values:
//  *            @arg ADC_ExtTRGSensitivity_Rising: External Trigger Sensitivity is Rising Edge
//  *            @arg ADC_ExtTRGSensitivity_Falling: External Trigger Sensitivity is Falling Edge
//  *            @arg ADC_ExtTRGSensitivity_All: External Trigger Sensitivity is Falling and Rising Edge
//  * @retval None
//  */
//void ADC_ExternalTrigConfig(ADC_TypeDef* ADCx,
//                            ADC_ExtEventSelection_TypeDef ADC_ExtEventSelection,
//                            ADC_ExtTRGSensitivity_TypeDef ADC_ExtTRGSensitivity)
//{
//  /* Check the parameters */
//  assert_param(IS_ADC_EXT_EVENT_SELECTION(ADC_ExtEventSelection));
//  assert_param(IS_ADC_EXT_TRG_SENSITIVITY(ADC_ExtTRGSensitivity));
//
//  /*clear old configuration of CR2 register */
//  ADCx->CR2 &= (uint8_t)~(ADC_CR2_TRIGEDGE | ADC_CR2_EXTSEL);
//
//  /* set the External Trigger Edge Sensitivity  and the external event 
//     selection */
//  ADCx->CR2 |= (uint8_t)( (uint8_t)ADC_ExtTRGSensitivity | \
//                          (uint8_t)ADC_ExtEventSelection);
//}
/**
  * @brief  Selects the TIM1 Trigger Output Mode.
  * @param  TIM1_TRGOSource specifies the Trigger Output source.
  *          This parameter can be one of the following values
  *            @arg TIM1_TRGOSource_Reset
  *            @arg TIM1_TRGOSource_Enable
  *            @arg TIM1_TRGOSource_Update
  *            @arg TIM1_TRGOSource_OC1
  *            @arg TIM1_TRGOSource_OC1REF
  *            @arg TIM1_TRGOSource_OC2REF
  *            @arg TIM1_TRGOSource_OC3REF
  *            @arg TIM1_TRGOSource_OC4REF
  * @retval None
  */
//void TIM1_SelectOutputTrigger(TIM1_TRGOSource_TypeDef TIM1_TRGOSource)
//{
//  /* Check the parameters */
//  assert_param(IS_TIM1_TRGO_SOURCE(TIM1_TRGOSource));
//  /* Reset the MMS Bits & Select the TRGO source */
//  TIM1->CR2 = (uint8_t)((uint8_t)(TIM1->CR2 & (uint8_t)(~TIM1_CR2_MMS)) | (uint8_t) TIM1_TRGOSource);
//}

void adc_init()
{
	/* ADC configuration -------------------------------------------*/
	 ADC_Config();
	
	  /* DMA configuration -------------------------------------------*/
	 DMA_Config();
	   
	 /* Enable ADC1 DMA requests*/
	 ADC_DMACmd(ADC1, ENABLE);
	
	 /* Start ADC1 Conversion using TIM1 TRGO*/
	 ADC_ExternalTrigConfig(ADC1, ADC_ExtEventSelection_Trigger2,
							ADC_ExtTRGSensitivity_Rising);
	
	 /* Master Mode selection: Update event */
	 TIM1_SelectOutputTrigger(TIM1_TRGOSource_Update);
	
	 /* Enable TIM1 */
	 TIM1_Cmd(ENABLE);
	
	 /* Enable Interrupts */
	 enableInterrupts();

}
