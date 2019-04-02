#ifndef __UTIL_SPI_H__
#define __UTIL_SPI_H__

#include  "xz_spi_config.h"
#include "stm32f0xx_hal.h"

/* USER CODE BEGIN Includes */
#include "stm32f0xx.h"
#include "main.h"

#define RF_SPI_NSS   GPIO_PIN_4
#define RF_SPI_SCK   GPIO_PIN_5
#define RF_SPI_MISO  GPIO_PIN_6
#define RF_SPI_MOSI  GPIO_PIN_7
#define RF_SPI_RST   GPIO_PIN_2
#define RF_DATA_IRQ  GPIO_PIN_3

#define bool _Bool
#define true 1
#define false 0

#define uint8 unsigned char
#define uint32 uint32_t
#define uint16 uint16_t
/*!
 *******************************************************************************
 @brief  SPI 4线IO口初始化，包括CS,MISO,MOSI,SCLK
 @param  void  无
 @return void  无
 *******************************************************************************
 */
void UTIL_SPI_4W_Init(void);

/*!
 *******************************************************************************
 @brief  SPI 3线移位输出，读取一个字节，不包括CS操作
 @param  uint8 data  SPI移位输出的数据
 @return uint8 返回SPI移位读取的数据
 *******************************************************************************
 */
uint8 UTIL_SPI_3W_ReadWriteUint8(uint8 data);


/*!
*******************************************************************************
@brief  SPI 3线移位输出，不包括CS操作
@param  uint8 data  SPI移位输出的数据
@return void
*******************************************************************************
*/
#define UTIL_SPI_3W_WriteUint8(x) UTIL_SPI_3W_ReadWriteUint8(x)


/*!
*******************************************************************************
@brief  SPI 3线读取一个字节，不包括CS操作
@param  void  无
@return uint8 返回SPI移位读取的数据
*******************************************************************************
*/
#define UTIL_SPI_3W_ReadUint8()  UTIL_SPI_3W_ReadWriteUint8(0xFF)


/*!
*******************************************************************************
@brief  SPI 3线写入NByte
@param  const uint8 *data  SPI写入数据的起始地址
@param  uint8 size         SPI写入数据长度
@return void               无
*******************************************************************************
*/
void UTIL_SPI_3W_WriteNByte(const uint8 *data,uint8 size);

/*!
*******************************************************************************
@brief  SPI 3线读取NByte
@param  uint8 *data      SPI读取数据存储起始地址
@param  uint8 size       SPI读取数据长度
@return void             无
*******************************************************************************
*/
void UTIL_SPI_3W_ReadNByte(uint8 *data,uint8 size);

/*!
 *******************************************************************************
 @brief  SPI移位输出输入前，CS下降沿，包括CS下降沿后可能需要的等待操作
 @param  void 无
 @return void 无
 *******************************************************************************
 */
void UTIL_SPI_CS_Begin(void);

/*!
 *******************************************************************************
 @brief  SPI移位输出输入后，CS上升沿，包括CS上升沿前后可能需要的等待操作
 @param  void 无
 @return void 无
 *******************************************************************************
 */
void UTIL_SPI_CS_End(void);

/*!
 *******************************************************************************
 @brief  SPI 3线移位输出，读取一个字节，包括CS操作
 @param  uint8 data  SPI移位输出的数据
 @return uint8 返回SPI移位读取的数据
 *******************************************************************************
 */
uint8 UTIL_SPI_4W_ReadWriteUint8(uint8 data);


/*!
*******************************************************************************
@brief  SPI 4线写入NByte
@param  const uint8 *data  SPI写入数据的起始地址
@param  uint8 size         SPI写入数据长度
@return void               无
*******************************************************************************
*/
void UTIL_SPI_4W_WriteNByte(const uint8 *data,uint8 size);


/*!
*******************************************************************************
@brief  SPI 4线读取NByte
@param  uint8 *data      SPI读取数据存储起始地址
@param  uint8 size       SPI读取数据长度
@return void             无
*******************************************************************************
*/
void UTIL_SPI_4W_ReadNByte(uint8 *data,uint8 size);


/*!
*******************************************************************************
@brief  SPI SCLK空闲
@param  void  无
@return void  无
*******************************************************************************
*/
void UTIL_SPI_SCLK_InActive(void);



void  BOARD_CC1121_GPIO_Init(void);
void   RF_RECIVE_DATA_GPIO_Init(void);






#endif
