#include "bsp_uart.h"
#include "stdio.h"
#include "ddl.h"
#include "uart.h"
#include "gpio.h"
#include "bt.h"
/**
* @brief  Configure USART peripheral  
* @param  None
* @retval None
*/
void RxIntCallback(void);
void ErrIntCallback(void);
unsigned char UsartReceiveData[BUFFERSIZE];
unsigned char UsartReceiveFlag,usart_i,j;
void UART_Config(void)
{
    uint16_t timer=0;
    uint32_t pclk=0;
	
    stc_uart_config_t  stcConfig;
    stc_uart_irq_cb_t stcUartIrqCb;
    stc_uart_multimode_t stcMulti;
    stc_uart_baud_config_t stcBaud;
    stc_bt_config_t stcBtConfig;
    

    DDL_ZERO_STRUCT(stcUartIrqCb);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcBaud);
    DDL_ZERO_STRUCT(stcBtConfig);

    
  //  Gpio_InitIO(T1_PORT,T1_PIN,GpioDirIn); 
   // Gpio_InitIO(0,3,GpioDirOut);
   // Gpio_SetIO(0,3,1);
    
    Gpio_InitIOExt(UART1_PORT,UART1_TXD,GpioDirOut,TRUE,FALSE,FALSE,FALSE);   
    Gpio_InitIOExt(UART1_PORT,UART1_RXD,GpioDirOut,TRUE,FALSE,FALSE,FALSE); 
    
    //通道端口配置
    Gpio_SetFunc_UART1TX_P35();
    Gpio_SetFunc_UART1RX_P36();

    //外设时钟使能
    Clk_SetPeripheralGate(ClkPeripheralBt,TRUE);//模式0/2可以不使能
    Clk_SetPeripheralGate(ClkPeripheralUart1,TRUE);



    stcUartIrqCb.pfnRxIrqCb = RxIntCallback;
    stcUartIrqCb.pfnTxIrqCb = NULL;
    stcUartIrqCb.pfnRxErrIrqCb = ErrIntCallback;
    stcConfig.pstcIrqCb = &stcUartIrqCb;
    stcConfig.bTouchNvic = TRUE;
  

    stcConfig.enRunMode = UartMode3;//测试项，更改此处来转换4种模式测试
   

    stcMulti.enMulti_mode = UartNormal;//测试项，更改此处来转换多主机模式，mode2/3才有多主机模式

    stcConfig.pstcMultiMode = &stcMulti;

    stcBaud.bDbaud = 0u;//双倍波特率功能
    stcBaud.u32Baud =4800u;//更新波特率位置
    stcBaud.u8Mode = UartMode3; //计算波特率需要模式参数
    pclk = Clk_GetPClkFreq();
    timer=Uart_SetBaudRate(UARTCH1,pclk,&stcBaud);

    stcBtConfig.enMD = BtMode2;
    stcBtConfig.enCT = BtTimer;
    Bt_Init(TIM1, &stcBtConfig);//调用basetimer1设置函数产生波特率
    Bt_ARRSet(TIM1,timer);
    Bt_Cnt16Set(TIM1,timer);
    Bt_Run(TIM1);

    Uart_Init(UARTCH1, &stcConfig);
    Uart_EnableIrq(UARTCH1,UartRxIrq);
    Uart_ClrStatus(UARTCH1,UartRxFull);
    Uart_EnableFunc(UARTCH1,UartRx);
	
}

void USART_SendStr(unsigned char *Str) 
{
  while(*Str!=0)
  {
  Uart_SendData(UARTCH1, *Str);
    Str++;
  }
}
void UART1_SendByte(uint8_t data)
{  	
  Uart_SendData(UARTCH1, data);
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
  Uart_SendData(UARTCH1, ch);
  
  return (ch);
}
#else
PUTCHAR_PROTOTYPE
{
  /* Write a character to the UART1 */
  Uart_SendData(UARTCH1, c);
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
  while (Uart_GetStatus(UARTCH1,UartRxFull) == FALSE);
  c = Uart_ReceiveData(UARTCH1);
  return (c);
}
void RxIntCallback(void)
{

    if(Uart_GetStatus(UARTCH1,UartRxFull) == TRUE)
    {
        UsartReceiveFlag =1;
       Uart_ClrStatus(UARTCH1,UartRxFull);//清除中断标志
       UsartReceiveData[j] = Uart_ReceiveData(UARTCH1);
      j++;
     if(j>=BUFFERSIZE)
	 	j=0;
    }
}
void ErrIntCallback(void)
{
  Uart_ClrStatus(UARTCH1,UartRFRAMEError);
}

