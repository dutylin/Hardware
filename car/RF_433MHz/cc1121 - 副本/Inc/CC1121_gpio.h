#ifndef __DK_SPI_h__
#define __DK_SPI_h__
#include "stm32f0xx_hal.h"
//#include "DK_PIC16_App.h"

#define u8     unsigned char
#define word   unsigned int
#define HARD_spi 1

//-----------------------------------------------------------------------------
// define MCU GPIO
//-----------------------------------------------------------------------------


#define RF_RST       	GPIO_PIN_2
//#define nIRQ1         	GPIO_PIN_3   //  0    1     0    RFMxx nIRQ1
//#define nIRQ0         	GPIO_PIN_2   //  0    1     0    RFMxx nIRQ0


#define nCS   			GPIO_PIN_4
#define SCK           		GPIO_PIN_5
#define MOSI          	GPIO_PIN_7
#define MISO          	GPIO_PIN_6 //INPUT


#define RFM95_PORchkout_SET()    	 HAL_GPIO_WritePin(GPIOA, RF_RST, GPIO_PIN_SET)
#define RFM95_PORchkout_RESET()     HAL_GPIO_WritePin(GPIOA, RF_RST, GPIO_PIN_RESET)


#define nCS_SET()   				 HAL_GPIO_WritePin(GPIOA, nCS, GPIO_PIN_SET)
#define nCS_RESET()				 HAL_GPIO_WritePin(GPIOA, nCS, GPIO_PIN_RESET)

#define SCK_SET()            			HAL_GPIO_WritePin(GPIOA, SCK, GPIO_PIN_SET)
#define SCK_RESET()          			HAL_GPIO_WritePin(GPIOA, SCK, GPIO_PIN_RESET)

#define MOSI_SET()           			HAL_GPIO_WritePin(GPIOA, MOSI, GPIO_PIN_SET)
#define MOSI_RESET()         			HAL_GPIO_WritePin(GPIOA, MOSI, GPIO_PIN_RESET)

#define MISO_READ()          			HAL_GPIO_ReadPin(GPIOA, MISO) //INPUT

void si4463_main(void);
void SPICmd8bit(u8 WrPara);
u8 SPIRead(u8 adr);
u8 SPIRead8bit(void);
void SPIWrite(word WrPara);
void SPIBurstRead(u8 adr, u8 *ptr, u8 length);
void BurstWrite(u8 adr, u8 *ptr, u8 length);
void Si4463_GpioInt(void);
void BurstWrite_RF_TX(u8 adr, u8 *data_head ,u8 *ptr, u8 length);
void SPIBurstRead_RX(u8 adr);
void IO_TEST(void);
#endif







