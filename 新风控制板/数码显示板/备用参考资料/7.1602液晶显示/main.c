/*******************************************************************************
* File Name          : main.c
* Author             : Wuhan R&D Center, Embest
* Date First Issued  : 08/08/2008
* Description        : Main program body
********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define ADC1_DR_Address    ((u32)0x4001244C)

unsigned short int ADC_ConvertedValue;
GPIO_InitTypeDef GPIO_InitStructure;
ADC_InitTypeDef   ADC_InitStructure;
DMA_InitTypeDef   DMA_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;
ErrorStatus HSEStartUpStatus;
extern vu32 TimingDelay;





/*	  自定义变量															 */
u8 table1[]="  OKBOARD-I    ";
u8 table2[]="WWW.OKMCU.NET  ";


/* Private functions ---------------------------------------------------------*/



/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void NVIC_Configuration(void);
void GPIO_Configuration(void);
void Delay(vu32 nTime);
void SysTick_Configuration(void);
void SetupLED (void) ;	
extern void SetupADC    (void);
	
/* Private functions ---------------------------------------------------------*/

void write_com(u16 com)
{
	Delay(5);
	GPIOD->ODR = (GPIOD->ODR&0xff00)|com ;
	GPIOC->ODR &=~(1<<12 );
	GPIOB->ODR &=~(1<<9) ;
	Delay(5);
	GPIOB->ODR |=(1<<9) ;
	Delay(5);
	GPIOB->ODR &=~(1<<9) ;  	
}

void write_date(u16 date)
{
	Delay(5);
	GPIOD->ODR = (GPIOD->ODR&0xff00)|date ;
	GPIOC->ODR |=(1<<12) ;
	GPIOB->ODR &=~(1<<9) ;
	Delay(5);
	GPIOB->ODR |=(1<<9) ;
	Delay(5);
	GPIOB->ODR &=~(1<<9) ;
	
}

void init()
{
	GPIOB->CRL =  0x33333333; 
    GPIOB->CRH = 0x33333333;
	GPIOC->CRL =  0x33333333; 
    GPIOC->CRH =  0x33333333;
	GPIOD->CRL =  0x33333333; 
    GPIOD->CRH =  0x33333333;

	 //GPIOB->ODR |=(1<<13) ;
	 //GPIOB->ODR |=(1<<15) ;
	GPIOB->ODR &=~(1<<6) ;	//1602写

	//GPIOB->ODR |= (1<<8) ;//关蜂鸣器

	Delay(20);
	write_com(0x38);
	Delay(20);
	write_com(0x0f);
	Delay(20);
	write_com(0x06);
	Delay(20);
	write_com(0x01);
	Delay(20);	

}
/*********************************************************/
/*                                                       */
/* 设定显示位置                                          */
/*                                                       */
/*********************************************************/
void lcd_pos(u8 X,u8 Y)
{                          
   u8  pos;
   if (X==0)
     {X=0x80;}
   else if (X==1)
     {X=0x90;}
   else if (X==2)
     {X=0x88;}
   else if (X==3)
     {X=0x98;}
   pos = X+Y ;  
   write_com(pos);     //显示地址
}

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
u8 i,a;
#ifdef DEBUG
  debug();
#endif
   
  /* Configure the system clocks */
  RCC_Configuration();
  SysTick_Configuration();
    
  /* NVIC Configuration */
  NVIC_Configuration();

  /* Configure the GPIO ports */
  GPIO_Configuration();
  



 init();


 write_com(0x80+17);
	Delay(10);
	for(a=0;a<16;a++)
	{
	write_date(table1[a]);
	Delay(2);		
	}

	write_com(0xc0+17);
	Delay(5);
	for(a=0;a<15;a++)
	{
	write_date(table2[a]);
	Delay(4);		
	}

	for(a=0;a<16;a++)
	{
		write_com(0x18);
		Delay(100);
	}
	 Delay(800);

 

   for(;;)
   {  
  	    GPIOD->ODR = 0xffffffff;
  GPIOB->ODR = 0xffffffff;
		
	}   

}

/*******************************************************************************
* Function Name  : SysTick_Configuration
* Description    : Configures the SysTick to generate an interrupt each 1 millisecond.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Configuration(void)
{
  /* Select AHB clock(HCLK) as SysTick clock source */
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

  /* Set SysTick Priority to 3 */
  NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick, 3, 0);
   
  /* SysTick interrupt each 1ms with HCLK equal to 72MHz */
  SysTick_SetReload(72000);

  /* Enable the SysTick Interrupt */
  SysTick_ITConfig(ENABLE);
}



/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
   
  /* Enable GPIOB, GPIOC and AFIO clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | 
                         RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE);
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
  
#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
}


/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 
  /* Configure PC. as Output push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  /* Configure PD. as Output push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  /* Configure PB. as Output push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);   

}


/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nTime: specifies the delay time length, in milliseconds.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(u32 nTime)
{
  /* Enable the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Enable);
  
  TimingDelay = nTime;

  while(TimingDelay != 0);

  /* Disable the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Disable);
  /* Clear the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Clear);
}

/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
/*
void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}

*/





#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
