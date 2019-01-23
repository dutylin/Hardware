#include "infrared.h"
//#include "stm32l1xx_tim.h"
#include "global.h"
extern TIM_HandleTypeDef htim4;
extern uint8_t  g_IR_State ;
extern uint32_t g_IR_Code[IR_CODE_LEN];

/**************************************************************
函数：void Timer_Config(uint32_t arr,uint16_t psc)
功能：红外 TIM3_CH1
**************************************************************/
void Infrared_Config(uint32_t arr,uint16_t psc)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_TIM4_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  /*Configure GPIO pin : PC6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);







  
    /* TIM4 interrupt Init */
    HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);
	
TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = psc;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = arr;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

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
		

HAL_TIM_Base_Start(&htim4);
__HAL_TIM_ENABLE_IT(&htim4,TIM_IT_CC1|TIM_IT_UPDATE);

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

