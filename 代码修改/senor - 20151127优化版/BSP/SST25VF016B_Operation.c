////////////////////////////////////////////////////////////////////////
//                         SST25VF016B²Ù×÷º¯Êı
//1.Ä¬ÈÏÊ¹ÓÃSPI1½øĞĞÍ¨ĞÅ£¬Ê±ÖÓÉÏ±ßÑØËø´æ£¬ĞèÌáÇ°½øĞĞ³õÊ¼»¯
//2.STM32¿â°æ±¾v3.5
//3.Æ¬Ñ¡¹Ü½ÅNSSĞèÌáÇ°³õÊ¼»¯Îª¸ßµçÆ½£¬Í¬Ê±¸ü¸ÄSelectDevice¡¢UnselectDevice¶¨Òå
////////////////////////////////////////////////////////////////////////

#include "stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "sst25vf016b.h"
typedef unsigned char  u8;
typedef unsigned int    u16;
typedef uint32_t         u32;
#define SelectDevice GPIO_ResetBits(GPIOA, GPIO_Pin_8);
#define UnselectDevice GPIO_SetBits(GPIOA, GPIO_Pin_8);
////////////SPI2½Ó¿ÚÅäÖÃ/////////////////////////////////////////////////////
void FLASH_SPI_Configuration(void)
{
	 
	
	  SPI_InitTypeDef  SPI_InitStructure;
	  GPIO_InitTypeDef GPIO_InitStructure;

	  RCC_APB1PeriphClockCmd( RCC_APB2Periph_SPI1, ENABLE);

	  /* SPI1Òı½Å¶¨Òå  PB5--SPI2_SCK  PB7--SPI1_MOSI */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6|  GPIO_Pin_7 ;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
		//GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
	  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	  
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8  ;
	  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
	  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  /* SPI2 Config -------------------------------------------------------------*/
	  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;   //SPI_Direction_2Lines_FullDuplex;
	  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	  SPI_InitStructure.SPI_CRCPolynomial = 7;
	  SPI_Init(SPI1, &SPI_InitStructure);
	  /* Enable SPI2 */
	  SPI_Cmd(SPI1, ENABLE);
}
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
//·µ»ØÃ¦×´Ì¬£¬0£º¿ÕÏĞ£¬!0£ºÃ¦
u8 SST25VF016B_IsBusy(void)
{
	u8 Busy,Register;
	Register=SST25VF016B_ReadReg();
	Busy=Register & 0x01;
	return (Busy);
}

////////////////////////////////////////////////////////////
//¶ÁÈ¡Êı¾İ
//u32 Address - ÆğÊ¼µØÖ·£¬·¶Î§£º0x000000-0x1fffff
//u8 *Data - ¶ÁÈ¡Öµ
//u32 nCount - ¶ÁÈ¡×Ö½ÚÊı
void SST25VF016B_Read(u8 *Data,u32 Address,u32 nCount)
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
//Ğ´Ê¹ÄÜ
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
//4KB¿é²Á³ı
//u32 Address - ÆğÊ¼µØÖ·£¬·¶Î§£º0-511
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
//32KB¿é²Á³ı
//u32 Address - ÆğÊ¼µØÖ·£¬·¶Î§£º0-63
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
//64KB¿é²Á³ı
//u32 Address - ÆğÊ¼µØÖ·£¬·¶Î§£º0-31
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
//ÕûÆ¬²Á³ı
void SST25VF016B_AllErase(void)
{
	SST25VF016B_WriteEnable();
	SelectDevice;
	SPISend(0x60);
	UnselectDevice;
	while(SST25VF016B_IsBusy());	
}

////////////////////////////////////////////////////
//Ğ´ÈëÊı¾İ
//u32 Address - ÆğÊ¼µØÖ·£¬·¶Î§£º0x000000-0x1fffff
//u8 *Data - ´ıĞ´Öµ
//u32 nCount - Ğ´Èë×Ö½ÚÊı
void SST25VF016B_Write_Page(u8 *Data,u32 Address,u32 nCount)
{
	u32 temp;
	SST25VF016B_WriteEnable();
	SelectDevice;
	SPISend(0x02);
	SPISend((Address&0xffffff)>>16);
	SPISend((Address&0xffff)>>8);
	SPISend(Address&0xff);
	for (temp=0;temp<nCount;temp++)
	{
		SPISend(Data[temp]);
		Address++;	
	}
	UnselectDevice;
	while(SST25VF016B_IsBusy());	
}


//ÎŞ¼ìÑéĞ´SPI FLASH 
//±ØĞëÈ·±£ËùĞ´µÄµØÖ··¶Î§ÄÚµÄÊı¾İÈ«²¿Îª0XFF,·ñÔòÔÚ·Ç0XFF´¦Ğ´ÈëµÄÊı¾İ½«Ê§°Ü!
//¾ßÓĞ×Ô¶¯»»Ò³¹¦ÄÜ 
//ÔÚÖ¸¶¨µØÖ·¿ªÊ¼Ğ´ÈëÖ¸¶¨³¤¶ÈµÄÊı¾İ,µ«ÊÇÒªÈ·±£µØÖ·²»Ô½½ç!
//pBuffer:Êı¾İ´æ´¢Çø
//WriteAddr:¿ªÊ¼Ğ´ÈëµÄµØÖ·(24bit)
//NumByteToWrite:ÒªĞ´ÈëµÄ×Ö½ÚÊı(×î´ó0x1fffff)
//CHECK OK
void SST25VF016B_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=4096-WriteAddr%4096; //µ¥Ò³Ê£ÓàµÄ×Ö½ÚÊı		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//²»´óÓÚ256¸ö×Ö½Ú
	while(1)
	{	   
		SST25VF016B_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//Ğ´Èë½áÊøÁË
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	
			NumByteToWrite-=pageremain;			  //¼õÈ¥ÒÑ¾­Ğ´ÈëÁËµÄ×Ö½ÚÊı
			if(NumByteToWrite>4096)pageremain=4096; //Ò»´Î¿ÉÒÔĞ´Èë256¸ö×Ö½Ú
			else pageremain=NumByteToWrite; 	  //²»¹»256¸ö×Ö½ÚÁË
		}
	};	    
} 


//Ğ´SPI FLASH  
//ÔÚÖ¸¶¨µØÖ·¿ªÊ¼Ğ´ÈëÖ¸¶¨³¤¶ÈµÄÊı¾İ
//¸Ãº¯Êı´ø²Á³ı²Ù×÷!
//pBuffer:Êı¾İ´æ´¢Çø
//WriteAddr:¿ªÊ¼Ğ´ÈëµÄµØÖ·(24bit)						
//NumByteToWrite:ÒªĞ´ÈëµÄ×Ö½ÚÊı(×î´ó0x1fffff)   
u8 SST25VF016B_BUFFER[4096];	
u8 WriteAddressPostion[3];
void SST25VF016B_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * SST25VF016B_BUF;	  
   	SST25VF016B_BUF=SST25VF016B_BUFFER;	
	 	secpos=WriteAddr/4096;//ÉÈÇøµØÖ·  
		secoff=WriteAddr%4096;//ÔÚÉÈÇøÄÚµÄÆ«ÒÆ
		secremain=4096-secoff;//ÉÈÇøÊ£Óà¿Õ¼ä´óĞ
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//²âÊÔÓÃ
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//²»´óÓÚ4096¸ö×Ö½Ú
	while(1) 
	{	
		SST25VF016B_Read(SST25VF016B_BUF,secpos*4096,4096);//¶Á³öÕû¸öÉÈÇøµÄÄÚÈİ
		for(i=0;i<secremain;i++)//Ğ£ÑéÊı¾İ
		{
			if(SST25VF016B_BUFFER[secoff+i]!=0XFF)break;//ĞèÒª²Á³ı  	  
		}
		if(i<secremain)//ĞèÒª²Á³ı
		{
			SST25VF016B_4KBErase(secpos);//²Á³ıÕâ¸öÉÈÇø
			for(i=0;i<secremain;i++)	   //¸´ÖÆ
			{
				SST25VF016B_BUF[i+secoff]=pBuffer[i];	  
			}
			SST25VF016B_Write_NoCheck(SST25VF016B_BUFFER,secpos*4096,4096);//Ğ´ÈëÕû¸öÉÈÇø  

		}else SST25VF016B_Write_NoCheck(pBuffer,WriteAddr,secremain);//Ğ´ÒÑ¾­²Á³ıÁËµÄ,Ö±½ÓĞ´ÈëÉÈÇøÊ£ÓàÇø¼ä. 				   
		if(NumByteToWrite==secremain)break;//Ğ´Èë½áÊøÁË
		else//Ğ´ÈëÎ´½áÊø
		{
			secpos++;//ÉÈÇøµØÖ·Ôö1
			secoff=0;//Æ«ÒÆÎ»ÖÃÎª0 	 

		   	pBuffer+=secremain;  //Ö¸ÕëÆ«ÒÆ
			WriteAddr+=secremain;//Ğ´µØÖ·Æ«ÒÆ	   
		   	NumByteToWrite-=secremain;				//×Ö½ÚÊıµİ¼õ
			if(NumByteToWrite>4096)secremain=4096;	//ÏÂÒ»¸öÉÈÇø»¹ÊÇĞ´²»Íê
			else secremain=NumByteToWrite;			//ÏÂÒ»¸öÉÈÇø¿ÉÒÔĞ´ÍêÁË
		}	 
	};	 
}

void Read_AddressWrite()
{
	SST25VF016B_Read(WriteAddressPostion,0,3);
}
void SPI_FLASH_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
   u32 tmp;
    SST25VF016B_Read(WriteAddressPostion,0,3);
   if(WriteAddressPostion[0]==0xff&&WriteAddressPostion[1]==0xff&&
   	 WriteAddressPostion[2]==0xff)
  	 tmp = 	 NumByteToWrite;
   else
   	tmp = WriteAddressPostion[0]<<16+ WriteAddressPostion[1]<<8+ 
   	     WriteAddressPostion[2] + NumByteToWrite ;
   if(tmp>=0x200000)
   {
   	tmp = tmp - 0x200000 + 3;
   	WriteAddressPostion[0]=tmp>>16;
   	WriteAddressPostion[1]=tmp>>8;
   	WriteAddressPostion[2]=tmp;  
   }
   else
   {
   	WriteAddressPostion[0]=tmp>>16;
   	WriteAddressPostion[1]=tmp>>8;
   	WriteAddressPostion[2]=tmp; 
   }
   SST25VF016B_Write(WriteAddressPostion,0,3)  ;//µØÖ·0---2´æ´¢µ±Ç°flashÖĞ¿ÉÒÔÓÃµØÖ·¿Õ¼äµÄÊ×µØÖ·
   SST25VF016B_Write( pBuffer, WriteAddr, NumByteToWrite)  ; 	//WriteAddrµØÖ··¶Î§0x000003---0x1ffffff
    
}
/****************************************************************************
* Ãû    ³Æ£ºvoid FlashReadID(void)
* ¹¦    ÄÜ£º¶Á¹¤³§Âë¼°ĞÍºÅµÄº¯Êı
* Èë¿Ú²ÎÊı£º  
* ³ö¿Ú²ÎÊı£ºÎŞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎŞ 
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
	dev_id= SPISend(0x00);;	              //41H: Æ÷¼şĞÍºÅSST25VF016B     
	dev_id= SPISend(0x00);;	              //41H: Æ÷¼şĞÍºÅSST25VF016B   	
	UnselectDevice;
}
