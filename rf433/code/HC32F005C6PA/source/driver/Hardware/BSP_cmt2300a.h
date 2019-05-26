#ifndef BSP_CMT2300A_H
#define BSP_CMT2300A_H
#include "gpio.h"
#define SPI_PORT          2
#define INT_PORT          3
#define CSB_PIN           5
#define SCLK_PIN          6
#define SDIO_PIN          3
#define FCSB_PIN          4
#define GPIO_PINS (CSB_PIN | SCLK_PIN  |SDIO_PIN  | FCSB_PIN)
#define INTERRUPT_PORT          3
#define INT1                    2
#define INT2                    1 
 void cmt2300a_GPIO_Init(void);
#endif