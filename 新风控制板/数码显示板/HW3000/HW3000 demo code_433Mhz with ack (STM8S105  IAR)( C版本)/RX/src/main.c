/******************************************************************************
 * @file     main.c
 * @brief    main funcation
 *           
 *
 * @version  1.0
 * @date     2016-01-14
 *
 * @author   Sundy
 *
 * @note
 * @Copyright (C) 2016 Shanghai Eastsoft Microelectronics C0., Ltd.
 ******************************************************************************/
#include "stm8s.h"
#include "gpio.h"
#include "hw3000.h"
#include "delay.h"
#include "user.h"
#include <stdbool.h>

extern volatile hw3000_mode_t 	_hw3000_mode;
extern hw3000_data_t _hw3000_data_buf;
extern  uint8_t 		_hw3000_irq_request;
extern bool Packge_ok;

/******************************************************************************
 * @brief    main
 *           
 * @note
 *
 * @param	 None
 *			 
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void main(void)
{   
   uint16_t  crc;
   
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);//16Mhz
	MCU_gpio_init();
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB,EXTI_SENSITIVITY_FALL_ONLY);
    EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_FALL_ONLY);
    
	_hw3000_mode.osc  = OSC20MHZ;
	_hw3000_mode.band = FREQ_BAND_433MHZ;
	_hw3000_mode.rate = SYMBOL_RATE_10K;
	_hw3000_mode.freq_mode = DIRECT;
	_hw3000_mode.power = 20;
	_hw3000_mode.frame_mode = FRAME;
	_hw3000_mode.ack_mode = hw3000_DISABLE;
	_hw3000_mode.tx_mode = 1;
	_hw3000_mode.rx_mode = 0;
	_hw3000_mode.lp_enable = hw3000_DISABLE;
    
   
	hw3000_init(_hw3000_mode);   
    enableInterrupts(); /* enable interrupts */
    
    
    hw3000_rx_enable();
    while(1)
    {        
      if(_hw3000_irq_request==1)
       {
        _hw3000_irq_request=0;
        hw3000_rx_task();
        if(Packge_ok)
        {
         Packge_ok=false;
         
        _hw3000_data_buf.data[0] =0;//包长高字节
        _hw3000_data_buf.data[1] =5;//包长低字节,本例中我们Ack发送5个字节的数据
        _hw3000_data_buf.len = _hw3000_data_buf.data[1];
        _hw3000_data_buf.data[2]=0x58;
        _hw3000_data_buf.data[3]=0x67;
        _hw3000_data_buf.data[4]=0;
        _hw3000_data_buf.data[3]=0;//包后两个字节留着装包CRC校验数据
        crc = cal_crc_ccitt(_hw3000_data_buf.data, _hw3000_data_buf.len - 2);
        _hw3000_data_buf.data[_hw3000_data_buf.len - 2] = crc >> 8;
	    _hw3000_data_buf.data[_hw3000_data_buf.len - 1] = crc;  
         
        disableInterrupts();
        hw3000_init(_hw3000_mode);   
        enableInterrupts(); /* enable interrupts */
        m928_delay(50);
        if (hw3000_tx_data(_hw3000_mode, &_hw3000_data_buf) == 0) 
        {
          GPIO_WriteLow(RF_LED_PORT,RF_LED_IO);
          m928_delay(20);
          GPIO_WriteHigh(RF_LED_PORT,RF_LED_IO);
        }
       }

        hw3000_init(_hw3000_mode); 
        hw3000_rx_enable();
       }
    }
}


