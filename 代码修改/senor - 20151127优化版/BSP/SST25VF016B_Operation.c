////////////////////////////////////////////////////////////////////////
//                         SST25VF016B²Ù×÷º¯Êý
//1.Ä¬ÈÏÊ¹ÓÃSPI1½øÐÐÍ¨ÐÅ£¬Ê±ÖÓÉÏ±ßÑØËø´æ£¬ÐèÌáÇ°½øÐÐ³õÊ¼»¯
//2.STM32¿â°æ±¾v3.5
//3.Æ¬Ñ¡¹Ü½ÅNSSÐèÌáÇ°³õÊ¼»¯Îª¸ßµçÆ½£¬Í¬Ê±¸ü¸ÄSelectDevice¡¢UnselectDevice¶¨Òå
////////////////////////////////////////////////////////////////////////

#include "stm32f10x.h"
#define SelectDevice GPIO_ResetBits(GPIOA, GPIO_Pin_4);
#define UnselectDevice GPIO_SetBits(GPIOA, GPIO_Pin_4);

u8 SPISend(u8 data)
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)!=SET);
	SPI_I2S_SendData(SPI1, data);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)!=SET);
	return(SPI_I2S_ReceiveData(SPI1));	
}

////////////////////////////////////////////////////
//¶ÁÈ¡×´Ì¬¼Ä´æÆ÷Öµ
//·µ»Ø¶ÁÈ¡Öµ
u8 SST25VF016B_ReadReg(void)
{
	u8 Register;
	SelectDevice;
	SPISend(0x05);
	Register=SPISend(0x00);
	UnselectDevice;
	return (Register);
}

/////////////////////////////////////////////////////
//Ã¦¼ì²â
//·µ»ØÃ¦×´Ì¬£¬0£º¿ÕÏÐ£¬!0£ºÃ¦
u8 SST25VF016B_IsBusy(void)
{
	u8 Busy,Register;
	Register=SST25VF016B_ReadReg();
	Busy=Register & 0x01;
	return (Busy);
}

////////////////////////////////////////////////////////////
//¶ÁÈ¡Êý¾Ý
//u32 Address - ÆðÊ¼µØÖ·£¬·¶Î§£º0x000000-0x1fffff
//u8 *Data - ¶ÁÈ¡Öµ
//u32 nCount - ¶ÁÈ¡×Ö½ÚÊý
void SST25VF016B_Read(u32 Address,u8 *Data,u32 nCount)
{
	u32 temp;
	while(SST25VF016B_IsBusy());
	SelectDevice;
	SPISend(0x0b);
	SPISend((Address&0xffffff)>>16);
	SPISend((Address&0xffff)>>8);
	SPISend(Address&0xff);
	SPISend(0);
	for(temp=0;temp<nCount;temp++)
	{
		Data[temp]=SPISend(0x00);
	}
	UnselectDevice;
}

////////////////////////////////////////////////////
//Ð´Ê¹ÄÜ
void SST25VF016B_WriteEnable(void)
{
	SelectDevice;
	SPISend(0x06);
	SPISend(0x50);
	SPISend(0x01);
	SPISend(0x02);
	UnselectDevice;
}

////////////////////////////////////////////////////
//4KB¿é²Á³ý
//u32 Address - ÆðÊ¼µØÖ·£¬·¶Î§£º0-511
void SST25VF016B_4KBErase(u32 Address)
{
	Address<<=12;
	SST25VF016B_WriteEnable();
	SelectDevice;
	SPISend(0x20);
	SPISend((Address&0xffffff)>>16);
	SPISend((Address&0xffff)>>8);
	SPISend(Address&0xff);
	UnselectDevice;
	while(SST25VF016B_IsBusy());
}

////////////////////////////////////////////////////
//32KB¿é²Á³ý
//u32 Address - ÆðÊ¼µØÖ·£¬·¶Î§£º0-63
void SST25VF016B_32KBErase(u32 Address)
{
	Address<<=15;
	SST25VF016B_WriteEnable();
	SelectDevice;
	SPISend(0x52);
	SPISend((Address&0xffffff)>>16);
	SPISend((Address&0xffff)>>8);
	SPISend(Address&0xff);
	UnselectDevice;
	while(SST25VF016B_IsBusy());
}

////////////////////////////////////////////////////
//64KB¿é²Á³ý
//u32 Address - ÆðÊ¼µØÖ·£¬·¶Î§£º0-31
void SST25VF016B_64KBErase(u32 Address)
{
	Address<<=16;
	SST25VF016B_WriteEnable();
	SelectDevice;
	SPISend(0xd8);
	SPISend((Address&0xffffff)>>16);
	SPISend((Address&0xffff)>>8);
	SPISend(Address&0xff);
	UnselectDevice;
	while(SST25VF016B_IsBusy());
}

////////////////////////////////////////////////////
//ÕûÆ¬²Á³ý
void SST25VF016B_AllErase(void)
{
	SST25VF016B_WriteEnable();
	SelectDevice;
	SPISend(0x60);
	UnselectDevice;
	while(SST25VF016B_IsBusy());	
}

////////////////////////////////////////////////////
//Ð´ÈëÊý¾Ý
//u32 Address - ÆðÊ¼µØÖ·£¬·¶Î§£º0x000000-0x1fffff
//u8 *Data - ´ýÐ´Öµ
//u32 nCount - Ð´Èë×Ö½ÚÊý
void SST25VF016B_Write(u32 Address,u8 *Data,u32 nCount)
{
	u32 temp;
	for (temp=0;temp<nCount;temp++)
	{
		SST25VF016B_WriteEnable();
		SelectDevice;
		SPISend(0x02);
		SPISend((Address&0xffffff)>>16);
		SPISend((Address&0xffff)>>8);
		SPISend(Address&0xff);
		SPISend(Data[temp]);
		UnselectDevice;	
		Address++;
		while(SST25VF016B_IsBusy());
	}
}}

/****************************************************************************
* Ãû    ³Æ£ºvoid FlashReadID(void)
* ¹¦    ÄÜ£º¶Á¹¤³§Âë¼°ÐÍºÅµÄº¯Êý
* Èë¿Ú²ÎÊý£º  
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎÞ 
****************************************************************************/ 
unsigned char fac_id,dev_id;

void FlashReadID(void)
{
	while(SST25VF016B_IsBusy());
	SelectDevice;
  	SPISend(0x90);
	SPISend(0x00);
	SPISend(0x00);
	SPISend(0x00);
  	fac_id= SPISend(0x00);;		          //BFH: ¹¤³ÌÂëSST
	dev_id= SPISend(0x00);;	              //41H: Æ÷¼þÐ  	fac_id= SPISend(0x00);;		          //BFH: ¹¤³ÌÂëSST
	dev_id= SPISend(0x00);;	              //41H: Æ÷¼þÐÍºÅSST25VF016B     
	UnselectDev	while(SST25VF016B_IsBusy());
	SelectDevice;
  	SPISend(0x90);
	SPISend(0x00);
	SPISend(0x00);
	SPISend(0x00);
  	fac_id= SPISend(0x00);;		          //BFH: ¹¤³ÌÂëSST
	dev_id= SPISend(0x00);;	              //41H: Æ÷¼þÐÍºÅSST25VF016B     
	UnselectDevice;
}

