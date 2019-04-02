#ifndef _SYS_H
#define _SYS_H
#include "stm32f1xx_hal.h"
#define u8 unsigned char 
#define u16 unsigned int
#define u32 uint32_t



typedef const uint32_t uc32;  
typedef const uint16_t uc16;  
typedef const uint8_t uc8; 

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  
typedef __I uint16_t vuc16; 
typedef __I uint8_t vuc8; 
#endif
