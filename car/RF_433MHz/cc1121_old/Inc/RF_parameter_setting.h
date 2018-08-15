#ifndef SSCOM_PARAMETERS_SETTING_H
#define SSCOM_PARAMETERS_SETTING_H
#include "stm32f0xx_hal.h"
void my_strncpy(unsigned char *dest, unsigned char *src, unsigned char  count) ;
unsigned char my_strncmp(unsigned char *string1,unsigned char *string2,size_t count);
unsigned char setting_rf_bySSCOM(void);
void RE_Configuration(void);
#endif
