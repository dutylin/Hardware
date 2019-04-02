#include "global.h"
#include "adc.h"
#include "other.h"
#include "CC1110.h"
#include "spi.h"
#include "infrared.h"
#include "rtc.h"

RCC_ClocksTypeDef RCC_Clocks;
static uint8_t  fac_us=0;
static uint16_t fac_ms=0;
extern WPStatus PD_WAKEUP_Flag;
/**************************************************************
函数：void Bsp_Config (void)
功能：硬件初始化
**************************************************************/
void RCC_Config(void)
{
    RCC_HSICmd(ENABLE);//使能内部高速晶振
    while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
    {}
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
    RCC_MSIRangeConfig(RCC_MSIRange_6);
    RCC_HSEConfig(RCC_HSE_OFF);
    if(RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET )
    {
        while(1);
    }
}

void ControlGPIO_Initt(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
//	    EXTI_InitTypeDef EXTI_InitStructure;
		
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	
}

void ControlGPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
//	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
//	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
//	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
//	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
//	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
/**************************************************************
函数：void Wake_Config(void)
功能：唤醒引脚配置
**************************************************************/
void Wake_Config(void)
{
	
	  NVIC_InitTypeDef  NVIC_InitStructure;

    EXTI_InitTypeDef EXTI_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_DeInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
 //   
	
  EXTI_ClearITPendingBit(EXTI_Line20);
  EXTI_InitStructure.EXTI_Line = EXTI_Line20;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable the RTC Wakeup Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


	

    /****************PB0对应的0线********************/
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    /****************PB1对应的1线********************/
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_Init(&EXTI_InitStructure);	
		EXTI_ClearITPendingBit(EXTI_Line0);
		EXTI_ClearITPendingBit(EXTI_Line1);
		//ControlGPIO_Init();

}
/**************************************************************
函数：void EnterLowPower(void)
功能：进入低功耗模式
**************************************************************/
void EnterLowPower(void)
{	
		GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE , ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC , ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	    GPIO_ResetBits(GPIOC, GPIO_Pin_4);
	
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA| RCC_AHBPeriph_GPIOC |RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE , DISABLE);//为了M0M1置位，把C口关闭去掉了
    TIM2->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));     //关闭定时器计数
    TIM3->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));     //关闭定时器计数
    SPI2->CR1 &= (uint16_t)~((uint16_t)SPI_CR1_SPE);       //关闭SPI
    SPI1->CR1 &= (uint16_t)~((uint16_t)SPI_CR1_SPE);       //关闭SPI	
    USART3->CR1 &= (uint16_t)~((uint16_t)USART_CR1_UE);    //关闭USART
    ADC1->CR2 &= (uint32_t)(~ADC_CR2_ADON);                //关闭ADC外设
    RCC->APB2ENR &= ~ RCC_APB2Periph_ADC1;
    RCC->APB1ENR &= ~(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|RCC_APB1Periph_SPI2|RCC_APB1Periph_USART3); //关闭外设时钟
    Wake_Config();	
//		
//    //RtcWakeUpConfig();

//		
    PWR_ClearFlag(PWR_FLAG_WU);
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);

}
/**************************************************************
函数：WPStatus PD_WAKEUP_status (void)
功能：光敏唤醒检测 读PB1状态 如果高电平检测到光照*
**************************************************************/
PDStatus PD_WAKEUP_status (void)
{
    PDStatus status = Low;
    if(GPIOB->IDR & GPIO_Pin_1)status=High;
    else status=Low;
    return status;
}
/**************************************************************
函数：void delay_init(void)
功能：延时
**************************************************************/
void delay_init(void)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    fac_us=SystemCoreClock/16000000;
    fac_ms=(uint16_t)fac_us*1000;
}
/**************************************************************
函数：void delay_us(uint32_t nus)
功能：延时微秒
**************************************************************/
void delay_us(uint32_t nus)
{
    uint32_t temp;
    SysTick->LOAD=nus*fac_us; //时间加载
    SysTick->VAL=0x00;        //清空计数器
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; //开始倒数
    do
    {
        temp=SysTick->CTRL;
    }
    while(temp&0x01&&!(temp&(1<<16)));//等待时间到达
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; //关闭计数器
    SysTick->VAL =0X00;       //清空计数器
}
/**************************************************************
函数：void delay_ms(uint16_t nms)
功能：延时毫秒 nms<=0xffffff*8*1000/SYSCLK 对32M条件下,nms<=4148
**************************************************************/
void delay_ms(uint16_t nms)
{
    uint32_t temp;
    SysTick->LOAD=(uint32_t)nms*fac_ms;
    SysTick->VAL =0x00;
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
    do
    {
        temp=SysTick->CTRL;
    }
    while(temp&0x01&&!(temp&(1<<16)));
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL =0X00;
}

/**************************************************************
函数：void Bsp_Config (uint8_t (*Fuction_Cfg)())
功能：硬件初始化
**************************************************************/
void Bsp_Config (void)
{
    RCC_Config();
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.HCLK_Frequency /100);
    Power_Control_GPIO();     //探头供电
    ADC_Config();             //ADC初始化
    Battery_Monitor_Config(); //电池检测
    delay_init();             //延时

}

