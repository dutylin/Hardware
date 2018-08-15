#include "stm32f4xx_hal.h"
#include "LTE_analysis.h"
#include "SIM7600SA.h"


int LTE_Parse(unsigned char *pbuff, device_com_down dev_info)
{
    unsigned int ch;
	unsigned int sum_crc=0;
	unsigned int i;

    ch = (pbuff[0]<<8)+pbuff[1];
    

    if (ch == Ser_Head) //如果第五个字符是C，($GPRMC)
    {
        dev_info.len=pbuff[2]<<24+pbuff[3]<<16+pbuff[4]<<8+pbuff[6];


		
		
		//CRC calculate
		crc_init(&sum_crc);
		for(i=2; i<(dev_info.len+2); i++)
		{
			crc_accumulate(&pbuff[i],&sum_crc);//CRC校验
		}
		dev_info.crc_value=sum_crc;
		if(pbuff[i+3]==dev_info.crc_value&0xff&&pbuff[i+4]==(dev_info.crc_value>>8))
			return 1; 
		else 
			return 0;
	
    }
    return 0;
}


