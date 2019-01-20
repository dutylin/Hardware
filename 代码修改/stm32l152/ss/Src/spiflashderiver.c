/*void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
  
 SPI_FLASH_Write_SR(0x02);//使能状态寄存器中的写存储器  
  
         SST25V_DBSY();  
 SPI_FLASH_Write_SR(0x02);//使能状态寄存器中的写存储器  
         SST25V_DBSY();  
实验目的：将数据写入外部FLASH中，然后再读出来显示在LCD上  
 实验平台：基于STM32F103C8T6的彩屏开发板  
FLASH：SST25VF016B  
  
 flash_SST.c  */
  
 #include "flash_SST.h"  
#include "stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "sst25vf016b.h" 

 typedef unsigned char	u8;
 typedef unsigned int	 u16;
 typedef uint32_t		  u32;
 extern SPI_HandleTypeDef hspi1;
#define SelectDevice  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
#define UnselectDevice HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,GPIO_PIN_SET);


 u8 SPIx_ReadWriteByte(u8 data)
{
	uint8_t RxData;

	HAL_SPI_TransmitReceive(&hspi1,&data, &RxData, 1, 10);
	return RxData;
} 
 //4Kbytes为一个Sector  
 //16个扇区为1个Block  
 //SST25VF016B  
 //容量为2M字节,共有32个Block(块),512个Sector（扇区）  
//初始化SPI FLASH的IO口  
//修改状态寄存器，允许芯片存储器被写  
void SPI_Flash_Init(void)  
 {  
 //RCC->APB2ENR|=1<<2;        //PORTA时钟使能          
//GPIOA->CRH&=0XFFFFFF0F;  
// GPIOA->CRH|=0X00000030;    //PA9 推挽           
//GPIOA->ODR|=1<<9;          //PA9上拉  
//SPIx_Init();               //初始化SPI  
 SPI_FLASH_Write_SR(0x02);  //使能状态寄存器中的写存储器  
SST25V_DBSY();  
 }   
  
 //读取SPI_FLASH的状态寄存器  
//BIT7  6   5   4   3   2   1   0  
 //SPR   RV  TB BP2 BP1 BP0 WEL BUSY  
 //SPR:默认0,状态寄存器保护位,配合WP使用  
//TB,BP2,BP1,BP0:FLASH区域写保护设置  
//WEL:写使能锁定  
//BUSY:忙标记位(1,忙;0,空闲)  
 //默认:0x00  
 u8 SPI_Flash_ReadSR(void)    
 {   
 u8 byte=0;    
 SelectDevice;                            //使能器件    
SPIx_ReadWriteByte(SST25_ReadStatusReg);    //发送读取状态寄存器命令     
byte=SPIx_ReadWriteByte(0Xff);             //读取一个字节   
UnselectDevice;                                 //取消片选      
return byte;    
 }  
  
 //写SPI_FLASH状态寄存器  
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!  
void SPI_FLASH_Write_SR(u8 sr)    
 {    
 SelectDevice;    //片选  
SPIx_ReadWriteByte(SST25_EnableWriteStatusReg);  //使能写状态寄存器命令    
UnselectDevice;                                                 //取消片选  
SelectDevice;                                                  //片选                          
SPIx_ReadWriteByte(SST25_WriteStatusReg);   //发送写取状态寄存器命令     
SPIx_ReadWriteByte(sr);                                //写入一个字节   
UnselectDevice;                                        //取消片选                   
 }   
  
  
 //SPI_FLASH写使能  
//将WEL置位    
void SPI_FLASH_Write_Enable(void)    
 {  
 SelectDevice;                            //使能器件    
SPIx_ReadWriteByte(SST25_WriteEnable);      //发送写使能   
UnselectDevice;                            //取消片选                 
 }  
  
 //SPI_FLASH写禁止  
//将WEL清零   
void SPI_FLASH_Write_Disable(void)    
 {   
 SelectDevice;                                       //使能器件    
SPIx_ReadWriteByte(SST25_WriteDisable);     //发送写禁止指令     
UnselectDevice;                                       //取消片选                 
 }                               
  
 //读取芯片ID SST25VF016的是 0XBF41  
 u16 SPI_Flash_ReadID(void)  
 {  
 u16 Temp = 0;        
 SelectDevice;   
      
 //发送读取ID命令                             
SPIx_ReadWriteByte(0x90);  
  
 //发送24位的地址          
SPIx_ReadWriteByte(0x00);               
 SPIx_ReadWriteByte(0x00);               
 SPIx_ReadWriteByte(0x00);  
  
 //读取返回的16位值                                
Temp=SPIx_ReadWriteByte(0xFF)<<8;  //高8位数据  
Temp+=SPIx_ReadWriteByte(0xFF);      //底八位数据  
UnselectDevice;                                     
 return Temp;  
 }  
  
 //读取SPI FLASH   
 //在指定地址开始读取指定长度的数据  
//pBuffer:数据存储区  
//ReadAddr:开始读取的地址(24bit)  
 //NumByteToRead:要读取的字节数(最大65535即64k)  
 void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)    
 {  
 u16 i;                                                                                                                  
 SelectDevice;                                       //使能器件    
SPIx_ReadWriteByte(SST25_ReadData);         //发送读取命令  
  
//发送24bit地址     
SPIx_ReadWriteByte((u8)((ReadAddr)>>16));     
 SPIx_ReadWriteByte((u8)((ReadAddr)>>8));    
 SPIx_ReadWriteByte((u8)ReadAddr);  
 for(i=0;i<NumByteToRead;i++)  
 {  
   pBuffer=SPIx_ReadWriteByte(0XFF);   //循环读数   
}  
UnselectDevice;                            //取消片选                 
 }   
  
 //地址自动增加的写数据A  
 void AutoAddressIncrement_WordProgramA(u8 Byte1, u8 Byte2, u32 Addr)  
 {  
 SPI_FLASH_Write_Enable();  
 SelectDevice;  
 SPIx_ReadWriteByte(SST25_AAI_WordProgram);  
  
 //输入所要写数据的起始地址  
SPIx_ReadWriteByte((Addr & 0xFF0000) >> 16);  
 SPIx_ReadWriteByte((Addr & 0xFF00) >> 8);  
 SPIx_ReadWriteByte(Addr & 0xFF);                                  //发送最初的两个数据  
SPIx_ReadWriteByte(Byte1);  
 SPIx_ReadWriteByte(Byte2);  
 UnselectDevice;  
 SPI_Flash_Wait_Busy();  
 }  
  
 //地址自动增加的写数据B  
 void AutoAddressIncrement_WordProgramB(u8 state,u8 Byte1, u8 Byte2)  
 {  
 SPI_FLASH_Write_Enable();  
 SelectDevice;  
 SPIx_ReadWriteByte(SST25_AAI_WordProgram);  
 SPIx_ReadWriteByte(Byte1);  
 SPIx_ReadWriteByte(Byte2);  
 UnselectDevice;  
 SPI_Flash_Wait_Busy();  
 if(state==1)  
 {  
   SPI_FLASH_Write_Disable();  
 }  
 SPI_Flash_Wait_Busy();  
 }  
  
 //结合AB构成的地址自动增加的连续数据的写入  
//具有先擦除待写区域的功能  
//pBuffer：为待写数据组  
//WriteAddr：所写数据的起始地址  
//NumByteToWrite：所要写的数据的长度  
void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)  
 {  
 u16 i,temp;  
 u32 secpos;  
 u16 secoff;  
 u16 secremain;       
 //以下代码为擦除待写区域的代码  
secpos=WriteAddr/4096;                                           //扇区（4K）地址0~511 for     SST25VF016  
 secoff=WriteAddr@96;                                                             //在扇区内的偏移  
secremain=4096-secoff;                                                           //扇区剩余空间大小  
if(NumByteToWrite<secremain)                                                  //剩余空间大于所存数据  
{  
   temp=1;  
 }  
 else                                                                 //剩余空间小于所存数据  
{  
   i=NumByteToWrite-secremain;                           //判断还占了几个扇区  
if(i%96==0)  
        temp=i/4096+1;  
 else  
        temp=i/4096+2;  
 }  
 for(i=0;i<temp;i++)  
 {  
   SPI_Flash_Erase_Sector((secpos+i)*4096);                   //擦除将要写入数据的扇区     
 }  
   //以下代码为将数据写入指定地址的代码  
if(NumByteToWrite%2==0)  
 {  
   temp=NumByteToWrite/2-1;  
 }  
 else  
 {  
   temp=NumByteToWrite/2;  
 }  
 AutoAddressIncrement_WordProgramA(pBuffer[0], pBuffer[1],WriteAddr );                                        //开始写数据  
for(i=1;i<temp;i++)  
 {  
   AutoAddressIncrement_WordProgramB(0,pBuffer[2*i], pBuffer[2*i+1]);  
 }  
 if(NumByteToWrite%2==0)  
 {  
   AutoAddressIncrement_WordProgramB(1,pBuffer[NumByteToWrite-2], pBuffer[NumByteToWrite-1]);   //结束写数据  
}  
else  
 {  
   AutoAddressIncrement_WordProgramB(1,pBuffer[NumByteToWrite-1],0);                                         //结束写数据  
}  
  
 //写入1Byte数据  
//pBuffer:待写的数据  
//WriteAddr：待写数据的地址  
void Flash_WriteByte(u8* pBuffer,u32 WriteAddr)  
 {  
 u32 secpos;  
 secpos=WriteAddr/4096;                                   //扇区地址 0~511 for w25x16  4096=4k  
 SPI_Flash_Erase_Sector(secpos);                        //擦除这个扇区  
SPI_FLASH_Write_Enable();                              //SET WEL  
 SelectDevice;                                            //使能器件    
SPIx_ReadWriteByte(SST25_ByteProgram );       //发送写页命令  
  
//发送24bit地址     
SPIx_ReadWriteByte((u8)((WriteAddr)>>16));    
 SPIx_ReadWriteByte((u8)((WriteAddr)>>8));    
 SPIx_ReadWriteByte((u8)WriteAddr);                  //发送待写的数据  
SPIx_ReadWriteByte(pBuffer[0]);  
 UnselectDevice;   
 SPI_Flash_Wait_Busy();                                     //等待写完成  
}  
  
 //擦除整个芯片  
//整片擦除时间:  
 //W25X16:25s  
 //W25X32:40s  
 //W25X64:40s  
 //等待时间超长...  
void SPI_Flash_Erase_Chip(void)    
 {                                              
 SPI_FLASH_Write_Enable();                          //SET WEL  
 SPI_Flash_Wait_Busy();    
 SelectDevice;                                       //使能器件    
SPIx_ReadWriteByte(SST25_ChipErase);       //发送片擦除命令   
UnselectDevice;                                     //取消片选                 
SPI_Flash_Wait_Busy();                              //等待芯片擦除结束  
}  
  
 //擦除一个扇区  
//Dst_Addr:扇区地址 0~511 for w25x16  
 //擦除一个山区的最少时间:150ms  
 void SPI_Flash_Erase_Sector(u32 Dst_Addr)    
 {    
 SPI_FLASH_Write_Enable();                                       //SET WEL             
 SPI_Flash_Wait_Busy();    
 SelectDevice;                                                   //使能器件    
SPIx_ReadWriteByte(SST25_4KByte_BlockERASE);      //发送扇区擦除指令  
SPIx_ReadWriteByte((u8)((Dst_Addr)>>16));            //发送24bit地址     
SPIx_ReadWriteByte((u8)((Dst_Addr)>>8));    
 SPIx_ReadWriteByte((u8)Dst_Addr);   
 UnselectDevice;                                            //取消片选                 
SPI_Flash_Wait_Busy();                                    //等待擦除完成  
}   
  
 //等待空闲  
void SPI_Flash_Wait_Busy(void)    
 {    
 while ((SPI_Flash_ReadSR()&0x01)==0x01);       // 等待BUSY位清空  
}  
  
void SST25V_EBSY(void)  
 {  
 SelectDevice;  
 SPIx_ReadWriteByte( SST25_EBSY);  
 UnselectDevice;     
 }  
  
 void SST25V_DBSY(void)  
 {  
 SelectDevice;  
 SPIx_ReadWriteByte( SST25_DBSY);  
 UnselectDevice;     
 }  
  
 
 
