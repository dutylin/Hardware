#ifndef LED_H_
#define LED_H_
#include "stm32f4xx_hal.h"

#define SEL_PWR_LED		0

#define SEL_LTE_LED   1

#define SEL_GPS_LED   2

#define SEL_RF_LED    3


#define LTE_LED_ON 		0
#define LTE_LED_OFF 	1

#define GPS_LED_ON 		0
#define GPS_LED_OFF 	1

#define RF_LED_ON 		0
#define RF_LED_OFF 	1

#define PWR_LED_ON 		0
#define PWR_LED_OFF 	1
void LED_ctrl(unsigned char sel_led,unsigned char flag);
void LED_all_ctrl(unsigned char flag);
#endif
