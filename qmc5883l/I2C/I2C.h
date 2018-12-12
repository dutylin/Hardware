#ifndef __I2C_H
#define __I2C_H
#include "stm32f0xx_hal.h"
#include "main.h"



#define IIC_SCL_SET      HAL_GPIO_WritePin(GPIOB, scl_Pin, GPIO_PIN_SET)	 //SCL
#define IIC_SCL_RESET    HAL_GPIO_WritePin(GPIOB, scl_Pin, GPIO_PIN_RESET)//SCL
#define IIC_SDA_SET      HAL_GPIO_WritePin(GPIOB, sda_Pin, GPIO_PIN_SET)	 //SCL
#define IIC_SDA_RESET    HAL_GPIO_WritePin(GPIOB, sda_Pin, GPIO_PIN_RESET)//SCL 
#define READ_SDA          HAL_GPIO_ReadPin(GPIOB, sda_Pin)//SCL // ‰»ÎSDA


void i2c_init(void);
void SDA_IN(void);
void SDA_OUT(void);
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(u8 txd);
u8 IIC_Read_Byte(void );

#endif
