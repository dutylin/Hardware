// ========================================================
/// @file       FLASH.c
/// @brief      FLASH Configuration and WR Operation
/// @version    V1.0
/// @date       2016/08/19
/// @company    WooZoom Co., Ltd.
/// @website    http://www.woozoom.net
/// @author     ZhangMeng
/// @mobile     +86-13804023611
// ========================================================
#include "stm32f4xx_hal.h"
#include "EEPROM.h"

static FLASH_EraseInitTypeDef EraseInitStruct;
uint32_t Address = FLASH_USER_START_ADDR, PAGEError = 0;

uint32_t mcuID[3];
uint32_t* cpuidGetId(void)
{
    mcuID[0] = *(__IO uint32_t*)(0x1FFF7A10);
    mcuID[1] = *(__IO uint32_t*)(0x1FFF7A14);
    mcuID[2] = *(__IO uint32_t*)(0x1FFF7A18);
	 return mcuID;
}
/*******************************************************************************
function: user flash Init
input:no
output:no
*******************************************************************************/
void flash_init()
{
	/* Fill EraseInit structure*/
//	  HAL_FLASH_Unlock();
//		EraseInitStruct.TypeErase   = FLASH_TYPEERASE_SECTORS;
//		EraseInitStruct.Sector = FLASH_USER_START_ADDR;
//		EraseInitStruct.NbSectors     = (FLASH_USER_END_ADDR - FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE;	
//		HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError);
}


/*******************************************************************************
function: write data into flash
input:addr is the address;
*******************************************************************************/
void flash_write(uint32_t addr,uint32_t data,uint16_t numtoread)	
{
	int i;
	HAL_FLASH_Unlock();

	Address=FLASH_USER_START_ADDR+4*addr;
	for(i=0;i<numtoread;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, data);	
		Address =Address+4;
	}
	HAL_FLASH_Lock();
}
/*******************************************************************************
function: read data from flash
input:addr is the address;
*******************************************************************************/
void flash_read(uint32_t addr,uint32_t *pbuffer,uint16_t numtoread)	
{
	uint16_t i;
	Address = FLASH_USER_START_ADDR+4*addr;
	for(i=0;i<numtoread;i++)
	{
		pbuffer[i] = *(__IO uint32_t *)Address;	
		Address = Address+4;
	}
}
