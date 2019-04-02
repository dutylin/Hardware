#include "flash.h"

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

HAL_StatusTypeDef FLASHStatus = 0;

uint32_t NbrOfPage = 0, j = 0, Address = 0;
extern unsigned char Flash_Write_Buff[4];
/**************************************************************
函数：uint8_t Flash_write_check(uint32_t Start_Address,uint32_t End_Address, uint8_t *pbuf)
功能：flash写检查
**************************************************************/
uint8_t Flash_write_check(uint32_t Start_Address,uint32_t End_Address, uint32_t *pbuf)
{
    __IO uint32_t MemoryProgramStatus = 0,i=0;
    uint8_t result_check =0;
    MemoryProgramStatus = 0x0;

    while (Start_Address < End_Address)
    {
        *(pbuf+i) = *(__IO uint32_t*)Start_Address;
        if (*(pbuf+i) != Flash_Write_Buff[i])
        {
            MemoryProgramStatus++;
            result_check=0;
        }
        else
        {
            result_check=1;
        }
        i=i+1;
        if(i>15)i=0;
        Start_Address = Start_Address + 4;
    }
    return result_check;
}
/**************************************************************
函数：void FLASH_Read(uint32_t Start_Address,uint32_t End_Address,uint8_t *pbuf)
功能：flash读函数
**************************************************************/
void FLASH_Read(uint32_t Start_Address,uint32_t End_Address,uint32_t *pbuf)
{
    __IO uint32_t data32 = 0 , MemoryProgramStatus = 0,i = 0;
    MemoryProgramStatus = 0x0;
    while (Start_Address < End_Address)
    {
        *(uint32_t*)(pbuf+i) = *(__IO uint32_t*)Start_Address;
        pbuf=pbuf+1;
        Start_Address = Start_Address + 4;
    }
}
/**************************************************************
函数：void FLASH_Write(uint32_t Start_Address,uint32_t End_Address, uint8_t *pbuf)
功能：flash写函数
**************************************************************/
void FLASH_Write(uint32_t Start_Address,uint32_t End_Address, uint32_t *pbuf)
{
    //DATA_EEPROM_Unlock();
   HAL_FLASHEx_DATAEEPROM_Unlock( );
  //  FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
                    //| FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR | FLASH_FLAG_OPTVERRUSR);

    NbrOfPage = ((End_Address - Start_Address) + 1 ) >> 2;

    /*****************写之前先擦出扇区*********************/
    for(j = 0; j < NbrOfPage; j++)
    {
        FLASHStatus = HAL_FLASHEx_DATAEEPROM_Erase(FLASH_TYPEPROGRAM_WORD,Start_Address + (4 * j));
    }
    /***************开始从起始地址写数据********************/
    while (Start_Address < End_Address)
    {
        if ( HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,Start_Address, *(uint32_t*)pbuf) == HAL_OK)
        {
            pbuf=pbuf+1;
            Start_Address = Start_Address + 4;
        }
        else{
        }
    }
    HAL_FLASHEx_DATAEEPROM_Lock();
}


