#ifndef __CC1110_H
#define __CC1110_H
#include "stm32l1xx.h"
#include "global.h"
#ifdef __cplusplus
 extern "C" {
#endif 

void CC1110_Config (uint32_t baud_rate);
void CC1110_Send (uint8_t * TxMsg ,int len);
void CC1110_Gpio_RESET(void);
CCStatus CC1110_TX_Status(void);
#ifdef __cplusplus
}
#endif
#endif


