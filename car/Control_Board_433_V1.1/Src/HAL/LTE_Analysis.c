#include "stm32f4xx_hal.h"
#include "LTE_analysis.h"
#include "SIM7600SA.h"


int LTE_Parse(unsigned char *pbuff, device_com_down dev_info)
{
    unsigned int ch;
	unsigned int sum_crc=0;
	unsigned int i;
	unsigned char res;
	res=0;

    ch = (pbuff[0]<<8)+pbuff[1];
    

    if (ch == Ser_Head) //如果第五个字符是C，($GPRMC)
    {
        dev_info.len=pbuff[2]<<24+pbuff[3]<<16+pbuff[4]<<8+pbuff[6];


		
		
		//CRC calculate
		if(CRC_decode(&pbuff[2],dev_info.len)==1)
		{
                    res=1;
		}
		else 
		   res=0;

	
    }
    return res;
}


