/******************************************************************************
 * @file     gpio.h
 * @brief    gpio funcations
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
#ifndef __M928_GPIO_H
#define __M928_GPIO_H

#include "stm8s.h"

#define  HW3000_CSN_PORT        GPIOB
#define  HW3000_CSN_IO          GPIO_PIN_6

#define  HW3000_CLK_PORT        GPIOB
#define  HW3000_CLK_IO          GPIO_PIN_5

#define  HW3000_SDO_PORT        GPIOB
#define  HW3000_SDO_IO          GPIO_PIN_4

#define  HW3000_SDI_PORT        GPIOB
#define  HW3000_SDI_IO          GPIO_PIN_3

#define  HW3000_GPIO0_PORT      GPIOB
#define  HW3000_GPIO0_IO        GPIO_PIN_0

#define  HW3000_GPIO1_PORT      GPIOB
#define  HW3000_GPIO1_IO        GPIO_PIN_1

#define  HW3000_GPIO2_PORT      GPIOB
#define  HW3000_GPIO2_IO        GPIO_PIN_2

#define  HW3000_IRQ_PORT        GPIOB
#define  HW3000_IRQ_IO          GPIO_PIN_7

#define  HW3000_PDN_PORT        GPIOC
#define  HW3000_PDN_IO          GPIO_PIN_1
   
#define  HW3000_KEY1_PORT       GPIOD
#define  HW3000_KEY1_IO         GPIO_PIN_0

#define  HW3000_KEY2_PORT       GPIOD
#define  HW3000_KEY2_IO         GPIO_PIN_2

#define  HW3000_KEY3_PORT       GPIOD
#define  HW3000_KEY3_IO         GPIO_PIN_3

#define  OLED_CS_PORT           GPIOC
#define  OLED_CS_IO             GPIO_PIN_5
   
#define  OLED_RST_PORT          GPIOG
#define  OLED_RST_IO            GPIO_PIN_0
   
#define  OLED_DC_PORT           GPIOG
#define  OLED_DC_IO             GPIO_PIN_1
   
#define  OLED_SCLK_PORT         GPIOC
#define  OLED_SCLK_IO           GPIO_PIN_6
   
#define  OLED_SDIN_PORT         GPIOC
#define  OLED_SDIN_IO           GPIO_PIN_7
  

#define  RF_LED_PORT            GPIOE
#define  RF_LED_IO              GPIO_PIN_0


void MCU_gpio_init(void);



#endif


