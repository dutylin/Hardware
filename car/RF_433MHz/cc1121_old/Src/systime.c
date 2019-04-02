#include "systime.h"


extern __IO uint32_t uwTick;
uint32_t TimingDelay;


//void delay_us(__IO uint32_t nTime)
//{ 
//  TimingDelay = uwTick;
//  while((uwTick-TimingDelay)< nTime);
//}

void delay_us(uint32_t time)
{    
   unsigned int i=0;  
   while(time--)
   {
      i=8;  //????
      while(i--) ;    
   }
}
