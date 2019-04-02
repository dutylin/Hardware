#include "USART1.h"
#include "delay.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	  
#include "timer.h"
#include "lora_cfg.h"
#include "lora_app.h"


//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//串口3初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2016/6/17
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.0修改说明 
////////////////////////////////////////////////////////////////////////////////// 

//串口发送缓存区 	
__align(8) u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 	      //发送缓冲,最大USART3_MAX_SEND_LEN字节
#ifdef USART3_RX_EN   								      //如果使能了接收   	  
//串口接收缓存区 	
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				      //接收缓冲,最大USART3_MAX_RECV_LEN个字节.

UART_HandleTypeDef UART3_Handler;                         //UART句柄

//初始化IO,串口3
//bound:波特率
void usart3_init(u32 bound)
{	
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_GPIOB_CLK_ENABLE();			              //使能GPIOB时钟
	__HAL_RCC_USART3_CLK_ENABLE();			              //使能USART1时钟

	//UART 初始化设置
	UART3_Handler.Instance=USART1;					      //USART1
	UART3_Handler.Init.BaudRate=bound;				      //波特率
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;     //字长为8位数据格式
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	      //一个停止位
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		      //无奇偶校验位
	UART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;     //无硬件流控
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		      //收发模式
	HAL_UART_Init(&UART3_Handler);					      //HAL_UART_Init()会使能UART3	

	GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_11;			  //PB10、PB11
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		              //复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;			              //上拉
	GPIO_Initure.Speed=GPIO_SPEED_FAST;		              //高速
	GPIO_Initure.Alternate=GPIO_AF7_USART3;	              //复用为USART1
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);	            	  //初始化PB10,和PB11
	
	__HAL_UART_DISABLE_IT(&UART3_Handler,UART_IT_TC);
#if USART3_RX_EN 
	__HAL_UART_ENABLE_IT(&UART3_Handler,UART_IT_RXNE);    //开启接收中断
	HAL_NVIC_EnableIRQ(USART3_IRQn);				      //使能USART1中断通道
	HAL_NVIC_SetPriority(USART3_IRQn,2,3);			      //抢占优先级2，子优先级3
#endif	
	TIM7_Init(100-1,9000-1);                              //重新设置为10ms中断	
	HAL_TIM_Base_Stop(&TIM7_Handler);                     //关闭定时器7
	USART3_RX_STA=0;				                      //清零 
	
  
}

//timer=10ms
//通过判断接收连续2个字符之间的时间差不大于timer来决定是不是一次连续的数据.
//如果2个字符接收间隔超过timer,则认为不是1次连续数据.也就是超过timer没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
u16 USART3_RX_STA=0;  

//串口3中断服务程序
void USART3_IRQHandler(void)
{
	  u8 Res;
	  if((__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_RXNE)!=RESET))
	  {
		  HAL_UART_Receive(&UART3_Handler,&Res,1,1000);
		  if((USART3_RX_STA&0x8000)==0)                //接收完的一批数据,还没有被处理,则不再接收其他数据
		  { 
			 if(USART3_RX_STA<USART3_MAX_RECV_LEN)     //还可以接收数据
			 {
				 
				 if(!Lora_mode)//配置模式下(启动定时器超时)
				 {
					  TIM7->CNT = 0;                       //计数器清空
					  if(USART3_RX_STA==0)                 //如果没有接收任何数据
					  {
						 HAL_TIM_Base_Start(&TIM7_Handler);//开启定时器7 
					  }
					 
				 }
				 USART3_RX_BUF[USART3_RX_STA++]=Res;  //记录接收到的值
				  
			 }
			 else
			 {
				  USART3_RX_STA|=1<<15;			       //强制标记接收完成
			 }
		  }
	  }
//	  HAL_UART_IRQHandler(&UART3_Handler);	
}

//串口3,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void u3_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF);//此次发送数据的长度
	for(j=0;j<i;j++)                     //循环发送数据
	{
		while((USART1->SR&0X40)==0);     //循环发送,直到发送完毕   
		USART1->DR=USART3_TX_BUF[j];  
	}
}

//串口3波特率和校验位配置
//bps:波特率（1200~115200）
//parity:校验位（无、偶、奇）
void usart3_set(u8 bps,u8 parity)
{

    static u32 bound=0;
	
	switch(bps)
	{
		case LORA_TTLBPS_1200:   bound=1200;     break;
		case LORA_TTLBPS_2400:   bound=2400;     break;
		case LORA_TTLBPS_4800:   bound=4800;     break;
		case LORA_TTLBPS_9600:   bound=9600;     break;
		case LORA_TTLBPS_19200:  bound=19200;    break;
		case LORA_TTLBPS_38400:  bound=38400;    break;
		case LORA_TTLBPS_57600:  bound=57600;    break;
		case LORA_TTLBPS_115200: bound=115200;   break;
	}
    
   
    __HAL_UART_DISABLE(&UART3_Handler);//关闭串口
	
	UART3_Handler.Init.BaudRate = bound;
	UART3_Handler.Init.StopBits = UART_STOPBITS_1; 
	
	if(parity==LORA_TTLPAR_8N1)//无校验
	{
		UART3_Handler.Init.WordLength= UART_WORDLENGTH_8B;    
		UART3_Handler.Init.Parity = UART_PARITY_NONE;
	}else if(parity==LORA_TTLPAR_8E1)//偶校验
	{
		UART3_Handler.Init.WordLength= UART_WORDLENGTH_9B;    
		UART3_Handler.Init.Parity = UART_PARITY_EVEN;
	}else if(parity==LORA_TTLPAR_8O1)//奇校验
	{
		UART3_Handler.Init.WordLength = UART_WORDLENGTH_9B;    
		UART3_Handler.Init.Parity = UART_PARITY_ODD;
	}
    HAL_UART_Init(&UART3_Handler);	
	
} 

//串口接收使能控制
//enable:0,关闭 1,打开
void usart3_rx(u8 enable)
{
	
	 __HAL_UART_DISABLE(&UART3_Handler); //失能串口 
	
	 if(enable)
	 {
		 UART3_Handler.Init.Mode=UART_MODE_TX_RX;//收发模式
	 }else
	 {
		  UART3_Handler.Init.Mode = UART_MODE_TX;//只发送  
	 }
	 
	 HAL_UART_Init(&UART3_Handler); //HAL_UART_Init()会使能UART3
	
}

#endif













