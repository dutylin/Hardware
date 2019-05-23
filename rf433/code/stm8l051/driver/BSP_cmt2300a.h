#ifndef BSP_CMT2300A_H
#define BSP_CMT2300A_H
#include "stm8l15x.h"
#include "stm8l15x_gpio.h"
#define SPI_PORT          GPIOB

#define CSB_PIN           GPIO_Pin_4
#define SCLK_PIN          GPIO_Pin_5
#define SDIO_PIN          GPIO_Pin_6
#define FCSB_PIN          GPIO_Pin_7

#define INTERRUPT_PORT          GPIOC
#define INT1                    GPIO_Pin_0
#define INT2                    GPIO_Pin_1 
 void cmt2300a_GPIO_Init(void);
#endif