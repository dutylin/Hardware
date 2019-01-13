////////////////////////////////////////////////////////////////////////
//                         SST25VF016B操作函数
//1.默认使用SPI1进行通信，时钟上边沿锁存，需提前进行初始化
//2.STM32库版本v3.5
//3.片选管脚NSS需提前初始化为高电平，同时更改SelectDevice、UnselectDevice定义
////////////////////////////////////////////////////////////////////////

#include "stm32l1xx.h"
#include "sst25vf016b.h"
typedef unsigned char  u8;
typedef unsigned int    u16;
typedef uint32_t         u32;
extern SPI_HandleTypeDef hspi1;
#define SelectDevice  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
#define UnselectDevice HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,GPIO_PIN_SET);
////////////SPI2接口配置/////////////////////////////////////////////////////
void FLASH_SPI_Configuration(void)
{

//		  SPI_InitTypeDef  SPI_InitStructure;
//	  GPIO_InitTypeDef GPIO_InitStructure;
//    //SPI_Rcc_Configuration();
//	  RCC_APB1PeriphClockCmd( RCC_APB2Periph_SPI1, ENABLE);
//	  RCC_APB1PeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
//	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7|GPIO_Pin_6 ;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
//	  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//	  GPIO_Init(GPIOA, &GPIO_InitStructure);



//	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
//	  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//	  GPIO_Init(GPIOA, &GPIO_InitStructure);
//	  
//  	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4  ;
//	  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
//				GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
//	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
//	  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//	  GPIO_Init(GPIOC, &GPIO_InitStructure);  
//	  GPIO_SetBits(GPIOC, GPIO_Pin_4);
//	  /* SPI2 Config -------------------------------------------------------------*/
//	
//	  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //SPI_Direction_2Lines_FullDuplex;
//		//SPI_InitStructure.SPI_Direction= SPI_Direction_2Lines_FullDuplex;
//	  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
//	  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
//	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
//	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
//	  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
//		//SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
//	  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//	  SPI_InitStructure.SPI_CRCPolynomial = 7;
//	  SPI_Init(SPI1, &SPI_InitStructure);

//	  SPI_Cmd(SPI1, ENABLE);
}
u8 SPISend(u8 data)
{
	uint8_t RxData;

	HAL_SPI_TransmitReceive(&hspi1,&data, &RxData, 1, 10);
	return RxData;
}

////////////////////////////////////////////////////
//读取状态寄存器值
//返回读取值
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
//忙检测
//返回忙状态，0：空闲，!0：忙
u8 SST25VF016B_IsBusy(void)
{
	u8 Busy,Register;
	Register=SST25VF016B_ReadReg();
	Busy=Register & 0x01;
	return (Busy);
}

////////////////////////////////////////////////////////////
//读取数据
//u32 Address - 起始地址，范围：0x000000-0x1fffff
//u8 *Data - 读取值
//u32 nCount - 读取字节数
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
//写使能
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
//4KB块擦除
//u32 Address - 起始地址，范围：0-511
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
//32KB块擦除
//u32 Address - 起始地址，范围：0-63
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
//64KB块擦除
//u32 Address - 起始地址，范围：0-31
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
//整片擦除
void SST25VF016B_AllErase(void)
{
	SST25VF016B_WriteEnable();
	SelectDevice;
	SPISend(0x60);
	UnselectDevice;
	while(SST25VF016B_IsBusy());	
}

////////////////////////////////////////////////////
//写入数据
//u32 Address - 起始地址，范围：0x000000-0x1fffff
//u8 *Data - 待写值
//u32 nCount - 写入字节数
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


//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大0x1fffff)
//CHECK OK
void SST25VF016B_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=4096-WriteAddr%4096; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		SST25VF016B_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	
			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>4096)pageremain=4096; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};	    
} 


//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)						
//NumByteToWrite:要写入的字节数(最大0x1fffff)   
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
	 	secpos=WriteAddr/4096;//扇区地址  
		secoff=WriteAddr%4096;//在扇区内的偏移
		secremain=4096-secoff;//扇区剩余空间大?
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		SST25VF016B_Read(SST25VF016B_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(SST25VF016B_BUFFER[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			SST25VF016B_4KBErase(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				SST25VF016B_BUF[i+secoff]=pBuffer[i];	  
			}
			SST25VF016B_Write_NoCheck(SST25VF016B_BUFFER,secpos*4096,4096);//写入整个扇区  

		}else SST25VF016B_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移	   
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
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
   	 WriteAddressPostion[2]==0xff)//判断该地址空间是否被写过
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
   SST25VF016B_Write(WriteAddressPostion,0,3)  ;//地址0---2存储当前flash中可以用地址空间的首地址
   SST25VF016B_Write( pBuffer, WriteAddr, NumByteToWrite)  ; 	//WriteAddr地址范围0x000003---0x1ffffff
    
}
/****************************************************************************
* 名    称：void FlashReadID(void)
* 功    能：读工厂码及型号的函数
* 入口参数：  
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
unsigned char fac_id,dev_id;

void FlashReadID(unsigned char fac_id,unsigned char dev_id1,unsigned char dev_id2)
{
 // while(SST25VF016B_IsBusy());
  SelectDevice;
  SPISend(0x9f);
	//SPISend(0x00);
	//SPISend(0x00);
	//SPISend(0x00);
  fac_id= SPISend(0x00);;		          //BFH: 工程码SST
	dev_id= SPISend(0x00);;	              //41H: 器件型号SST25VF016B     
	dev_id= SPISend(0x00);;	              //H: 器件型号SST25VF016B   	
	UnselectDevice;
}
