#include "adc.h"
#include "stm8l15x_adc.h"
/**
  * @brief  Configure ADC and Analog watchdog
  * @param  None
  * @retval None
  */
 void ADC_Config(void)
{

      /* Enable ADC1 clock */
      CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);

      /* Initialize and configure ADC1 */
      ADC_Init(ADC1, ADC_ConversionMode_Continuous, ADC_Resolution_12Bit, ADC_Prescaler_2);

      /* ADC channel used for IDD measurement */
      ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);

      /* Enable ADC1 */
      ADC_Cmd(ADC1, ENABLE);

      /* Disable SchmittTrigger for ADC_Channel, to save power */
      ADC_SchmittTriggerConfig(ADC1, ADC_VBAT_CHANNEL, DISABLE);

      /* Enable ADC1 Channel used for IDD measurement */
      ADC_ChannelCmd(ADC1, ADC_VBAT_CHANNEL, ENABLE);

      /* Start ADC1 Conversion using Software trigger*/
      ADC_SoftwareStartConv(ADC1);

      /* Waiting until press Joystick Up */
//      while (Key != JOY_UP)
//      {
//        /* Wait until End-Of-Convertion */
//        while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)
//        {}
//
//        /* Get conversion value */
//        ADCData = ADC_GetConversionValue(ADC1);
//
//        /* Calculate voltage value in uV over capacitor  C67 for IDD measurement*/
//        VDD = (uint32_t)((uint32_t)ADCData * (uint32_t)ADC_CONVERT_RATIO);
//
//        /* Calculate the current consumption in uA */
//        IDD = (uint32_t) ((uint32_t)((VDD / EVAL_MAX9938_GAIN)) / (uint32_t)(EVAL_RESISTOR_R33));
//
//        /* Display Current  value on LCD */
//        CurrentDisplay(IDD);
//
//        /* Read Joystick */
//        Key = ReadJoystick();
//
//        /* Waiting Delay 200ms */
//        Delay(200);
//      }
      /* DeInitialize ADC1 */
      ADC_DeInit(ADC1);

      /* Disable ADC1 clock */
      CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
    
}
