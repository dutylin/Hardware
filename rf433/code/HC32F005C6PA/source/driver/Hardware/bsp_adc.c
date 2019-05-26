#include "bsp_adc.h"
#include "gpio.h"
#include "ddl.h"
/**
* @brief  Configure ADC and Analog watchdog
* @param  None
* @retval None
*/
uint16_t ADC_Config(void)
{
  uint16_t ADCData;
  stc_adc_cfg_t      stcAdcCfg;
  stc_adc_norm_cfg_t stcAdcNormCfg;
  
  DDL_ZERO_STRUCT(stcAdcCfg);
  DDL_ZERO_STRUCT(stcAdcNormCfg);
  
  //Clk_SwitchTo(ClkRCL);
  //Clk_SetRCHFreq(ClkFreq24Mhz);
  //Clk_SwitchTo(ClkRCH);
  
  if (Ok != Clk_SetPeripheralGate(ClkPeripheralGpio, TRUE))
  {
    return Error;
  }
  
  if (Ok != Clk_SetPeripheralGate(ClkPeripheralAdcBgr, TRUE))
  {
    return Error;
  }
  
  Gpio_SetAnalog(ADC_VBAT_PORT, ADC_VBAT_CHANNEL, TRUE);
  
  Adc_Enable();
  M0P_BGR->CR_f.BGR_EN = 0x1u;//BGR必须使能
  M0P_BGR->CR_f.TS_EN = 0x0u;
  
  delay100us(1);
  
  stcAdcCfg.enAdcOpMode = AdcNormalMode;          //单次采样模式
  stcAdcCfg.enAdcClkSel = AdcClkSysTDiv1;         //PCLK
  stcAdcCfg.enAdcSampTimeSel = AdcSampTime4Clk;   //4个采样时钟
  stcAdcCfg.enAdcRefVolSel = RefVolSelInBgr2p5;   //参考电压:内部2.5V(avdd>3V,SPS<=200kHz)  SPS速率 = ADC时钟 / (采样时钟 + 16CLK) 
  stcAdcCfg.bAdcInBufEn = FALSE;                  //电压跟随器如果使能，SPS采样速率 <=200K
  stcAdcCfg.u32AdcRegHighThd = 0u;                //比较阈值上门限
  stcAdcCfg.u32AdcRegLowThd = 0u;                 //比较阈值下门限
  stcAdcCfg.enAdcTrig0Sel = AdcTrigDisable;       //ADC转换自动触发设置
  stcAdcCfg.enAdcTrig1Sel = AdcTrigDisable;
  Adc_Init(&stcAdcCfg);    
  
  stcAdcNormCfg.enAdcNormModeCh = ADC_VBAT_CHANNEL;  //通道0 P24
  stcAdcNormCfg.bAdcResultAccEn = FALSE;
  Adc_ConfigNormMode(&stcAdcCfg, &stcAdcNormCfg);
  
  Adc_Start();
  
  while(FALSE != Adc_PollBusyState());
  
  Adc_GetResult(&ADCData);
  Adc_ClrAccResult();
  return ADCData;
}
