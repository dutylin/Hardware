#include "infrared.h"
#include "stm32l1xx_tim.h"
#include "global.h"
extern uint8_t  g_IR_State ;
extern uint32_t g_IR_Code[IR_CODE_LEN];

/**************************************************************
函数：void Timer_Config(uint32_t arr,uint16_t psc)
功能：红外 TIM3_CH1
**************************************************************/
void Infrared_Config(uint32_t arr,uint16_t psc)
{
    /*TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC->APB1ENR |= RCC_APB1Periph_TIM4;
    RCC->AHBENR |= RCC_AHBPeriph_GPIOC;

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM4);

    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_ICInitStructure.TIM_Channel     = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Falling;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x03;
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
    TIM_Cmd(TIM4, ENABLE);
    TIM_ITConfig(TIM4, TIM_IT_CC1|TIM_IT_Update, ENABLE);*/
		
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC->APB1ENR |= RCC_APB1Periph_TIM4;
    RCC->AHBENR |= RCC_AHBPeriph_GPIOB;

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);

    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_ICInitStructure.TIM_Channel     = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Falling;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x03;
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
    TIM_Cmd(TIM4, ENABLE);
    TIM_ITConfig(TIM4, TIM_IT_CC1|TIM_IT_Update, ENABLE);
}
/****************************************************************************
* 名    称：uint8_t IR_KEY_Scan(void)
* 功    能：红外按键 F1->04 F2->06 丄->0C 丅->0E OK->14 C->16 用户码00FF
//01fe45ba 01ff847a 01fed12e 01ff609e 01fe21de 01feb54a
****************************************************************************/
uint32_t IR_KEY_Scan(void)
{
    unsigned char i=0;
    uint32_t Code_temp=0;
    if(g_IR_State == IR_STATE_OK)
    {
        if((g_IR_Code[i]<NEC_lead+5) && (g_IR_Code[i] > NEC_lead-5)) // NEC编码格式
        {
            for (i=1; i<33; i++)
            {
                if(g_IR_Code[i] < NEC_H+5 && g_IR_Code[i] > NEC_H-5)
                {
                    Code_temp |= 1;
                }
                else if (g_IR_Code[i] < NEC_L+5 && g_IR_Code[i] > NEC_L-5)
                {
                    Code_temp |= 0;
                }
                else {
                    return 0;
                }
                Code_temp = Code_temp << 1;
            }
            g_IR_State = IR_STATE_NO;
            return Code_temp;
        }
        else {// 解码错误,清除,准备下次接收
            g_IR_State = IR_STATE_NO;
            return 0;
        }
    } else {
        return 0;
    }

}

