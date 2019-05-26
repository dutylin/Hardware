#ifndef LED_H
#define LED_H
#include "stm8l15x.h"
#include "stm8l15x_gpio.h"
#define LED_PORT          GPIOA

#define LED_PIN1          GPIO_Pin_2
#define LED_PIN2          GPIO_Pin_3

#define LED_RED           0X00
#define LED_GREEN         0X01
void LED_ctrl(unsigned char led_sel,bool status);
void LED_blink(unsigned char led_sel);
 void LED_Init(void);
#endif