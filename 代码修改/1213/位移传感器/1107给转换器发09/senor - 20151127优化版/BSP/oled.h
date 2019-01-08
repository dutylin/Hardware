#ifndef __OLED_H
#define __OLED_H
#include "stm32l1xx.h"

#define     RCC_APB2Periph_OLED_PORT        RCC_APB2Periph_GPIOB

#define     OLED_PORT                       GPIOA

#define     OLED_RST_PIN                    GPIO_Pin_12

#define     OLED_RST_L                      GPIO_ResetBits(GPIOB, OLED_RST_PIN)
#define     OLED_RST_H                      GPIO_SetBits(GPIOB, OLED_RST_PIN)

#define     OLED_DC_PIN                     GPIO_Pin_14

#define	    OLED_DC_L                       GPIO_ResetBits(GPIOB, OLED_DC_PIN);
#define     OLED_DC_H                       GPIO_SetBits(GPIOB, OLED_DC_PIN);

#define  	OLED_SCK_PIN					GPIO_Pin_13 //对应SLK
#define		OLED_SDA_PIN					GPIO_Pin_15 //对应MOSI

void OLED_WB(uint8_t data);
void OLED_DLY_ms(unsigned int ms);
void delay_us(uint16_t time);
void OLED_CLS(void);

void OLED_WR_Byte(uint8_t dat,uint8_t cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);

void OLED_Init(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p);
void SPI_OLED_Init(void);


#endif
