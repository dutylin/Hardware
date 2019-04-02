#include "CC1110.h"
#include "global.h"
#include <string.h>
#include <stdio.h>

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
    while((USART1->SR&0X40)==0) //循环发送,直到发送完毕
        USART1->DR = (uint8_t) ch;
    return ch;
}
/**************************************************************
函数：void CC1110_Config (uint32_t baud_rate)
功能：CC1110用的是UART1通讯 PA9-UART_TX;PA10-UART_RX;PA5-ACT;PA8-BUF
**************************************************************/
void CC1110_Config (uint32_t baud_rate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
	
    RCC->APB1ENR |= RCC_APB1Periph_USART3;
    RCC->AHBENR |= RCC_AHBPeriph_GPIOB;

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10,GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
    /*********************?????T??悫??PB10--TX*************************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    /*********************?????T??悫??PB11--RX*************************/
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_11;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    /*********************配置无线通讯TX RX*************************/
    USART_InitStructure.USART_BaudRate = baud_rate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3,&USART_InitStructure);
    USART_Cmd(USART3, ENABLE);
		
		 //USART_ITConfig(USART3, USART_IT_TXE, ENABLE);				  //enable TXD interrupt
	  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);				  //enable RXD interrupt
		
		NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		
		NVIC_Init(&NVIC_InitStructure);
}
/**************************************************************
函数：void CC1110_Gpio_RESET(void)
功能：将单片机UART引脚重置
**************************************************************/
void CC1110_Gpio_RESET(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;   //此处不能添加 否则无线不能发送
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/**************************************************************
函数：CCStatus CC1110_TX_status(void)
功能：读模块TX脚的状态
**************************************************************/
CCStatus CC1110_TX_Status(void)
{
    CCStatus status = Low;
    if(GPIOB->IDR & GPIO_Pin_11)status=High;
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
    USART3->SR = (uint16_t)~0x0040;
    for(i=0; i<len; i++)
    {
        USART3->DR = (*(TxMsg+i) & (uint16_t)0x01FF);
        while((USART3->SR & 0x0080) == RESET); //发送完成标志
    }
}
int rx_index = 0;
int mq = 0;
int i=0;
void USART3_IRQHandler(void)//接收中继器数据
{
	//static int rx_index = 0;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) // Received characters modify string  
	{  
		RxBuffer1[rx_index++]=USART_ReceiveData(USART3);
		xinxi=1;
		
		cnt++;
		ControlGPIO_Initt();
		GPIO_ResetBits(GPIOC,GPIO_Pin_8);//拉低 

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
