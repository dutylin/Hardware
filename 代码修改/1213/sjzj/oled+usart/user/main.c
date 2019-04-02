#include "stm32f10x_can.h"
#include "stm32f10x.h"
#include "platform_config.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "stdarg.h"
#include "stm32f10x_spi.h"
#include "oled.h"


#define CAN_BAUDRATE 500
/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;


__IO uint32_t rec_ff = 0; /* for return of the interrupt handling */
volatile TestStatus TestRx;
int rec_u = 0;
/* Private functions ---------------------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
TestStatus CAN_Polling(void);
TestStatus CAN_Interrupt(void);

static __IO uint32_t TimingDelay;
void RCC_Configuration(void);
void Delay(__IO uint32_t nTime);
extern void Draw_BMP(unsigned char x0, unsigned char y0,unsigned char x1,unsigned char y1,const unsigned char * BMP);
extern unsigned char BMP[];
extern unsigned char fd0[];
extern unsigned char fd1[];
extern unsigned char fd2[];
extern unsigned char fd3[];
extern unsigned char fd4[];
extern char sss;
char r[5];
char m[5];
char n[5];
int ii,k;
extern void LCD_P6x8Str(unsigned char x, unsigned char y,unsigned char ch[]);
extern void LCD_P8x16Str(unsigned char x, unsigned char y,unsigned char ch[]);
extern void LCD_P16x16Ch(unsigned char x, unsigned char y, unsigned char N, const unsigned char * ch);
////////////SPI2接口配置/////////////////////////////////////////////////////
void SPI_Configuration(void)
{
	  SPI_InitTypeDef  SPI_InitStructure;
	  GPIO_InitTypeDef GPIO_InitStructure;

	  RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE);

	  /* SPI2引脚定义  PB13--SPI2_SCK  PB15--SPI2_MOSI */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15 ;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
	  /* PB12--OLED RST  引脚定义 */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12  ;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
      /* PC6--OLED C/D 引脚定义 */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6  ;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);
	  /* 配置SPI2 OLED片选  PB0 */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			  					 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   //输出模式最大速度50MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		   //通用推挽输出模式
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	  /* SPI2 Config -------------------------------------------------------------*/
	  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;   //SPI_Direction_2Lines_FullDuplex;
	  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	  SPI_InitStructure.SPI_CRCPolynomial = 7;
	  SPI_Init(SPI2, &SPI_InitStructure);
	  /* Enable SPI2 */
	  SPI_Cmd(SPI2, ENABLE);
}

void RCC_Configuration(void){

  SystemInit();   
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO |RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE
  						, ENABLE);
}
/****************************************************************************
* 名    称：void Delay(__IO uint32_t nTime)
* 功    能：定时延时程序 1ms为单位
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/  
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}
/****************************************************************************
* 名    称：void TimingDelay_Decrement(void)
* 功    能：获取节拍程序
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/  
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
uint8_t TxBuffer1[] = "USART Interrupt Example: This is USART1 DEMO";  
uint8_t RxBuffer1[],tx_flag;
extern uint8_t rec_f;
__IO uint8_t TxCounter1 = 0x00;
__IO uint8_t RxCounter1 = 0x00; 

uint32_t Rec_Len;


/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);

void Delay(__IO uint32_t nCount);
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...);
char *itoa(int value, char *string, int radix);
void USART_Config(USART_TypeDef* USARTx);


GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStruct;
USART_ClockInitTypeDef USART_ClockInitStruct;

/****************************************************************************
* 名    称：void ili9325_DrawPicture(u16 StartX,u16 StartY, u8 Dir，u8 *pic)
* 功    能：在指定座标范围显示一副图片
* 入口参数：StartX     行起始座标
*           StartY     列起始座标
*			Dir		   图像显示方向       
*           pic        图片头指针
* 出口参数：无
* 说    明：图片取模格式为水平扫描，16位颜色模式  取模软件img2LCD
* 调用方法：ili9325_DrawPicture(0,0,0,(u16*)demo);
****************************************************************************/
void USART_Config(USART_TypeDef* USARTx){
  USART_InitStructure.USART_BaudRate = 9600;						//速率9600bps
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位8位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位1位
  USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //无硬件流控
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式

  /* Configure USART1 */
  USART_Init(USARTx, &USART_InitStructure);							//配置串口参数函数
 
  
  /* Enable USART1 Receive and Transmit interrupts */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                    //使能接收中断
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);						//使能发送缓冲空中断   

  /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);	
}
int Value[5]= {0xFF,0xFF,0x01,0xAA,0xBB};
void CC1110_Sendd(void)
{
    int i;
    USART1->SR = (uint16_t)~0x0040;
    for(i=0; i<5; i++)
    {
        USART1->DR = (*(Value+i) & (uint16_t)0x01FF);
        while((USART1->SR & 0x0080) == RESET); //发送完成标志
    }
}	
/****************************************************************************
* 名    称：int main(void)
* 功    能：奋斗板主函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
int main(void)
{

   uint8_t a=0;
  /* System Clocks Configuration */
  RCC_Configuration();											  //系统时钟设置
       
  /*嵌套向量中断控制器 
      说明了USART1抢占优先级级别0（最多1位） ，和子优先级级别0（最多7位） */ 
  NVIC_Configuration();											  //中断源配置

  /*对控制LED指示灯的IO口进行了初始化，将端口配置为推挽上拉输出，口线速度为50Mhz。PA9,PA10端口复用为串口1的TX，RX。
  在配置某个口线时，首先应对它所在的端口的时钟进行使能。否则无法配置成功，由于用到了端口B， 因此要对这个端口的时钟
  进行使能，同时由于用到复用IO口功能用于配置串口。因此还要使能AFIO（复用功能IO）时钟。*/
  GPIO_Configuration();											  //端口初始化

	 //RCC_Configuration();	           		//系统时钟设置及各外设时钟使能
   if (SysTick_Config(3600))		    //时钟节拍中断时50us一次  用于定时 
   { 
     Capture error */ 
     while (1);
   }
   SPI_Configuration();		  
   Select_OLED
   LCD_Init();

  USART_Config(USART1);											  //串口1初始化
  
   while (1)
  {
		LCD_CLS();
		LCD_P16x16Ch(0,0,8,fd1);  
		//LCD_P16x16Ch(16,2,2,fd1);  //
		//LCD_P16x16Ch(16,4,2,fd2);  //
		//LCD_P16x16Ch(16,6,2,fd3);  //
		//LCD_P8x16Str(64,2,"24.5"); 
		//LCD_P8x16Str(64,4,"12.5"); 
		//LCD_P8x16Str(64,6,"6.2"); 
		LCD_P8x16Str(10,2,r);
		LCD_P8x16Str(50,2,m);
		LCD_P8x16Str(100,2,n);
		Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);
		Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);
		Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);
		Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);

		//CC1110_Sendd();
		TestRx = CAN_Interrupt();
		//LCD_P8x16Str(64,6,array="RxBuffer1[RxCounter1-2]");
	    //LCD_P16x16Ch(56,0,1,fd0);  //
		//LCD_P16x16Ch(16,2,2,fd1);  //
	if(rec_f==1){												  //判断是否收到一帧有效数据
		rec_f=0;
		//USART_OUT(USART1,"\r\n您发送的信息为: \r\n");    
		//USART_OUT(USART1,&TxBuffer1[0]);
		CC1110_Sendd();
		//TestRx = CAN_Interrupt();
		if(a==0) {GPIO_SetBits(GPIOB, GPIO_Pin_5); a=1;}          //LED1  V6（V3板） V2（MINI板） 明暗闪烁                
		else {GPIO_ResetBits(GPIOB, GPIO_Pin_5);a=0;  }
	}
  }
}

/****************************************************************************
* 名    称：void Delay(__IO uint32_t nCount)
* 功    能：延时函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
//void Delay(__IO uint32_t nCount)
//{
 // for(; nCount != 0; nCount--);
//}

/****************************************************************************
* 名    称：void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...)
* 功    能：格式化串口输出函数
* 入口参数：USARTx:  指定串口
			Data：   发送数组
			...:     不定参数
* 出口参数：无
* 说    明：格式化串口输出函数
        	"\r"	回车符	   USART_OUT(USART1, "abcdefg\r")   
			"\n"	换行符	   USART_OUT(USART1, "abcdefg\r\n")
			"%s"	字符串	   USART_OUT(USART1, "字符串是：%s","abcdefg")
			"%d"	十进制	   USART_OUT(USART1, "a=%d",10)
* 调用方法：无 
****************************************************************************/
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...){ 
	const char *s;
    int d;
    char buf[16];
    va_list ap;
    va_start(ap, Data);

	while(*Data!=0){				                          //判断是否到达字符串结束符
		if(*Data==0x5c){									  //'\'
			switch (*++Data){
				case 'r':							          //回车符
					USART_SendData(USARTx, 0x0d);	   

					Data++;
					break;
				case 'n':							          //换行符
					USART_SendData(USARTx, 0x0a);	
					Data++;
					break;
				
				default:
					Data++;
				    break;
			}
			
			 
		}
		else if(*Data=='%'){									  //
			switch (*++Data){				
				case 's':										  //字符串
                	s = va_arg(ap, const char *);
                	for ( ; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
            	case 'd':										  //十进制
                	d = va_arg(ap, int);
                	itoa(d, buf, 10);
                	for (s = buf; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
				default:
					Data++;
				    break;
			}		 
		}
		else USART_SendData(USARTx, *Data++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	}
}

/******************************************************
		整形数据转字符串函数
        char *itoa(int value, char *string, int radix)
		radix=10 标示是10进制	非十进制，转换结果为0;  

	    例：d=-379;
		执行	itoa(d, buf, 10); 后
		
		buf="-379"							   			  
**********************************************************/
char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */


void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				     //LED1控制--PB5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         		 //USART1 TX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 //复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);		    		 //A端口 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         	 //USART1 RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	 //复用开漏输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);		         	 //A端口 
}

/****************************************************************************
* 名    称：void NVIC_Configuration(void)
* 功    能：中断源配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void NVIC_Configuration(void)
{
  /*  结构声明*/
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
  /* Configure one bit for preemption priority */
  /* 优先级组 说明了抢占优先级所用的位数，和子优先级所用的位数   在这里是1， 7 */    
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	  
  
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	   //CAN1 RX0中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;		   //抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;			   //子优先级为0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			     	//设置串口1中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	     	//抢占优先级 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;				//子优先级为0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能
  NVIC_Init(&NVIC_InitStructure);
}
int k=0;
int l,v,s,w,a,b,id,z;
void USART1_IRQHandler(void)      //串口1 中断服务程序
{
  unsigned int i;
	
	//char sss;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	   //判断读寄存器是否非空
  {	   
    RxBuffer1[RxCounter1++] = USART_ReceiveData(USART1);   //将读寄存器的数据缓存到接收缓冲区里
		//int(RxBuffer1[RxCounter1-2])
		//RxBuffer1[RxCounter1-1]=(RxBuffer1[RxCounter1-2]<<8)+RxBuffer1[RxCounter1-1];
		//RxBuffer1[1] = USART_ReceiveData(USART1);   //将读寄存器的数据缓存到接收缓冲区里
		//RxBuffer1[2] = USART_ReceiveData(USART1);   //将读寄存器的数据缓存到接收缓冲区里
	  if(k==0)
		{
		 id = RxBuffer1[RxCounter1-1];
		 *itoa(id,r,10);
		 k++;
		}
		else if(k==1)
		{
     l=RxBuffer1[RxCounter1-1];
		 k++;
		}	
		else if(k==2)
		{
		 v=(l<<8)+RxBuffer1[RxCounter1-1];
		 *itoa(v,m,10);
		 k++;
		}
		else if(k==3)
		{
	   s=RxBuffer1[RxCounter1-1];
			if(s==0x07)
			{
			s=0x01;
			}
		 k++;
		}
		else if(k==4)
		{
		 w=(s<<8)+RxBuffer1[RxCounter1-1];
		 *itoa(w,n,10);
		 k++;
		}
		else if(k==5)
		{
		z=RxBuffer1[RxCounter1-1];	
		k=0;	
		}
		//k=RxBuffer1[RxCounter1-1];
    //if(RxBuffer1[RxCounter1-2]==0x0d&&RxBuffer1[RxCounter1-1]==0x0a)     //判断结束标志是否是0x0d 0x0a
		//if(RxBuffer1[RxCounter1-1]==0x02) 
    //{
	  for(i=0; i< RxCounter1; i++) TxBuffer1[i]	= RxBuffer1[i]; 	     //将接收缓冲器的数据转到发送缓冲区，准备转发
	  rec_u=1;															 //接收成功标志
	  TxBuffer1[RxCounter1]=0;		                                     //发送缓冲区结束符    
	  //RxCounter1=0;	  
    //}
  }
  
  if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)                   //这段是为了避免STM32 USART 第一个字节发不出去的BUG 
  { 
     USART_ITConfig(USART1, USART_IT_TXE, DISABLE);					     //禁止发缓冲器空中断， 
  }	
  
}

/******************* (C) COPYRIGHT 2011 奋斗STM32 *****END OF FILE****/
TestStatus CAN_Interrupt(void)
{
	
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;  
  GPIO_InitTypeDef  GPIO_InitStructure;
  CanTxMsg TxMessage;
  //CanRxMsg RxMessage;
  //uint8_t TransmitMailbox = 0;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 			      //LCD背光控制
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOD, GPIO_Pin_13);			             //LCD背光关闭	

  GPIO_PinRemapConfig(GPIO_Remap1_CAN1 , ENABLE);		     //端口复用为CAN1   

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	                 //PB8:CAN-RX 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			     //输入上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;					 //PB9:CAN-TX 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			 //复用模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* CAN寄存器初始化 */
  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN单元初始化 */
  CAN_InitStructure.CAN_TTCM=DISABLE;			   //MCR-TTCM  时间触发通信模式使能
  CAN_InitStructure.CAN_ABOM=DISABLE;			   //MCR-ABOM  自动离线管理 
  CAN_InitStructure.CAN_AWUM=DISABLE;			   //MCR-AWUM  自动唤醒模式
  //CAN_InitStructure.CAN_NART=ENABLE;			   //MCR-NART  禁止报文自动重传	  0-自动重传   1-报文只传一次
  CAN_InitStructure.CAN_NART=DISABLE;			   //MCR-NART  禁止报文自动重传	  0-自动重传   1-报文只传一次
  CAN_InitStructure.CAN_RFLM=DISABLE;			   //MCR-RFLM  接收FIFO 锁定模式  0-溢出时新报文会覆盖原有报文  1-溢出时，新报文丢弃
  CAN_InitStructure.CAN_TXFP = ENABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  //CAN_InitStructure.CAN_TXFP=DISABLE;			   //MCR-TXFP  发送FIFO优先级 0-优先级取决于报文标示符 1-优先级取决于发送请求的顺序
  //CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;	   //BTR-SILM/LBKM   CAN环回模式
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;		   //BTR-SJW 重新同步跳跃宽度 1个时间单元
  CAN_InitStructure.CAN_BS1=CAN_BS1_2tq;		   //BTR-TS1 时间段1 占用了2个时间单元
  CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;		   //BTR-TS1 时间段2 占用了3个时间单元
  
#if CAN_BAUDRATE == 1000 /* 1MBps */
  CAN_InitStructure.CAN_Prescaler =6;			   //BTR-BRP 波特率分频器  定义了时间单元的时间长度 36/(1+2+3)/6=1Mbps
#elif CAN_BAUDRATE == 500 /* 500KBps */
  CAN_InitStructure.CAN_Prescaler =12;
#elif CAN_BAUDRATE == 250 /* 250KBps */
  CAN_InitStructure.CAN_Prescaler =24;
#elif CAN_BAUDRATE == 125 /* 125KBps */
  CAN_InitStructure.CAN_Prescaler =48;
#elif  CAN_BAUDRATE == 100 /* 100KBps */
  CAN_InitStructure.CAN_Prescaler =60;
#elif  CAN_BAUDRATE == 50 /* 50KBps */
  CAN_InitStructure.CAN_Prescaler =120;
#elif  CAN_BAUDRATE == 20 /* 20KBps */
  CAN_InitStructure.CAN_Prescaler =300;
#elif  CAN_BAUDRATE == 10 /* 10KBps */
  CAN_InitStructure.CAN_Prescaler =600;
#endif  

  //CAN_InitStructure.CAN_TXFP=DISABLE;
  //CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
//  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
//  CAN_InitStructure.CAN_BS1=CAN_BS1_8tq;
//  CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;
  //CAN_InitStructure.CAN_Prescaler=1;
  CAN_Init(CAN1, &CAN_InitStructure);

  /* CAN过滤器初始化 */
  CAN_FilterInitStructure.CAN_FilterNumber=0;						//
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;		//FM1R  过滤器组0的工作模式。
  																	//0: 过滤器组x的2个32位寄存器工作在标识符屏蔽位模式； 
																	//1: 过滤器组x的2个32位寄存器工作在标识符列表模式。
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//FS1R 过滤器组0(13～0)的位宽。
  																	//0：过滤器位宽为2个16位； 1：过滤器位宽为单个32位。
  
  /* 使能报文标示符过滤器按照标示符的内容进行比对过滤，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0。 */
  CAN_FilterInitStructure.CAN_FilterIdHigh=(((u32)0x1234<<3)&0xFFFF0000)>>16;				//要过滤的ID高位 
  CAN_FilterInitStructure.CAN_FilterIdLow=(((u32)0x1234<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF;//要过滤的ID低位 
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xffff;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xffff;
  
//  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;					
//  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
//  /* 全为0，忽略了报文标识符判别 */
//  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
//  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;				//FFAx : 过滤器位宽设置 报文在通过了某过滤器的过滤后，
  																	//将被存放到其关联的FIFO中。 0：过滤器被关联到FIFO0； 1：过滤器被关联到FIFO1。
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;				//FACTx : 过滤器激活 软件对某位设置1来激活相应的过滤器。只有对FACTx位清0，
  																	//或对CAN_FMR寄存器的FINIT位设置1后，才能修改相应的过滤器寄存器
																	//x(CAN_FxR[0:1])。 0：过滤器被禁用； 1：过滤器被激活。
  CAN_FilterInit(&CAN_FilterInitStructure);

  /* CAN FIFO0 接收中断使能 */ 
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

  /* 发送一帧报文 */
  TxMessage.StdId=0x00;
  TxMessage.ExtId=id;
  TxMessage.IDE=CAN_ID_EXT;
  TxMessage.RTR=CAN_RTR_DATA;
  TxMessage.DLC=5;
  //TxMessage.Data[0]=0xDE;
	//TxMessage.Data[0]=RxBuffer1[RxCounter1-2];
	TxMessage.Data[0]=l;
	TxMessage.Data[1]=v;
	TxMessage.Data[2]=s;
	TxMessage.Data[3]=w;
	TxMessage.Data[4]=z;
  //TxMessage.Data[1]=0xCA;
	//TxMessage.Data[1]=RxBuffer1[RxCounter1-1];
	RxCounter1=0;
  if(rec_u==1)
	{												  //判断是否收到一帧有效数据
		rec_u=0;
	  CAN_Transmit(CAN1, &TxMessage);
//		CAN_Transmit(CAN1,RxBuffer1[RxCounter1-1]);
	}
  /* 等待接收成功标志置位 */
  rec_ff = 0xFF;	 
  //while(rec_ff == 0x000000FF);

  /* 接收中断禁止*/
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

  return (TestStatus)rec_ff;
}

/******************* (C) COPYRIGHT 2013 奋斗STM32 *****END OF FILE****/
