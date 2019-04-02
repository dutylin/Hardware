#include "global.h"
#include "other.h"
#include "spi.h"
unsigned char _menu_tab[]= {0x9e,0xa7,0xc6,0xbc,0xb3,0xf0};
unsigned char _idcl_Tab[]= {0xbf,0xff};    //ID显示标志 清除数码管
#if (defined Liceng)||(defined Weiyan)||(defined Yali)
unsigned char _flag1_tab[]= {0xfe,0xf7};             //离层传感器的标示
#elif (defined Maogan)||(defined Maosuo)
unsigned char _flag2_tab[]= {0xfe,0xbf,0xf7,0xbe};   //其他传感器的标示
#endif
unsigned char _data_tab[]= {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x7f}; //0123456789.
/**************************************************************
函数：void NVIC_Config(void)
功能：中断服务程序
**************************************************************/
void NVIC_Config(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    /**************TIM4 红外脉冲宽度计时中断****************/
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /****************TIM2普通定时中断服务********************/
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x08;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =  0x04;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /****************PB0对应的0线中断服务********************/
    NVIC_InitStructure.NVIC_IRQChannel =EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0e;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0e;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /****************PB0对应的1线中断服务********************/
    NVIC_InitStructure.NVIC_IRQChannel =EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}
/**************************************************************
函数：void Battery_Monitor_Config (void)
功能：电池电压监测配置PC1
**************************************************************/
void Battery_Monitor_Config (void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/**************************************************************
函数：uint8_t Battery_Monitor(void)
功能：电池电压监测 PC3
**************************************************************/
uint8_t Battery_Monitor(void)
{
    if(GPIOC->IDR & GPIO_Pin_1)return 0xCB;   //电池电压低
    else return 0xCA; //电池电压正常
}
/**************************************************************
函数：void Power_Control_GPIO(void)
功能： 传感器电源控制PC5->3.3V PC2->5V
**************************************************************/
void Power_Control_GPIO(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
#ifdef Liceng //如果是离层启动3.3V供电
    RCC->AHBENR |= RCC_AHBPeriph_GPIOC;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIOC->BSRRH = GPIO_Pin_5;
#else
    RCC->AHBENR |= RCC_AHBPeriph_GPIOC;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd =GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIOC->BSRRH = GPIO_Pin_2;
#endif
}
/**************************************************************
函数：void Power_Control(char state)
功能：传感器供电控制
**************************************************************/
void Power_Control(char state)
{
#ifdef Liceng //如果是离层启动3.3V供电
    if(state)GPIOC->BSRRL= GPIO_Pin_5;   //高开
    else GPIOC->BSRRH = GPIO_Pin_5;      //低关
#else  //如果是其他传感器启动5V供电
    if(state)GPIOC->BSRRL = GPIO_Pin_2;  //高开
    else GPIOC->BSRRH = GPIO_Pin_2;      //低关
#endif
}
/**************************************************************
函数：void Led_Control_GPIO(void)
功能：数码管控制PC0
**************************************************************/
void Led_Control_GPIO(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC->AHBENR |= RCC_AHBPeriph_GPIOC;
    /****************数码管控制******************/
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIOC->BSRRH = GPIO_Pin_0;
}
/**************************************************************
函数：void LED_Control(char state)
功能：数码管电源控制
**************************************************************/
void LED_Control(char state)
{
    if(state)GPIOC->BSRRL = GPIO_Pin_0; //高开
    else GPIOC->BSRRH = GPIO_Pin_0;     //低关
}
/**************************************************************
函数：void Timer_Config(uint8_t (*Fuction_Cfg)())
功能：定时器定时1秒
**************************************************************/
void Timer_Config(uint16_t per)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC->APB1ENR |= RCC_APB1Periph_TIM2;

    TIM_TimeBaseStructure.TIM_Period = per;            //(1+per)/16MHz*(1+9999)=x
    TIM_TimeBaseStructure.TIM_Prescaler = 9999;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM2->SR = (uint16_t)~TIM_FLAG_Update;
    TIM2->CR1 |= TIM_CR1_ARPE;
    TIM2->CR1 |= TIM_CR1_CEN;
    TIM2->DIER |= TIM_IT_Update;  //使能定时器TIM2
}
/**************************************************************
函数：void Storage_Data(void)
功能：PB12控制595锁存
**************************************************************/
void Storage_Data(void)
{
    GPIOB->BSRRL = GPIO_Pin_12;
    GPIOB->BSRRH = GPIO_Pin_12;
    GPIOB->BSRRL = GPIO_Pin_12;
}
/**************************************************************
函数：void Led_Display(uint16_t(*fun)(uint16_t),uint8_t *table,uint8_t i,uint8_t dis_bit)
功能：数码管显示各个位
**************************************************************/
void Led_Display(uint16_t(*fun)(uint16_t),uint8_t *table,uint8_t i,uint8_t dis_bit)
{
    fun(*(table+i));
    fun(dis_bit);
    Storage_Data();
    delay_ms(2);
}
/**************************************************************
函数：void Led_Display_Data(int channel,int data)
功能：数码管显示数据
**************************************************************/
void Led_Display_Data(uint8_t channel,uint16_t data)
{
#if (defined Liceng)||(defined Weiyan)||(defined Yali)
    Led_Display(&SPI2_ReadSend_byte,_flag1_tab,channel,0x08);//标示
#elif (defined Maogan)||(defined Maosuo)
    Led_Display(&SPI2_ReadSend_byte,_flag2_tab,channel,0x08);//标示
#endif
    Led_Display(&SPI2_ReadSend_byte,_data_tab,data/100%10,0x04);//百位
    Led_Display(&SPI2_ReadSend_byte,_data_tab,data/10%10,0x02); //十位
    Led_Display(&SPI2_ReadSend_byte,_data_tab,data%10,0x01); //个位
#if defined Yali
    Led_Display(&SPI2_ReadSend_byte,_data_tab,10,2);    //小数点
#endif
}
/**************************************************************
函数：void Led_Display_XX(uint8_t sensor_id)
功能：数码管显示传感器ID/NUM
**************************************************************/
void Led_Display_XX(uint8_t sensor_id)
{
    Led_Display(&SPI2_ReadSend_byte,_idcl_Tab,0,0x08);
    Led_Display(&SPI2_ReadSend_byte,_data_tab,sensor_id/10%10,0x04);//十位
    Led_Display(&SPI2_ReadSend_byte,_data_tab,sensor_id%10,0x02);//个位
    Led_Display(&SPI2_ReadSend_byte,_idcl_Tab,0,0x01);
}
/**************************************************************
函数：void Led_Display_Set(char type,uint8_t sensor_id)
功能：数码管显示传感器编号
**************************************************************/
void Led_Display_Set(uint8_t i,uint8_t data)
{
    Led_Display(&SPI2_ReadSend_byte,_menu_tab,i,0x08);
    Led_Display(&SPI2_ReadSend_byte,_data_tab,data/10%10,0x04);//十位
    Led_Display(&SPI2_ReadSend_byte,_data_tab,data%10,0x02);//个位
    Led_Display(&SPI2_ReadSend_byte,_menu_tab,i+3,0x01);
}
/**************************************************************
函数：void Led_Display_Set(uint8_t sensor_id)
功能：数码管显示传感器编号
**************************************************************/
void LED_Display_Clear(void)
{
    uint8_t i;
    for(i=0x08; i>=0x01; i=i>>1) //清除整数位
    {
        Led_Display(&SPI2_ReadSend_byte,_idcl_Tab,1,i);
    }
    for(i=4; i>=1; i--)       //清除小数点
    {
        Led_Display(&SPI2_ReadSend_byte,_idcl_Tab,1,i);
    }
}

