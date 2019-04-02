#ifndef __OLED_H
#define __OLED_H

/* includes */
#include "stm32l1xx.h"
#include "stm32l1xx_conf.h"



#define     OLED_RST_L                      GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define     OLED_RST_H                      GPIO_SetBits(GPIOB, GPIO_Pin_12)


#define	    OLED_DC_L                       GPIO_ResetBits(GPIOC, GPIO_Pin_6);
#define     OLED_DC_H                       GPIO_SetBits(GPIOC, GPIO_Pin_6);

#define  	OLED_SCK_PIN					GPIO_Pin_13
#define		OLED_SDA_PIN					GPIO_Pin_15

//#define Select_OLED       GPIO_ResetBits(GPIOB, GPIO_Pin_0);
//#define NotSelect_OLED    GPIO_SetBits(GPIOB, GPIO_Pin_0);
   
/* OLED_WriteByte */
void OLED_WB(uint8_t data);
void LCD_Init(void);
void LCD_CLS(void);
void LCD_Fill(unsigned char dat);

#endif
