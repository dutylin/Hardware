#ifndef __UTIL_SPI_CONFIG_H__
#define __UTIL_SPI_CONFIG_H__

#include "stm32f0xx.h"
#include "main.h"

//==============================================================================
//设置部分，对SPI接口设置只需要修改以下宏定义即可

//------------------------------------------------------------------------------
//设置SPI时钟极性
//1:SPI空闲时时,SCLK为高电平
//0:SPI空闲时时,SCLK为低电平
#define UTIL_SPI_CPOL                             0
//------------------------------------------------------------------------------
//设置SPI时钟相位
#define UTIL_SPI_CPHA                             0
//CHPA为0，SCLK第0个变沿（即CS下降沿，且SPI从机就绪）时，移出数据位，延时半个周期，SCLK第1个变沿时，读入数据位，延时半个周期，SCLK第二次变沿
//CHPA为1，先延时半个周期，SCLK第1个变沿时，移出数据位，延时半个周期，SCLK第2个变沿时，读入数据位 

//--------------------------------------------------------------------------------------------------------
//设置SPI移出是最高位，还是最低位
#define UTIL_SPI_MOST_BIT_FIRST                   1    


//------------------------------------------------------------------------------
//SPI IO口操作
#define UTIL_SPI_CS_LOW()         HAL_GPIO_WritePin(GPIOA,RF_SPI_NSS,GPIO_PIN_RESET)    
#define UTIL_SPI_CS_HIGH()        HAL_GPIO_WritePin(GPIOA,RF_SPI_NSS,GPIO_PIN_SET)     

#define UTIL_SPI_SCLK_LOW()       HAL_GPIO_WritePin(GPIOA,RF_SPI_SCK,GPIO_PIN_RESET) 
#define UTIL_SPI_SCLK_HIGH()      HAL_GPIO_WritePin(GPIOA,RF_SPI_SCK,GPIO_PIN_SET) 

#define UTIL_SPI_MISO_READ()      HAL_GPIO_ReadPin(GPIOA,RF_SPI_MISO)

#define UTIL_SPI_MOSI_LOW()       HAL_GPIO_WritePin(GPIOA,RF_SPI_MOSI,GPIO_PIN_RESET)                  
#define UTIL_SPI_MOSI_HIGH()      HAL_GPIO_WritePin(GPIOA,RF_SPI_MOSI,GPIO_PIN_SET)                  

//------------------------------------------------------------------------------
//SPI IO操作延时，或者其他等待

//SPI CS下降沿后，等待，通常等待SPI外色就绪
//注意：对于RF芯片，CS为低后，必须等待MISO（即GPIO0)为低，才能进一步操作
#define UTIL_SPI_CS_POST_FALL_DELAY()   //while(UTIL_SPI_MISO_READ())  //等待MISO为低

//SPI CS上升沿前，等待
#define UTIL_SPI_CS_PREV_RISE_DELAY()    //do{asm("nop");asm("nop");asm("nop");asm("nop");}while(0)

//SPI CS上升沿后，等待
#define UTIL_SPI_CS_POST_RISE_DELAY()   //do{asm("nop");asm("nop");asm("nop");asm("nop");}while(0)

//SPI SCLK上身沿前，等待
#define UTIL_SPI_SCLK_DELAY()          // do{asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");}while(0)//do{_delay_us(1);}while(0)


#define UTIL_SPI_CS_PREV_BEGIN()
#define UTIL_SPI_CS_POST_END()

#endif
