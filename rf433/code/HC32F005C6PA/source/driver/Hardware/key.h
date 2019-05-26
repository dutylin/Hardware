#ifndef KEY_H
#define KEY_H

#include "gpio.h"

//key gpio port
#define KEY_UP_PORT          GPIOB
#define KEY_DOWN_PORT        GPIOB
#define KEY_EAST_PORT        GPIOB
#define KEY_WEST_PORT        GPIOB
#define KEY_SOUTH_PORT       GPIOC
#define KEY_NORTH_PORT       GPIOC
#define KEY_START_PORT       GPIOA
#define KEY_STOP_PORT        GPIOB
#define KEY_BREAK_PORT       GPIOD
//key gpio pin
#define KEY_UP_PIN          GPIO_Pin_3
#define KEY_DOWN_PIN        GPIO_Pin_2
#define KEY_EAST_PIN        GPIO_Pin_1
#define KEY_WEST_PIN        GPIO_Pin_0
#define KEY_SOUTH_PIN       GPIO_Pin_6
#define KEY_NORTH_PIN       GPIO_Pin_4
#define KEY_START_PIN       GPIO_Pin_0
#define KEY_STOP_PIN        GPIO_Pin_5
#define KEY_BREAK_PIN       GPIO_Pin_0



#define INCH					0X01   //点动
#define SELFLOCK				0X02 //自锁
#define INHIBITION			0X03//相互拟制
#define NOINHIBITION		0X04//非拟制
 void KEY_Init(void);
#endif
