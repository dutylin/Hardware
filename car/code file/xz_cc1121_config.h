#ifndef __XZ_CC1121_CONFIG_H__
#define __XZ_CC1121_CONFIG_H__


#include "xz_spi.h"


#define CC1121_SPI_CS_Begin()                UTIL_SPI_CS_Begin()
#define CC1121_SPI_CS_End()                  UTIL_SPI_CS_End()
#define CC1121_SPI_3W_ReadWriteUint8(x)      UTIL_SPI_3W_ReadWriteUint8(x)
#define CC1121_SPI_3W_ReadUint8()            UTIL_SPI_3W_ReadWriteUint8(0xFF)
#define CC1121_SPI_3W_WriteUint8(x)          UTIL_SPI_3W_ReadWriteUint8(x)

#define CC1121_RST_OUT_LOW()                 GPIO_ResetBits(GPIOC,RF_SPI_RST)
#define CC1121_RST_OUT_HIGH()                GPIO_SetBits(GPIOC,RF_SPI_RST) 





#endif
