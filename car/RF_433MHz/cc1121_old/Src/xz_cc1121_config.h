#ifndef __XZ_CC1121_CONFIG_H__
#define __XZ_CC1121_CONFIG_H__


#include "xz_spi.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx.h"


#define CC1121_SPI_CS_Begin()                UTIL_SPI_CS_Begin()
#define CC1121_SPI_CS_End()                  UTIL_SPI_CS_End()
#define CC1121_SPI_3W_ReadWriteUint8(x)      UTIL_SPI_3W_ReadWriteUint8(x)
#define CC1121_SPI_3W_ReadUint8()            UTIL_SPI_3W_ReadWriteUint8(0xFF)
#define CC1121_SPI_3W_WriteUint8(x)          UTIL_SPI_3W_ReadWriteUint8(x)

#define CC1121_RST_OUT_LOW()                 HAL_GPIO_WritePin(GPIOA,RF_SPI_RST,GPIO_PIN_RESET)
#define CC1121_RST_OUT_HIGH()                HAL_GPIO_WritePin(GPIOA,RF_SPI_RST,GPIO_PIN_SET) 





#endif
