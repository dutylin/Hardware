#include "uartPase.h"
#include "ddl.h"
extern unsigned char UsartReceiveData[BUFFERSIZE];
unsigned char uartReceiveLen;
extern unsigned char uartReceiveriteIndex;
unsigned char uartReadIndex;
unsigned char uartPrase()
{
    CommandInfo_typedef CommandInfo;
    unsigned char TmpuartReceiveriteIndex;
	TmpuartReceiveriteIndex = uartReceiveLen;
	if(uartReadIndex!=uartReceiveriteIndex&&(uartReceiveLen%sizeof(CommandInfo)==0)
	{
	    memcpy(CommandInfo)
		unsigned char crc = crc_cal(&CommandInfo,UsartReceiveData+uartReadIndex);
		unsigned char crc=crc_cal(&CommandInfo,sizeof(CommandInfo));
		if(crc!=CommandInfo.crc)
		{
				uartReadIndex = uartReadIndex +sizeof(CommandInfo);
				if(uartReadIndex>=BUFFERSIZE)
					uartReadIndex = 0;
			
		}
		else 
		{
				unsigned char res= anlyz_uart_data(&CommandInfo);
				switch(res)
				{
					case :break;	
				}
		}
	}
}

