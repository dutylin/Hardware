#include "stm8l15x.h"
#include "sysclock.h"
#include "uart1.h"
#include "stm8l15x_clk.h"
#include "sx1276.h"
#include "stm8l15x_spi.h"
#include "bsp.h"
#include "stm8l15x_it.h"
#include "stm8l15x_tim3.h"
#include "stm8l15x_rtc.h"

u8 UART1_RX_BUF_1 = 78;

#define TX              1       // 发送模式
#define RX              0       // 接收模式

#define NormalMode      0x00
#define WakeMode        0x10
#define PowerSavingMode 0x01
u8 CurrentMode = 0;
u8 ConfigureDone = 0;

#define ACK_LENGTH      10      // 应答信号长度
#define SEND_LENGTH     10      // 发送数据每包的长度
INT8U   SendBuffer[SEND_LENGTH] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
INT8U   SendFlag = 1;           // =1，发送无线数据，=0不处理
INT16U  SendCnt = 0;            // 计数发送的数据包数
INT16U  SendTime = 1;           // 计数数据发送间隔时间
INT16U  RecvWaitTime = 0;       // 接收等待时间
#define RECV_TIMEOUT    800     // 接收超时
//需要应答的数据
INT8U   AckBuffer[ACK_LENGTH] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
INT16U  RecvCnt = 0;            // 计数接收的数据包数

void DelayMs(INT8U x)
{
  unsigned int i;
  i=1000;
  for(;x>0;x--)  
    for(i=1000;i>0;i--)
      ;
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

void LED_Init(void)
{
  GPIO_Init(GPIOD, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Fast);
}

void delay_ms(u16 num)//不是很精确
{
  u8 i = 0;
  while(num--)
  {
    for (i=0; i<100; i++);
  }
}

void GPIO_Initial(void)
{
  
  GPIO_Init( GPIOA, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  GPIO_Init( GPIOB, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  GPIO_Init( GPIOC, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  GPIO_Init( GPIOD, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  
  /* 配置SX127X相关控制引脚NSS(PB4), RST(PA3), TX_CTRL(PB3), RX_CTRL(PB2), OSEN(PA2)*/
  GPIO_Init(PORT_LED, PIN_LED, GPIO_Mode_Out_PP_High_Slow);
  GPIO_SetBits(PORT_LED, PIN_LED);
  
  GPIO_Init(PORT_SX127X_CSN, PIN_SX127X_CSN, GPIO_Mode_Out_PP_High_Slow);
  GPIO_SetBits(PORT_SX127X_CSN, PIN_SX127X_CSN);
  
  GPIO_Init(PORT_SX127X_RST, PIN_SX127X_RST, GPIO_Mode_Out_PP_High_Slow);
  GPIO_SetBits(PORT_SX127X_RST, PIN_SX127X_RST);
  
  GPIO_Init(PORT_SX127X_TX_CTRL, PIN_SX127X_TX_CTRL, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_ResetBits(PORT_SX127X_TX_CTRL, PIN_SX127X_TX_CTRL);
  
  GPIO_Init(PORT_SX127X_RX_CTRL, PIN_SX127X_RX_CTRL, GPIO_Mode_Out_PP_High_Slow);
  GPIO_SetBits(PORT_SX127X_RX_CTRL, PIN_SX127X_RX_CTRL);
  
  /*SX1276数据接收外部中断*/
  //GPIO_Init(GPIOD, GPIO_Pin_1, GPIO_Mode_In_FL_IT);
  //EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Rising);
  
  /*SX1276数据接收外部中断*/
  GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_In_FL_IT);
  EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Rising);
  GPIO_Init(PORT_SX127X_DIO0, PIN_SX127X_DIO0, GPIO_Mode_In_FL_IT);
  EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Rising);
  // GPIO_Init(PORT_SX127X_DIO0, PIN_SX127X_DIO0, GPIO_Mode_In_FL_IT);
  //EXTI_SetPinSensitivity(EXTI_Pin_0, EXTI_Trigger_Rising);
  //  EXTI_SetHalfPortSelection(EXTI_HalfPort_D_LSB, ENABLE);
  
}

void SPI_Initial(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, ENABLE);
  SPI_DeInit(SPI1);
  
  // 配置SPI相关参数,2分频（8MHZ）
  SPI_Init(SPI1, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_2,
           SPI_Mode_Master, SPI_CPOL_Low, SPI_CPHA_1Edge,
           SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft, 7);
  SPI_Cmd(SPI1, ENABLE);
  
  // SPI相关IO口配置
  GPIO_Init(PORT_SPI, PIN_MISO, GPIO_Mode_In_PU_No_IT);       // MISO (PB7)
  GPIO_Init(PORT_SPI, PIN_SCLK, GPIO_Mode_Out_PP_High_Slow);  // SCLK (PB5)
  GPIO_Init(PORT_SPI, PIN_MOSI, GPIO_Mode_Out_PP_High_Slow);  // MOSI (PB6)
}

void RF_Initial( )
{
  SX1276_Init(MODE_LORA);         //SX127X 模块复位并初始化为LoRa模式
  SX1276_LoRa_SetDataRate(5);
  SX1276_SetFreq(20);				//配置信道为23，即433Hz
  SX1276_SetPower(15, 1);         //配置RFO PIN输出，功率20dBm
  SX1276_SetPreambleSize(65530);
  SX1276_SetRxMode(); 
}

/*===========================================================================
* 函数 ：TIM3_Initial() => 初始化定时器3，定时时间为1ms                     *
============================================================================*/
void TIM3_Initial(void)
{
  TIM3_DeInit();
  
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);
  
  // 配置Timer3相关参数，内部时钟为16MHZ，定时时间 1ms  需要计次125
  TIM3_TimeBaseInit(TIM3_Prescaler_128, TIM3_CounterMode_Up, 31250); //定时250ms
  TIM3_ITConfig(TIM3_IT_Update, ENABLE);
  TIM3_Cmd(ENABLE);
}

void RTC_Config(uint16_t time) 
{
  RTC_DeInit(); //初始化默认状态 
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //允许RTC时钟 
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_64); //选择RTC时钟源LSI、2=19K 
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16); //19k/16=1.1875KHz t=0.85ms 
  if (CurrentMode == PowerSavingMode){
    
    RTC_ITConfig(RTC_IT_WUT, ENABLE); //开启中断 
    RTC_SetWakeUpCounter(time); //设置RTC Weakup计算器初值 
    RTC_WakeUpCmd(ENABLE); //使能自动唤醒 
    
  }
  else if(CurrentMode == NormalMode){
    RTC_ITConfig(RTC_IT_WUT, DISABLE); //开启中断 
    RTC_SetWakeUpCounter(time); //设置RTC Weakup计算器初值 
    RTC_WakeUpCmd(DISABLE); //使能自动唤醒 		
  }
  
}
INT8U length = 0, recv_buffer[128]={ 0 };
char i = 0;
uint32_t rx_length;
void lora_process()
{
  switch (CurrentMode) {
  case NormalMode:
    if(ConfigureDone) {
      // SX1276_SetRxMode();
      //goto NormalReceive;
      if( UsartReceiveFlag == 1 )
      {
        SX1276_SetPreambleSize(2100);
        SX1276_SendPacket(UsartReceiveData, (usart_i-1));
        SX1276_SetRxMode();
        //USART_SendData8(USART1, usart_i);
        //while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
        //          for(i = 0; i < (usart_i-1); i++) {
        USART_SendData8(USART1,usart_i);
        //            while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
        //          }
        usart_i = 0;
        LED_TOG();
        UsartReceiveFlag = 0;
      }
      length = SX1276_ReceivePacket(recv_buffer);           // 检测是否收到一个数据包
      if( length )
      {
        //USART_SendData8(USART1, length);
        //while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
        for(i = 0; i < length; i++) {
          USART_SendData8(USART1, recv_buffer[i]);
          while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
        }
        length = 0;
        LED_TOG();
      }
    } else {
      /*SX1276数据接收外部中断
      DIO0作为串口数据接收完成中断
      */
      disableInterrupts();
      RtcWakeUp = 0;
      //GPIO_Init(GPIOD, GPIO_Pin_1, GPIO_Mode_In_FL_IT);  //设置DIO0相应的管脚为外部中断
      //EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Rising);
      // SX1276_WriteReg( REG_LR_PREAMBLEMSB, 0x00 );
      // SX1276_WriteReg( REG_LR_PREAMBLELSB, 0x08 );
      
      ConfigureDone = 1;
      enableInterrupts();
    }
    break;
  case WakeMode:
    if(ConfigureDone) {
      SX1276_SetRxMode();
      //goto NormalReceive;
      if( UsartReceiveFlag == 1 )
      {
        SX1276_SendPacket(UsartReceiveData, (usart_i-1));
        SX1276_SetRxMode();
        //USART_SendData8(USART1, usart_i);
        //while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
        for(i = 0; i < (usart_i-1); i++) {
          USART_SendData8(USART1, UsartReceiveData[i]);
          while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
        }
        usart_i = 0;
        LED_TOG();
        UsartReceiveFlag = 0;
      }
      length = SX1276_ReceivePacket(recv_buffer);           // 检测是否收到一个数据包
      if( length )
      {
        //USART_SendData8(USART1, length);
        //while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
        for(i = 0; i < length; i++) {
          USART_SendData8(USART1, recv_buffer[i]);
          while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
        }
        length = 0;
        LED_TOG();
      }
    } else {
      /*SX1276数据接收外部中断
      DIO0作为串口数据接收完成中断
      唤醒模式加前导码，以用来唤醒省电模式的模块
      */
      disableInterrupts();
      RtcWakeUp = 0;
      SX1276_SetPreambleSize(1950);   //前导码为972 BW 500kHZ SF为7 则空中传输时间250ms
      ConfigureDone = 1;
      enableInterrupts();
    }
    break;
  case PowerSavingMode:
    if(ConfigureDone)
    {
      //goto PowerSavingReceive;
       if(RtcWakeUp == 1)
      {
        RtcWakeUp = 0;
        SX1276_SetPreambleSize(65530);
        SX1276_CADMode();
          if(CadDoneFlag == 1)
        {
          //  RTC_WakeUpCmd(DISABLE);
           //SX1276_SetPreambleSize(8000);
          RTC_WakeUpCmd(DISABLE);
           SX1276_SetRxMode();
           delay_ms(60000);
          //  SX1276_LoRa_SetMode( LORA_MODE_RXC );
         // length = SX1276_ReceivePacket(recv_buffer);           // 检测是否收到一个数据包
          //length = SX1276_LoRa_GetRxPacket( recv_buffer );
          
          if( length )
          {
            rx_length = rx_length +length+4;
             
            //SX1276_SetRxMode();
            //  SX1276_LoRa_SetMode( LORA_MODE_RXC );
            // SX1276_SetPreambleSize(65530);
            // USART_SendData8(USART1, length);
            //  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
            for(i = 0; i < length; i++) {
              USART_SendData8(USART1, recv_buffer[i]);
              while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
            }
            length = 0;
             RTC_WakeUpCmd(ENABLE);
          }
           else
             RTC_WakeUpCmd(ENABLE);
          //LED_TOG();
          CadDoneFlag = 0;
           SX1276_LoRa_SetMode( LORA_MODE_SLEEP );
        }
        LED_TOG();
      }
    } 
        else {
          /*SX1276数据接收外部中断
          DIO3作为CADDone的出发中断
          用RTC进行定时
          */
          disableInterrupts();
          
          //RTC_Config(588);  //500ms
          RTC_Config(19);
          enableInterrupts();
          ConfigureDone = 1;
     }
    break;
  default:
    break;
  }
  /*
  NormalReceive:
  if( UsartReceiveFlag == 1 )
  {
  SX1276_SendPacket(UsartReceiveData, (usart_i-1));
  SX1276_SetRxMode();
  //USART_SendData8(USART1, usart_i);
  //while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
  for(i = 0; i < (usart_i-1); i++) {
  USART_SendData8(USART1, UsartReceiveData[i]);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
}
  usart_i = 0;
  LED_TOG();
  UsartReceiveFlag = 0;
}
  length = SX1276_ReceivePacket(recv_buffer);           // 检测是否收到一个数据包
  if( length )
  {
  //USART_SendData8(USART1, length);
  //while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
  for(i = 0; i < length; i++) {
  USART_SendData8(USART1, recv_buffer[i]);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
}
  length = 0;
  LED_TOG();
}
  PowerSavingReceive:
  if(RtcWakeUp == 1)
  {
  RtcWakeUp = 0;
  SX1276_CADMode();
  if(CadDoneFlag == 1) {
  SX1276_LoRa_SetMode( LORA_MODE_RXC );
  length = SX1276_ReceivePacket(recv_buffer);           // 检测是否收到一个数据包
  if( length )
  {
  USART_SendData8(USART1, length);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
  for(i = 0; i < length; i++) {
  USART_SendData8(USART1, recv_buffer[i]);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
}
  length = 0;
}
  LED_TOG();
  CadDoneFlag = 0;
  SX1276_LoRa_SetMode( LORA_MODE_SLEEP );
}
  RTC_WakeUpCmd(ENABLE);
}
  */
}

void main(void)
{	
  
  
  SystemClock_Init();     // 系统时钟初始化
  disableInterrupts();
  
  GPIO_Initial();         // 初始化GPIO
  
  LED_Init();
  SPI_Initial();
  CurrentMode =NormalMode ;//NormalMode  PowerSavingMode
  
  RF_Initial();         // 初始化无线芯片
  Uart1_Init();
  enableInterrupts();
  USART_SendStr("时钟+串口初始化完成\r\n");
  
  
  while(1)
  {
    lora_process();
    
  }
}



