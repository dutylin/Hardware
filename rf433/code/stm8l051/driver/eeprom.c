

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "main.h"
#include "eeprom.h"
params_typedef params;
params_typedef *system_params_get()
{
  return &params;
}
void eeprom_data_read()
{
  uint32_t  add;
  add = FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS  ;
  if(FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS)==0x5a)
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
void eeprom_write(void)
{
  uint32_t  add;
  add = FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS  ;
  
  FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
  FLASH_Unlock(FLASH_MemType_Data);
  FLASH_ProgramByte(add++, 0x5a);
  FLASH_ProgramByte(add++, params.keyfunc);
  FLASH_ProgramByte(add++, params.freq>>8);
  FLASH_ProgramByte(add++, params.freq);
  FLASH_ProgramByte(add++, params.channel);
  FLASH_ProgramByte(add++, params.sn);
  FLASH_Lock(FLASH_MemType_Data);	  
}

