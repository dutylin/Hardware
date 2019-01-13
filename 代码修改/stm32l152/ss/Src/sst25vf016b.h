#ifndef ss25v_H
#define ss25v_H
#include "stm32l1xx_hal.h"

typedef unsigned char u8;
typedef unsigned int u16;
typedef uint32_t u32;
void FLASH_SPI_Configuration(void);
void SPI_FLASH_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void FlashReadID(unsigned char fac_id,unsigned char dev_id1,unsigned char dev_id2);

#endif

