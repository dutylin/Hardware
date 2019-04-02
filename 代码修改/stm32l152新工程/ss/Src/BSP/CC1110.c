#include "CC1110.h"
#include "global.h"
#include <string.h>
#include <stdio.h>
#include "stm32l1xx.h"
#include "stm32l1xx_hal_uart.h"
extern UART_HandleTypeDef huart3;
 UART_HandleTypeDef huart1;
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
int Value[14]= {0xAA,0x08,0xE1,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xCA,0x55};
int cnt=0;
int xinxi=0;
int jj=0;
uint8_t RxBuffer1[10]={0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05};
uint8_t TxBuffer1[];
uint8_t RxCounter1;
uint8_t vid1,vid2,vid3,vid4,oldid,newid;
extern uint32_t Flash_Read_Buff[4],Flash_Write_Buff[4];
struct __FILE
{
    int handle;

};
FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit( int x)
{
    x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
  uint8_t tmp;
	tmp = ch;
   // while((USART1->SR&0X40)==0) //循环发送,直到发送完毕
     //   USART1->DR = (uint8_t) ch;

 HAL_UART_Transmit(&huart1, &tmp, 1, 10);
	
    return ch;
}
/**************************************************************
函数：void CC1110_Config (uint32_t baud_rate)
功能：CC1110用的是UART1通讯 PA9-UART_TX;PA10-UART_RX;PA5-ACT;PA8-BUF
**************************************************************/
void CC1110_Config (uint32_t baud_rate)
{


 HAL_UART_MspInit(&huart3); 

 /*********************配置无线通讯TX RX*************************/
/* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = baud_rate;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);


}
/**************************************************************
函数：void CC1110_Gpio_RESET(void)
功能：将单片机UART引脚重置
**************************************************************/
void CC1110_Gpio_RESET(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin =GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}
/**************************************************************
函数：CCStatus CC1110_TX_status(void)
功能：读模块TX脚的状态
**************************************************************/
CCStatus CC1110_TX_Status(void)
{
    CCStatus status = Low;
    if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11))status=High;
    else status=Low;
    return status;
}

/**************************************************************
函数：void CC1110_Send (uint8_t * TxMsg ,int len)
功能：CC1110发送函数
**************************************************************/
void CC1110_Send (uint8_t * TxMsg ,int len)
{
    int i;
    //USART3->SR = (uint16_t)~0x0040;
    for(i=0; i<len; i++)
    {
     //   USART3->DR = (*(TxMsg+i) & (uint16_t)0x01FF);
       // while((USART3->SR & 0x0080) == RESET); //发送完成标志
         HAL_UART_Transmit(&huart3, (TxMsg+i), 1, 10);
    }
}
int rx_index = 0;
int mq = 0;
int i=0;

void USART3_IRQHandler(void)//接收中继器数据
{
	//static int rx_index = 0;
	if (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_RXNE) != RESET) // Received characters modify string  
	{  
		HAL_UART_Receive(&huart3,&RxBuffer1[rx_index++],1,10);
		xinxi=1;
		
		cnt++;
		ControlGPIO_Initt();
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);//拉低 
//		if(RxBuffer1[0]==0xA0)
//		{
//		   USART_ClearFlag(USART3,USART_FLAG_RXNE);
//		   CC1110_Sendd();
//				rx_index=0;
//		}
//		else 
			if(RxBuffer1[0]==0xC1)
    {

			CC1110_Senddata();
			rx_index=0;
		} 
//		else if((RxBuffer1[0]<128)&&(RxBuffer1[0]>0))
//    {

//  	  newid=RxBuffer1[0];
//			//CC1110_Sendbianhao();	
//      CC1110_Sendnum();			
//			rx_index=0;
//			
//		} 
	 
	}
		//rx_index=0;
}
