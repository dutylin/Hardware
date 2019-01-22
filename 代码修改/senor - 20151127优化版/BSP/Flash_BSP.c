#include "main.h"
#include "Flash_BSP.h"
#include "stm32l1xx.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//SPI驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/16/16
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//extern SPI_HandleTypeDef hspi1;

//以下是SPI模块的初始化代码，配置成主机模式 						  
//SPI口初始化
//这里针是对SPI1的初始化
void SPI1_Init(void)
{

	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1, ENABLE);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5,GPIO_AF_SPI1);//救命的语句啊
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);
	/* SPI1引脚定义  PA5--SPI1_SCK  PA6--SPI1_MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* PA8 RST  引脚定义 */
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	W25QXX_CS(1);
	/* SPI1 Config -------------------------------------------------------------*/

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //SPI_Direction_2Lines_FullDuplex;
	//SPI_InitStructure.SPI_Direction= SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	//SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	/* Enable SPI2 */
	SPI_Cmd(SPI1, ENABLE);


}

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
unsigned char SPI1_ReadWriteByte(unsigned char TxData)
{
    unsigned char Rxdata;
    while((SPI1->SR & SPI_I2S_FLAG_TXE)==(uint16_t)RESET);
    SPI1->DR =TxData;
    while((SPI1->SR & SPI_I2S_FLAG_RXNE)==(uint16_t)RESET);
    Rxdata = SPI1->DR;
	
 	return Rxdata;          		    //返回收到的数据		
}


void W25QXX_CS(unsigned char flag)
{
  switch(flag)
  {
      case 0:   
      {

	  GPIO_ResetBits(GPIOA, GPIO_Pin_8);
      }break;
      case 1:
      {
	  GPIO_SetBits(GPIOA, GPIO_Pin_8);
     
      } break;  
  }


}


