#ifndef __FLASH_H
#define __FLASH_H
#ifdef __cplusplus
extern "C" {
#endif
#include "stm32l1xx.h"


void FLASH_Read(uint32_t Start_Address,uint32_t End_Address,uint32_t *pbuf);
void FLASH_Write(uint32_t Start_Address,uint32_t End_Address, uint32_t *pbuf);

#endif

/*******************************************************************************/
