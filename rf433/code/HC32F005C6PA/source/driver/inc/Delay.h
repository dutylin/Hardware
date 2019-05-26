#ifndef __DELAY_H
  #define __DELAY_H

#include "stm8l15x.h"

void Tim4_init(void);
void Delay_us(uint16_t nCount);
void Delay_ms(uint16_t nCount);


#endif 