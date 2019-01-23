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

extern SPI_HandleTypeDef hspi1;

//以下是SPI模块的初始化代码，配置成主机模式 						  
//SPI口初始化
//这里针是对SPI1的初始化
void SPI1_Init(void)
{
 W25QXX_CS(1);
    //SPI1_ReadWriteByte(0Xff);                           //启动传输
}

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
unsigned char SPI1_ReadWriteByte(unsigned char TxData)
{
    unsigned char Rxdata;
    HAL_SPI_TransmitReceive(&hspi1,&TxData,&Rxdata,1, 10);       
 	return Rxdata;          		    //返回收到的数据		
}


void W25QXX_CS(unsigned char flag)
{
  switch(flag)
  {
      case 0:   
      {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
      }break;
      case 1:
      {
         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
     
      } break;  
  }


}


