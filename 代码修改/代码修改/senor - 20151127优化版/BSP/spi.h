#ifndef __SPI_H
#define __SPI_H
#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"

void SPI_Configuration(void);

uint16_t SPI2_ReadSend_byte(uint16_t data);


#ifdef __cplusplus
}
#endif
#endif

