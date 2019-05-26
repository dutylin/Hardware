

/* Includes ------------------------------------------------------------------*/
#include "flash.h"
#include "main.h"
#include "eeprom.h"
/******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "flash.h"



params_typedef params;
params_typedef *system_params_get()
{
  return &params;
}
unsigned char FLASH_ReadByte(uint32_t readadd)
{
	return *((volatile uint32_t*)readadd);
}
void eeprom_data_read()
{
  uint32_t  add;
  add =   0x3ff0;  
  if(*((volatile uint32_t*)add)==0x5a)
  {
    params.keyfunc= FLASH_ReadByte(add++);
    params.freq= FLASH_ReadByte(add++)<<8+FLASH_ReadByte(add++);
    params.channel= FLASH_ReadByte(add++);
    params.sn= FLASH_ReadByte(add++);				
  }
  else
  {
    params.keyfunc= KEYFUNC_DEFAULT;
    params.freq= FREQ_DEFAULT;
    params.channel= CHANNEL_DEFAULT;
    params.sn= SN_DEFAULT ;
  }
  
}

/*******************************************************************************
 * FLASH 中断服务函数
 ******************************************************************************/
 void FlashInt(void)
 {
    if (TRUE == Flash_GetIntFlag(flash_int0))
    {
        Flash_ClearIntFlag(flash_int0);

        Flash_DisableIrq(flash_int0);
    }
    if (TRUE == Flash_GetIntFlag(flash_int1))
    {
        Flash_ClearIntFlag(flash_int1);
        Flash_DisableIrq(flash_int1);
    }
      
 }

en_result_t eeprom_write(void)
{
     en_result_t       enResult = Error;
    uint32_t          u32Addr  = 0x3ff0;
    uint8_t           u8Data   = 0x5a;

   u32Addr  = 0x3ff0;
    Flash_Init(FlashInt, 0);
    
    Flash_SectorErase(u32Addr);
    
	enResult = Flash_WriteByte(u32Addr++, 0x5a);
	enResult = Flash_WriteByte(u32Addr++,  params.keyfunc);
	enResult = Flash_WriteByte(u32Addr++, (volatile uint8_t)(params.freq>>8));
	enResult = Flash_WriteByte(u32Addr++, (volatile uint8_t)params.freq);
	enResult = Flash_WriteByte(u32Addr++, params.channel);
	enResult = Flash_WriteByte(u32Addr++,  params.sn);
	return   enResult;
}



   
   


 
