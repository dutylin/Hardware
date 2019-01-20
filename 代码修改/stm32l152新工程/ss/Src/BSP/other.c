#include "global.h"
#include "other.h"
#include "spi.h"
extern TIM_HandleTypeDef htim2;
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

    /**************TIM4 红外脉冲宽度计时中断****************/

   HAL_NVIC_SetPriority(TIM4_IRQn, 0x04, 0x01);
   HAL_NVIC_EnableIRQ(TIM4_IRQn);
    /****************TIM2普通定时中断服务********************/
   HAL_NVIC_SetPriority(TIM2_IRQn, 0x08, 0x04);
   HAL_NVIC_EnableIRQ(TIM2_IRQn);
	
    /****************PB0对应的0线中断服务********************/
   HAL_NVIC_SetPriority(EXTI0_IRQn, 0x0e, 0x0e);
   HAL_NVIC_EnableIRQ(EXTI0_IRQn);	
    /****************PB0对应的1线中断服务********************/
   HAL_NVIC_SetPriority(EXTI1_IRQn, 0x0f, 0x0f);
   HAL_NVIC_EnableIRQ(EXTI1_IRQn);

}
/**************************************************************
函数：void Battery_Monitor_Config (void)
功能：电池电压监测配置PC1
**************************************************************/
void Battery_Monitor_Config (void)
{


 GPIO_InitTypeDef GPIO_InitStruct = {0};

  /*Configure GPIO pin : PC4 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
}
/**************************************************************
函数：uint8_t Battery_Monitor(void)
功能：电池电压监测 PC3
**************************************************************/
uint8_t Battery_Monitor(void)
{
    if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1))return 0xCB;   //电池电压低
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5,GPIO_PIN_RESET);
	
#else
  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2,GPIO_PIN_RESET);
	
#endif
}
/**************************************************************
函数：void Power_Control(char state)
功能：传感器供电控制
**************************************************************/
void Power_Control(char state)
{
#ifdef Liceng //如果是离层启动3.3V供电
    if(state)  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5,GPIO_PIN_SET);   //高开
    else  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5,GPIO_PIN_RESET);      //低关
#else  //如果是其他传感器启动5V供电
    if(state) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2,GPIO_PIN_SET);  //高开
    else  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2,GPIO_PIN_RESET);      //低关
#endif
}
/**************************************************************
函数：void Led_Control_GPIO(void)
功能：数码管控制PC0
**************************************************************/
void Led_Control_GPIO(void)
{

 GPIO_InitTypeDef GPIO_InitStruct = {0};
	
  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_RESET);
	
}
/**************************************************************
函数：void LED_Control(char state)
功能：数码管电源控制
**************************************************************/
void LED_Control(char state)
{
    if(state)HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_SET); //高开
    else HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_RESET);    //低关
}
/**************************************************************
函数：void Timer_Config(uint8_t (*Fuction_Cfg)())
功能：定时器定时1秒
**************************************************************/
void Timer_Config(uint16_t per)
{

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 9999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = per;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
 HAL_TIM_Base_Start(&htim2);


}
/**************************************************************
函数：void Storage_Data(void)
功能：PB12控制595锁存
**************************************************************/
void Storage_Data(void)
{
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,GPIO_PIN_SET);
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

