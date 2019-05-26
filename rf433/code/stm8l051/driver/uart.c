#include "uart.h"
#include "stdio.h"
/**
* @brief  Configure USART peripheral  
* @param  None
* @retval None
*/
void UART_Config(void)
{
  /* USART configured as follow:
  - BaudRate = 115200 baud  
  - Word Length = 8 Bits
  - One Stop Bit
  - Odd parity
  - Receive and transmit enabled
  - USART Clock disabled
  */
  /* Enable USART clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
  SYSCFG_REMAPDeInit();
  SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortC, ENABLE);
  /* Configure USART Tx as alternate function push-pull  (software pull up)*/
  GPIO_ExternalPullUpConfig(GPIOC,GPIO_Pin_5, ENABLE);
  
  /* Configure USART Rx as alternate function push-pull  (software pull up)*/
  GPIO_ExternalPullUpConfig(GPIOC, GPIO_Pin_6, ENABLE);
  USART_DeInit(USART1);		//复位UART1 
  /* USART configuration */
  USART_Init(USART1, 115200,
             USART_WordLength_8b,
             USART_StopBits_1,
             USART_Parity_No,
             (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));
  /* Enable general interrupts */
  enableInterrupts();
  
  /* Enable the USART Receive interrupt: this interrupt is generated when the USART
  receive data register is not empty */
  USART_ITConfig(USART1, USART_IT_RXNE|USART_IT_IDLE, ENABLE);
  /* Enable the USART Transmit complete interrupt: this interrupt is generated when the USART
  transmit Shift Register is empty */
  // USART_ITConfig(USART1, USART_IT_TC, ENABLE);
  
  /* Enable USART */
  USART_Cmd(USART1, ENABLE);
}

void USART_SendStr(unsigned char *Str) 
{
  while(*Str!=0)
  {
    USART_SendData8(USART1, *Str);
    while(!USART_GetFlagStatus (USART1, USART_FLAG_TXE));
    Str++;
  }
}
void UART1_SendByte(u8 data)
{  	
  USART_SendData8(USART1, data);	
  /* 等待传输结束 */	
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    ;
}
/*******************************************************************************
* 名称: fputc
* 功能: 重定向c库函数printf到UART1
* 形参: 无
* 返回: 要打印的字符
* 说明: 由printf调用 
******************************************************************************/
#ifdef _IAR_
typedef struct __FILE FILE;
int fputc(int ch, FILE *f)
{  
  /* 将Printf内容发往串口 */
  UART1_SendByte(ch);
  
  return (ch);
}
#else
PUTCHAR_PROTOTYPE
{
  /* Write a character to the UART1 */
  UART1_SendByte(c);
  return (c);
}
#endif

GETCHAR_PROTOTYPE
{
#ifdef _COSMIC_
  char c = 0;
#else
  int c = 0;
#endif
  /* Loop until the Read data register flag is SET */
  while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
  c = USART_ReceiveData8(USART1);
  return (c);
}


