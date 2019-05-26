#include "delay.h"


/* Time base configuration */
void Tim4_init(void)
{
	TIM4->IER = 0x00;
	
	/* Clear TIM4 update flag */
	TIM4_ClearFlag(TIM4_FLAG_Update);
	
	TIM4_GenerateEvent(TIM4_EventSource_Update);
	/* Enable update interrupt */
	//TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
	
	/* enable interrupts */
	enableInterrupts();
	
}

void Delay_us(uint16_t nCount)     //N usÑÓÊ±º¯Êý
  {
	Tim4_init();
	unsigned char temp;
	
	if(nCount<16)
 	{
 	temp = (nCount<<4);	//unit = 1/16 us	
 	temp -= 4;				//compensate
	TIM4_TimeBaseInit(TIM4_Prescaler_1, temp);
	TIM4_SetCounter(temp);

 	}				
   else if(nCount<128)
 	{
 	temp = (nCount<<1);	//unit = 1/2 us
 	temp -= 1;				//compensate
	TIM4_TimeBaseInit(TIM4_Prescaler_8, temp);
	TIM4_SetCounter(temp);

 	}
   else if(nCount<1024)
 	{
 	temp = (nCount>>2);	//unit = 4us
	TIM4_TimeBaseInit(TIM4_Prescaler_64, temp);
	TIM4_SetCounter(nCount);
 	}

    TIM4_Cmd(ENABLE);
    while((TIM4->SR1 & 0x01) != 0X01);
	TIM4->SR1 = 0x00;
    TIM4_DeInit();  
  }


void Delay_ms(uint16_t nCount)	
{
	uint16_t temp1=0;

	Tim4_init();	
	TIM4_TimeBaseInit(TIM4_Prescaler_128, 124);
	TIM4_SetCounter(124);
	 TIM4_Cmd(ENABLE);
	while(temp1 < nCount )
	{
          while((TIM4->SR1 & 0x01) != 0X01);
           temp1++;	
           TIM4->SR1 = 0x00;
	//TIM4_Cmd(DISABLE);	 
  	}
	TIM4_DeInit(); 
}



