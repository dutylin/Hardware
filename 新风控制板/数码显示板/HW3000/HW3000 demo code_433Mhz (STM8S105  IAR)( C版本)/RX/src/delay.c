#include<stdint.h>

void m928_delay(uint16_t time_delay)
{
 uint16_t i,j;
 for(i=time_delay;i>0;i--)
 {
   for(j=3200;j>0;j--);
 }
}