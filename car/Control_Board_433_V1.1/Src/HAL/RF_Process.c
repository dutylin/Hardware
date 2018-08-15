#include "main.h"
#include "BSP.h"
#include "LTE_BSP.h"
#include "stm32f4xx_hal.h"
#include "systick_timer.h"
#include "LED_BSP.h"
#include <string.h>
#include "CRC.h"
#include "RF_Process.h"
void RF_Process()
{
;
}
unsigned char RF_Param(unsigned char *pbuff, RF_INFO_DEF rf_info)
{
    unsigned int tmp;
	
	tmp=(pbuff[2]>>8)+pbuff[3];
	if(CRC_decode(pbuff,tmp))
	{
		rf_info.len         = tmp-2;
		rf_info.dev_sn      = (pbuff[4]>>8)+pbuff[5];
		rf_info.temperature = (pbuff[6]>>8)+pbuff[7];
    return 1;
	}
	return 0;


}
