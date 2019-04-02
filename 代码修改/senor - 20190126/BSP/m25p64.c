#include "M25p64.h"
//#include "spi.h"
#include "stm32l1xx.h"
#include "Flash_BSP.h"
#include "global.h"
#include "flash.h"
//*+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+*
//项目名称:M25P64芯片驱动
//作用说明:外部FLASH存储数据
//硬件说明:STM32F429控制芯片平台
//版本时间:2016/09/07-?
//开发人员:吴荣山@Halo
//*+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+**+_+*



uint32_t M25PXX_TYPE=0x202017;	//默认是M25P64
uint32_t WriteAddressPostion;

//------------------------------------------------------------------------------------------------------------------------------
/************************************************************
*函数名称:void M25PXX_Init(void)
*功    能:M25P64初始化函数
*说    明:主要是初始化IO和SPI1通信
*输入参数:无
*输出参数:无
*************************************************************/
uint32_t tmp;
void M25PXX_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_4);
	 SPI1_Init();

	//SPI1_ReadWriteByte(0Xff);                           //启动传输
	//W25QXX_CS(1);			                //SPI FLASH不选中
	while(tmp!=M25P32)
 		tmp=M25PXX_ReadID();
	//SPI1_Init();		   			        //初始化SPI
	//SPI1_SetSpeed(SPI_BAUDRATEPRESCALER_2); //设置为45M时钟,高速模式
	//M25PXX_TYPE=M25PXX_ReadID();	        //读取FLASH 24位ID.
}

/************************************************************
*函数名称:unsigned char M25PXX_ReadSR(void)
*功    能:读取M25PXX的状态寄存器
*说    明:无
*输入参数:无
*输出参数:byte=返回读取的状态数据
*************************************************************/
//unsigned char byte=0x01;
unsigned char M25PXX_ReadSR(void)
{
	unsigned char byte=0;
	W25QXX_CS(0);                             //使能器件
	SPI1_ReadWriteByte(M25X_RDSR);          //发送读取状态寄存器命令
	do
	{
	  /* Send a dummy byte to generate the clock needed by the FLASH
	  and put the value of the status register in FLASH_Status variable */
	  byte=SPI1_ReadWriteByte(Dummy_Byte);    //读取一个字节
	
	}
	while ((byte & WIP_Flag) == 0x01); /* Write in progress */

	//printf("芯片状态:%d \r\n",byte); //显示芯片的状态
	W25QXX_CS(1);							//取消片选
  
	return byte;
}
/************************************************************
*函数名称:void M25PXX_Write_SR(unsigned char sr)
*功    能:写M25PXX状态寄存器
*说    明:无
*输入参数:sr=要写入的状态
*输出参数:无
*************************************************************/
void M25PXX_Write_SR(unsigned char sr)
{
	W25QXX_CS(0);                            //使能器件
	SPI1_ReadWriteByte(M25X_WRSR);          //发送写取状态寄存器命令
	SPI1_ReadWriteByte(sr);                 //写入一个字节
	W25QXX_CS(1);                            //取消片选
}
/************************************************************
*函数名称:void M25PXX_Wait_Busy(void)
*功    能:等待芯片空闲
*说    明:无
*输入参数:无
*输出参数:无
*************************************************************/
void M25PXX_Wait_Busy(void)
{
	M25PXX_ReadSR();   // 等待空闲
}
/************************************************************
*函数名称:void M25PXX_Write_Enable(void)
*功    能:M25PXX写使能
*说    明:每次写入数据都要将WEL置位一下，否则不能写
*输入参数:无
*输出参数:无
*************************************************************/
void M25PXX_Write_Enable(void)
{
	W25QXX_CS(0);                             //使能器件
	SPI1_ReadWriteByte(M25X_WREN);          //发送写使能
	//W25QXX_CLK(1);

	W25QXX_CS(1);                              //取消片选
	//W25QXX_SDO(0);
	//delay_us(100);
}

/************************************************************
*函数名称:void M25PXX_Write_Disable(void)
*功    能:M25PXX写禁止
*说    明:将WEL清零
*输入参数:无
*输出参数:无
*************************************************************/
void M25PXX_Write_Disable(void)
{
	W25QXX_CS(0);                            //使能器件
	SPI1_ReadWriteByte(M25X_WRDI); 		    //发送写禁止指令
	W25QXX_CS(1);                            //取消片选
}
/************************************************************
*函数名称:void M25PXX_Erase_Sector(uint32_t Dst_Addr)
*功    能:擦除一个扇区
*说    明:擦除一个扇区的最少时间:150ms
*输入参数:Dst_Addr:扇区地址 根据实际容量设置
*输出参数:无
*************************************************************/
uint32_t ttt;
void M25PXX_Erase_Sector(uint32_t Dst_Addr)
{
	//unsigned char addr_tmp;
	//监视falsh擦除情况,测试用
	//printf("fe:%x\r\n",Dst_Addr);
	Dst_Addr*=M25PXX_SECTOR_BYTES_SIZE; //扇区*页(64K)
	//M25PXX_Wait_Busy();
	M25PXX_Write_Enable();            //写使能
	W25QXX_CS(0);                          //使能器件
	SPI1_ReadWriteByte(M25X_SE);   			//发送扇区擦除指令
	SPI1_ReadWriteByte((Dst_Addr&0x00FF0000)>>16);  //发送24bit地址
	SPI1_ReadWriteByte((Dst_Addr&0x0000FF00)>>8);
	SPI1_ReadWriteByte((Dst_Addr&0x000000FF));
	W25QXX_CS(1);                          //取消片选
	//M25PXX_Write_Disable();
	delay_ms(1) ;
	M25PXX_Wait_Busy();   				    //等待芯片空闲
}
/************************************************************
*函数名称:void M25PXX_Erase_Chip(void)
*功    能:擦除整个芯片
*说    明:等待时间超长...
*输入参数:无
*输出参数:无
*************************************************************/
void M25PXX_Erase_Chip(void)
{
	M25PXX_Write_Enable();                  //写使能
	W25QXX_CS(0);                            //使能器件
	SPI1_ReadWriteByte(M25X_BE);            //发送片擦除命令
	W25QXX_CS(1);
	//delay_ms(200) ;                             //取消片选
	M25PXX_Wait_Busy();   				    //等待芯片擦除结束
}
/*****************************************************************************
*函数名称:void M25PXX_Read(unsigned char* pBuffer,uint32_t ReadAddr,unsigned int NumByteToRead)
*功    能:读取SPI FLASH
*说    明:在指定地址开始读取指定长度的数据
*输入参数:pBuffer:数据存储区，ReadAddr:开始读取的地址(24bit)，
		 NumByteToRead:要读取的字节数(最大65535)
*输出参数:无
******************************************************************************/
uint32_t read_count;
unsigned char M25PXX_Read_Length(unsigned int* pBuffer)
{
        unsigned char pb[4];
	W25QXX_CS(0);                            //使能器件
	SPI1_ReadWriteByte(M25X_READ);      	//发送读取命令
	SPI1_ReadWriteByte((0 & 0x00FF0000)>>16);   //发送24bit地址
	SPI1_ReadWriteByte((0& 0x0000FF00)>>8);
	SPI1_ReadWriteByte(0& 0x000000FF);
	pb[0]=SPI1_ReadWriteByte(Dummy_Byte);    //循环读数据
	pb[1]=SPI1_ReadWriteByte(Dummy_Byte);    //循环读数据
	pb[2]=SPI1_ReadWriteByte(Dummy_Byte);    //循环读数据
	pb[3]=SPI1_ReadWriteByte(Dummy_Byte);    //循环读数据
	W25QXX_CS(1);

	if(pb[0]==0xff&&pb[1]==0xff&&pb[2]==0xff&&pb[3]==0xff)
	{
	   // return 0;
		return 0;
	}
	else
	{
		*pBuffer=pb[0]<<24+pb[1]<<16+pb[2]<<8+pb[3];
		return 1;

	}


}
unsigned char M25PXX_Read(unsigned char* pBuffer,uint32_t ReadAddr,uint32_t NumByteToRead)
{
	uint32_t i;
	W25QXX_CS(0);                            //使能器件
	SPI1_ReadWriteByte(M25X_READ);      	//发送读取命令
	SPI1_ReadWriteByte((ReadAddr & 0x00FF0000)>>16);   //发送24bit地址
	SPI1_ReadWriteByte((ReadAddr& 0x0000FF00)>>8);
	SPI1_ReadWriteByte(ReadAddr& 0x000000FF);
	/*pBuffer[0]=SPI1_ReadWriteByte(Dummy_Byte);    //循环读数据
	pBuffer[1]=SPI1_ReadWriteByte(Dummy_Byte);    //循环读数据
	pBuffer[2]=SPI1_ReadWriteByte(Dummy_Byte);    //循环读数据
	pBuffer[3]=SPI1_ReadWriteByte(Dummy_Byte);    //循环读数据*/
	/*if(pBuffer[0]==0xff&&pBuffer[1]==0xff&&pBuffer[2]==0xff&&pBuffer[3]==0xff)
	{
	   // return 0;
	
	    NumByteToRead=0;
		for(i=4; i<NumByteToRead; i++)
		{
			read_count=i;
			pBuffer[i]=SPI1_ReadWriteByte(Dummy_Byte);    //循环读数据
		}
			W25QXX_CS(1);
		return 0;
	}
		
	else*/
	{
		//NumByteToRead= pBuffer[0]<<24+pBuffer[1]<<16+pBuffer[2]<<8+pBuffer[3];
		for(i=0; i<NumByteToRead; i++)
		{
			read_count=i;
			pBuffer[i]=SPI1_ReadWriteByte(Dummy_Byte);    //循环读数据
		}
			W25QXX_CS(1);
		return 1;

	}


}
/*****************************************************************************
*函数名称:void M25PXX_Read(unsigned char* pBuffer,uint32_t ReadAddr,unsigned int NumByteToRead)
*功    能:读取SPI FLASH
*说    明:在指定地址开始读取指定长度的数据
*输入参数:pBuffer:数据存储区，ReadAddr:开始读取的地址(24bit)，
		 NumByteToRead:要读取的字节数(最大65535)
*输出参数:无
******************************************************************************/
uint32_t read_count;
unsigned char M25PXX_Read_len(uint32_t *NumByteToRead)
{
//	uint32_t i;
	unsigned char pBuffer[4];
	W25QXX_CS(0);                            //使能器件
	SPI1_ReadWriteByte(M25X_READ);      	//发送读取命令
	SPI1_ReadWriteByte((0 & 0x00FF0000)>>16);   //发送24bit地址
	SPI1_ReadWriteByte((0& 0x0000FF00)>>8);
	SPI1_ReadWriteByte(0& 0x000000FF);
	pBuffer[0]=SPI1_ReadWriteByte(Dummy_Byte);    //循环读数据
	pBuffer[1]=SPI1_ReadWriteByte(Dummy_Byte);    //循环读数据
	pBuffer[2]=SPI1_ReadWriteByte(Dummy_Byte);    //循环读数据
	pBuffer[3]=SPI1_ReadWriteByte(Dummy_Byte);    //循环读数据
	if(pBuffer[0]==0xff&&pBuffer[1]==0xff&&pBuffer[2]==0xff&&pBuffer[3]==0xff)
	{
	   // return 0;
	
		*NumByteToRead=0;
		return 0;
	}
		
	else
	{
        *NumByteToRead=pBuffer[0]<<24+pBuffer[1]<<16+pBuffer[2]<<8+pBuffer[3];
		return 1;

	}


}

/*****************************************************************************
*函数名称:void M25PXX_Write_Page(unsigned char* pBuffer,uint32_t WriteAddr,unsigned int NumByteToWrite)
*功    能:SPI在一页(0~65535)内写入少于256个字节的数据
*说    明:在指定地址开始写入最大256字节的数据
*输入参数:pBuffer:数据存储区，WriteAddr:开始写入的地址(24bit)，
         NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
*输出参数:无
******************************************************************************/
uint32_t write_count;
void M25PXX_Write_Page(unsigned char* pBuffer,uint32_t WriteAddr,unsigned int NumByteToWrite)
{
	unsigned int i;
	//M25PXX_Wait_Busy();
	M25PXX_Write_Enable();                  //SET WEL
	//M25PXX_Wait_Busy();
	W25QXX_CS(0);                            //使能器件
	SPI1_ReadWriteByte(M25X_PP);   			//发送写页命令
	SPI1_ReadWriteByte((unsigned char)((WriteAddr)>>16)); //发送24bit地址
	SPI1_ReadWriteByte((unsigned char)((WriteAddr)>>8));
	SPI1_ReadWriteByte((unsigned char)WriteAddr);
	
		for(i=0; i<NumByteToWrite; i++)
		{
			SPI1_ReadWriteByte(pBuffer[i]); //循环写数据
			write_count = write_count+1;
		}
	
		W25QXX_CS(1);
		delay_ms(1) ;						  //取消片选
		M25PXX_Wait_Busy(); 					//等待写入结束
	}

/******************************************************************************/
void M25PXX_Write_NoCheck(unsigned char* pBuffer,uint32_t WriteAddr,uint32_t NumByteToWrite)
{
	uint32_t pageremain;
	pageremain=256-WriteAddr%256; //单页剩余的字节数
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{
		M25PXX_Write_Page(pBuffer,WriteAddr,pageremain); //写页
		if(NumByteToWrite==pageremain)break;//写入结束了
		else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};
}


/*****************************************************************************
*函数名称:void M25PXX_Write(unsigned char* pBuffer,uint32_t WriteAddr,unsigned int NumByteToWrite)
*功    能:写SPI FLASH，在指定地址开始写入指定长度的数据
*说    明:该函数带擦除操作!
*输入参数:pBuffer:数据存储区，WriteAddr:开始写入的地址(24bit)，
         NumByteToWrite:要写入的字节数(最大65535)
*输出参数:无
******************************************************************************/

	unsigned char * M25PXX_BUF;
	unsigned char M25PXX_BUFFER[1024];
void M25PXX_Write(unsigned char* pBuffer,uint32_t WriteAddr,uint32_t NumByteToWrite,u8 ease_flag)
{
	uint32_t secpos;
	uint32_t secoff;
	uint32_t secremain;
	uint32_t i;



	M25PXX_BUF=M25PXX_BUFFER;	//得到数值缓存区
	secpos=WriteAddr/M25PXX_SECTOR_BYTES_SIZE;		//扇区地址
	secoff=WriteAddr%M25PXX_SECTOR_BYTES_SIZE;		//在扇区内的偏移
	secremain=M25PXX_SECTOR_BYTES_SIZE-secoff;		//扇区剩余空间大小
	if(NumByteToWrite<=secremain)
		secremain=NumByteToWrite;//不大于64K个字节
	
	M25PXX_Read(M25PXX_BUF,secpos*M25PXX_SECTOR_BYTES_SIZE,secremain);//读出整个扇区的内容
	i=NumByteToWrite+M25PXX_BUF[0]<<24+M25PXX_BUF[1]<<16+
	M25PXX_BUF[2]<<8+M25PXX_BUF[3];
	if(i>=65535)
	{
		M25PXX_BUF[0]=NumByteToWrite>>24;
		M25PXX_BUF[1]=NumByteToWrite>>16;
		M25PXX_BUF[2]=NumByteToWrite>>8;
		M25PXX_BUF[3]=NumByteToWrite;

	}
	else
	{
		M25PXX_BUF[0]=i>>24;
		M25PXX_BUF[1]=i>>16;
		M25PXX_BUF[2]=i>>8;
		M25PXX_BUF[3]=i;

	}
     M25PXX_BUF=pBuffer;

         while(1)
	 {
		
		if(i<secremain)//需要擦除
		{
			M25PXX_Erase_Sector(secpos);//擦除这个扇区
			/*for(i=0; i<secremain; i++)	 //复制
			{
				M25PXX_BUF[i+secoff+4]=pBuffer[i];
			}*/	
			
			for(i=0; i<secremain; i++)	 //复制
			{
				M25PXX_BUF[i+secoff]=pBuffer[i];
			}		
			M25PXX_Write_NoCheck(M25PXX_BUF,secpos*M25PXX_SECTOR_BYTES_SIZE,NumByteToWrite);//写入整个扇区

		}
		else
			M25PXX_Write_NoCheck(pBuffer,WriteAddr,secremain); //写已经擦除了的,直接写入扇区剩余区间.
		if(NumByteToWrite==secremain)
			break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0

			pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移
			NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>M25PXX_SECTOR_BYTES_SIZE)
				secremain=M25PXX_SECTOR_BYTES_SIZE;	//下一个扇区还是写不完
			else
				secremain=NumByteToWrite;			//下一个扇区可以写完了
		}
	    };
}
/************************************************************
*函数名称:uint32_t M25PXX_ReadID(void)
*功    能:读取芯片ID函数
*说    明:M25p64共8M位，ID号=0x202017
*输入参数:无
*输出参数:Temp=返回的ID号
*************************************************************/
uint32_t M25PXX_ReadID(void)
{
	uint32_t Temp=0,Temp0=0,Temp1=0,Temp2=0;
	W25QXX_CS(0);
	SPI1_ReadWriteByte(M25X_RDID);		   //发送读取ID命令
	Temp0=SPI1_ReadWriteByte(Dummy_Byte);  //读一个字节
	Temp1=SPI1_ReadWriteByte(Dummy_Byte);
	Temp2=SPI1_ReadWriteByte(Dummy_Byte);
	W25QXX_CS(1);
	Temp = (Temp0<<16)|(Temp1<<8)|Temp2;   //组成一个24位数据
	//printf("M25P64读ID：0x%x！\r\n",Temp);  //M25P64的识别ID是0x202017
	return Temp;
}



//void M25PXX_Write_Test()
//{
//    unsigned char *crc_pbuffer;
//	uint32_t buff_size;
//	unsigned char j;
//	uint32_t i;
//	buff_size=1024;
//	for(i=0; i<buff_size; i++)
//	{		
//			test_tx[i]=1+j++;
//	}    

// CRC_cal(test_tx,crc_pbuffer,10);
// crc_result=CRC_decode(test_tx,12);


//	M25PXX_Write((unsigned char*)test_tx,0x00000000,buff_size);		//从倒数第100个地址处开始,写入SIZE长度的数据
//	M25PXX_Read(test_rx,0x00000000,buff_size);					//从倒数第100个地址处开始,读出SIZE个字节

//}

uint32_t Read_AddressWrite()
{
	uint32_t addr_tmp;
	//增加内部flash读取函数
	FLASH_Read(((uint32_t)0x08080000),((uint32_t)0x08080000),&addr_tmp);
	if(addr_tmp<=0x400000)
		WriteAddressPostion = addr_tmp;
	
	else
		WriteAddressPostion = 0;
		return WriteAddressPostion;
}
void SPI_FLASH_READ(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)
{
    
    M25PXX_Read( M25PXX_BUFFER,ReadAddr,NumByteToRead);
	  pBuffer = M25PXX_BUFFER;
}
u32 wraddr_count;
void SPI_FLASH_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	u32 tmp;
   Read_AddressWrite();
  if(WriteAddressPostion == 0xffffffff)	
		WriteAddressPostion = NumByteToWrite;
  else 
		WriteAddressPostion = WriteAddressPostion + NumByteToWrite;	

		#if DEBUG
			WriteAddressPostion=0;
		#endif		

		//增加内部flash写函数，保存WriteAddressPostion到内部flash中
    FLASH_Write(((uint32_t)WRITEADDR),((uint32_t)WRITEADDR),&WriteAddressPostion);	
	
	M25PXX_Write_NoCheck(pBuffer,WriteAddressPostion,NumByteToWrite); 	//WriteAddr地址范围0x000003---0x1ffffff
    
}
	
void write_all_chip()
{  
    u8 p[1];
	  u32 i;
	p[0] = 0x56;
    for(i=0;i<4194303;i++)
    {
	M25PXX_Write_NoCheck(p,i,1);
   }
}


