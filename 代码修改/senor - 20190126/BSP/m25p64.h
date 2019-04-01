#ifndef __M25P64_H
#define __M25P64_H
#include "stm32l1xx.h"
#define WRITEADDR	0x08080100

typedef unsigned char u8;
typedef unsigned int u16;
typedef uint32_t u32;

//#define M25P32			1
#define M25XX_DEVICE         M25P32

#if(M25XX_DEVICE == M25P32)

#define M25P32 								0X202016 //ID?
#define M25PXX_PAGE_BYTES_SIZE				256
#define M25PXX_PER_SECTOR_PAGES_SIZE		256
#define M25PXX_SECTOR_SIZE					64
#define M25PXX_SECTOR_BYTES_SIZE 			65536
#define M25PXX_MAX_ADDRESS					0x400000
#elif (M25XX_DEVICE == M25P64)

#define M25P64 	0X202017 //ID号
#define M25PXX_PAGE_BYTES_SIZE				256
#define M25PXX_PER_SECTOR_PAGES_SIZE		256
#define M25PXX_SECTOR_SIZE					128
#define M25PXX_SECTOR_BYTES_SIZE 	M25PXX_PER_SECTOR_PAGES_SIZE	* M25PXX_PAGE_BYTES_SIZE
#endif
extern uint32_t M25PXX_TYPE;					//定义M25PXX芯片型号

//#define	M25PXX_CS 		PAout(4)  		//M25PXX的片选信号

//////////////////////////////////////////////////////////////////////////////////
//指令表
#define M25X_WREN		 0x06  //写使能
#define M25X_WRDI		 0x04  //写失能
#define M25X_RDID	 	 0x9F  //读ID
#define M25X_RDSR	 	 0x05  //读状态寄存器
#define M25X_WRSR 		 0x01  //写状态寄存器
#define M25X_READ		 0x03  //读取数据
#define M25X_FAST_READ   0x0B  //快速读取数据
#define M25X_PP    	 	 0x02  //写页面 
#define M25X_SE          0xD8  //扇区擦除
#define M25X_BE			 0xC7  //整块擦除 
#define M25X_RES		 0xAB  //读取电子签名

#define WIP_Flag   0x01  //写过程读标志
#define Dummy_Byte 0xFF  //虚拟的
//--------------------------------------------------------------------------------
void M25PXX_Init(void);			    //M25P64初始化函数
unsigned char M25PXX_ReadSR(void);  			    //读取M25PXX的状态寄存器
void M25PXX_Write_SR(unsigned char sr);    	    //写M25PXX状态寄存器
void M25PXX_Wait_Busy(void);    	    //等待芯片空闲
void M25PXX_Write_Enable(void); 	    //M25PXX写使能
void M25PXX_Write_Disable(void);	    //M25PXX写禁止
void M25PXX_Erase_Sector(uint32_t Dst_Addr); //擦除一个扇区
void M25PXX_Erase_Chip(void);           //擦除整个芯片

//void M25PXX_Write_Page(unsigned char* pBuffer,uint32_t WriteAddr,uint32_t NumByteToWrite);//写一页数据
void M25PXX_Write_NoCheck(unsigned char* pBuffer,uint32_t WriteAddr,uint32_t NumByteToWrite);//无检验写SPI FLASH，具有自动换页功能
void M25PXX_Write(unsigned char* pBuffer,uint32_t WriteAddr,uint32_t NumByteToWrite,u8 ease_flag);//写SPI FLASH
unsigned char M25PXX_Read(unsigned char* pBuffer,uint32_t ReadAddr,uint32_t NumByteToRead);//读取SPI FLASH

uint32_t M25PXX_ReadID(void); //读取芯片ID函数

unsigned char M25PXX_Read_Length(unsigned int* pBuffer);

unsigned char Device_data_store(unsigned char *pbuff,uint32_t WriteAddr,uint32_t len);
unsigned char M25PXX_Read_len(uint32_t *NumByteToRead);
unsigned char Device_data_read(unsigned char *pbuff,uint32_t Raddress,uint32_t len);
unsigned  char M25PXX_Erase_Head(void);
void SPI_FLASH_Write(unsigned char* pBuffer,uint32_t WriteAddr,unsigned int NumByteToWrite);
void write_all_chip(void);
void SPI_FLASH_READ(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);
uint32_t Read_AddressWrite(void);
#endif
