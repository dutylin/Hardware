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

extern volatile hw3000_mode_t 	_hw3000_mode;
extern hw3000_data_t _hw3000_data_buf;
extern  uint8_t 		_hw3000_irq_request;

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
       }
    }
}


