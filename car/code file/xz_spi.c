

#include "xz_spi.h"
#include "xz_cc1121_init.h"

//不同极性下的SCLK操作
#if UTIL_SPI_CPOL==1  //
#define UTIL_SPI_SCLK_ACTIVE()    UTIL_SPI_SCLK_LOW() 
#define UTIL_SPI_SCLK_INACTIVE()  UTIL_SPI_SCLK_HIGH()
#else
#define UTIL_SPI_SCLK_ACTIVE()    UTIL_SPI_SCLK_HIGH()  
#define UTIL_SPI_SCLK_INACTIVE()  UTIL_SPI_SCLK_LOW()
#endif

/*!
*******************************************************************************
@brief  SPI 4线IO口初始化，包括CS,MISO,MOSI,SCLK
@param  void  无
@return void  无
*******************************************************************************
*/
void UTIL_SPI_4W_Init(void)
{
  BOARD_CC1121_GPIO_Init();
}

/*!
*******************************************************************************
@brief  SPI 3线移位输出，读取一个字节，不包括CS操作
@param  uint8 data  SPI移位输出的数据
@return uint8 返回SPI移位读取的数据
*******************************************************************************
*/
#if UTIL_SPI_MOST_BIT_FIRST==1
OPTI_S uint8 UTIL_SPI_3W_ReadWriteUint8(uint8 data)   //移位顺序为高BIT最先移出
{
  uint8_least i;  //这是使用uint8_least,作用是在不同的平台下，以最快的变量类型进行循环运算
  uint8 temp;
  
  temp = 0;
  
  //CHPA为0，SCLK第0个变沿（即CS下降沿，且SPI从机就绪）时，移出数据位，延时半个周期 
  //SCLK第1个变沿时，读入数据位，延时半个周期，SCLK第二次变沿
#if UTIL_SPI_CPHA!=1         
  for(i = 0;i < 8;i ++)
  {   
    //------------------------------------------------------------------------------    
    if(data & 0x80)              //SCLK第0个变沿,移出数据位 
    {
      UTIL_SPI_MOSI_HIGH();             
    }
    else
    {
      UTIL_SPI_MOSI_LOW();           
    }
    //------------------------------------------------------------------------------    
    
    //------------------------------------------------------------------------------    
    //延时半个周期
    UTIL_SPI_SCLK_DELAY() ;
    temp <<= 1;                            //两个移位运算，分别放在两个半周期等待处，平衡SCLK高低电平时间
    //------------------------------------------------------------------------------    
    
    
    
    //------------------------------------------------------------------------------    
    //SCLK第1个变沿时，读入数据位 
    UTIL_SPI_SCLK_ACTIVE();                       
    if(UTIL_SPI_MISO_READ()) 
    {
      temp |= 1U<<0; 
    }
    //------------------------------------------------------------------------------   
    
    
    //------------------------------------------------------------------------------
    //延时半个周期 
    UTIL_SPI_SCLK_DELAY() ;                
    data <<= 1;                           //两个移位运算，分别放在两个半周期等待处，平衡SCLK高低电平时间
    //------------------------------------------------------------------------------    
    
    
    //------------------------------------------------------------------------------
    //SCLK第2个变沿输出数据位（输出数据位在下一个循环开始）
    UTIL_SPI_SCLK_INACTIVE();
    //------------------------------------------------------------------------------     
  } 
  
#else 
  
  //CHPA为1，先延时半个周期，SCLK第1个变沿时，移出数据位  
  //延时半个周期，SCLK第2个变沿时，读入数据位 
  for(i = 0;i < 8;i ++)
  {   
    
    //------------------------------------------------------------------------------
    //延时半个周期   
    UTIL_SPI_SCLK_DELAY() ; 
    temp <<= 1;                  //两个移位运算，分别放在两个半周期等待处，平衡SCLK高低电平时间
    //------------------------------------------------------------------------------    
    
    //------------------------------------------------------------------------------    
    if(data & 0x80)              //SCLK第1个变沿,移出数据位 
    {
      UTIL_SPI_MOSI_HIGH();             
    }
    else
    {
      UTIL_SPI_MOSI_LOW();           
    }
    UTIL_SPI_SCLK_ACTIVE();    //SCLK第1个变沿 
    //------------------------------------------------------------------------------    
    
    
    //------------------------------------------------------------------------------    
    //延时半个周期
    UTIL_SPI_SCLK_DELAY() ;
    data <<= 1;                  //两个移位运算，分别放在两个半周期等待处，平衡SCLK高低电平时间
    //------------------------------------------------------------------------------    
    
    
    //------------------------------------------------------------------------------    
    //SCLK第2个变沿时，读入数据位 
    if(UTIL_SPI_MISO_READ()) 
    {
      temp |= 1U<<0; 
    }
    UTIL_SPI_SCLK_INACTIVE();   //SCLK第2个变沿
    //------------------------------------------------------------------------------     
  } 
  
#endif
  
  return temp;
}

#else

OPTI_S uint8 UTIL_SPI_3W_ReadWriteUint8(uint8 data)   //移位顺序为低BIT最先移出
{
  uint8_least i;  //这是使用uint8_least,作用是在不同的平台下，以最快的变量类型进行循环运算
  uint8 temp;
  
  temp = 0;
  
    //CHPA为0，SCLK第0个变沿（即CS下降沿，且SPI从机就绪）时，移出数据位，延时半个周期 
  //SCLK第1个变沿时，读入数据位，延时半个周期，SCLK第二次变沿
#if UTIL_SPI_CPHA!=1         
  for(i = 0;i < 8;i ++)
  {   
    //------------------------------------------------------------------------------    
    if(data & 0x01)              //SCLK第0个变沿,移出数据位 
    {
      UTIL_SPI_MOSI_HIGH();             
    }
    else
    {
      UTIL_SPI_MOSI_LOW();           
    }
    //------------------------------------------------------------------------------    
    
    
    
    //------------------------------------------------------------------------------    
    //延时半个周期
    UTIL_SPI_SCLK_DELAY() ;
    temp >= 1;                            //两个移位运算，分别放在两个半周期等待处，平衡SCLK高低电平时间
    //------------------------------------------------------------------------------    
    
    
    
    //------------------------------------------------------------------------------    
    //SCLK第1个变沿时，读入数据位 
    UTIL_SPI_SCLK_ACTIVE();                       
    if(UTIL_SPI_MISO_READ()) 
    {
      temp |= 1U<<7; 
    }
    //------------------------------------------------------------------------------   
    
    
    //------------------------------------------------------------------------------
    //延时半个周期 
    UTIL_SPI_SCLK_DELAY() ;                
    data >>= 1;                           //两个移位运算，分别放在两个半周期等待处，平衡SCLK高低电平时间
    //------------------------------------------------------------------------------    
    
    
    //------------------------------------------------------------------------------
    //SCLK第2个变沿输出数据位（输出数据位在下一个循环开始）
    UTIL_SPI_SCLK_INACTIVE();
    //------------------------------------------------------------------------------     
  } 
  
#else 
  
  //CHPA为1，先延时半个周期，SCLK第1个变沿时，移出数据位  
  //延时半个周期，SCLK第2个变沿时，读入数据位 
  for(i = 0;i < 8;i ++)
  {   
    
    //------------------------------------------------------------------------------
    //延时半个周期   
    UTIL_SPI_SCLK_DELAY() ; 
    temp >>= 1;                  //两个移位运算，分别放在两个半周期等待处，平衡SCLK高低电平时间
    //------------------------------------------------------------------------------    
    
    //------------------------------------------------------------------------------    
    if(data & 0x01)              //SCLK第1个变沿,移出数据位 
    {
      UTIL_SPI_MOSI_HIGH();             
    }
    else
    {
      UTIL_SPI_MOSI_LOW();           
    }
    UTIL_SPI_SCLK_ACTIVE();    //SCLK第1个变沿 
    //------------------------------------------------------------------------------    
    
    
    //------------------------------------------------------------------------------    
    //延时半个周期
    UTIL_SPI_SCLK_DELAY() ;
    data >>= 1;                  //两个移位运算，分别放在两个半周期等待处，平衡SCLK高低电平时间
    //------------------------------------------------------------------------------    
    
    
    //------------------------------------------------------------------------------    
    //SCLK第2个变沿时，读入数据位 
    if(UTIL_SPI_MISO_READ()) 
    {
      temp |= 1U<<7; 
    }
    UTIL_SPI_SCLK_INACTIVE();   //SCLK第2个变沿
    //------------------------------------------------------------------------------     
  } 
  
#endif
  
  return temp;
}
#endif


/*!
*******************************************************************************
@brief  SPI 3线写入NByte
@param  const uint8 *data  SPI写入数据的起始地址
@param  uint8 size         SPI写入数据长度
@return void               无
*******************************************************************************
*/
OPTI_S void UTIL_SPI_3W_WriteNByte(const uint8 *data,uint8 size)
{
   for(uint8_least i=0;i<size;i++)
   {
     UTIL_SPI_3W_ReadWriteUint8(data[i]);
   }
}


/*!
*******************************************************************************
@brief  SPI 3线读取NByte
@param  uint8 *data      SPI读取数据存储起始地址
@param  uint8 size       SPI读取数据长度
@return void             无
*******************************************************************************
*/
OPTI_S void UTIL_SPI_3W_ReadNByte(uint8 *data,uint8 size)
{
   for(uint8_least i=0;i<size;i++)
   {
     data[i]=UTIL_SPI_3W_ReadWriteUint8(0xFF);
   }
}



/*!
*******************************************************************************
@brief  SPI移位输出输入前，CS下降沿，包括CS下降沿后可能需要的等待操作
@param  void 无
@return void 无
*******************************************************************************
*/
OPTI_S void UTIL_SPI_CS_Begin(void)
{
  
  UTIL_SPI_CS_PREV_BEGIN();  
  
  UTIL_SPI_CS_LOW();  
  
  UTIL_SPI_CS_POST_FALL_DELAY();
}

/*!
*******************************************************************************
@brief  SPI移位输出输入后，CS上升沿，包括CS上升沿前后可能需要的等待操作
@param  void 无
@return void 无
*******************************************************************************
*/
OPTI_S void UTIL_SPI_CS_End(void)
{
  UTIL_SPI_CS_PREV_RISE_DELAY();
  
  UTIL_SPI_CS_HIGH();
  
  UTIL_SPI_CS_POST_RISE_DELAY();
  UTIL_SPI_CS_POST_END();
}


/*!
*******************************************************************************
@brief  SPI 3线移位输出，读取一个字节，包括CS操作
@param  uint8 data  SPI移位输出的数据
@return uint8 返回SPI移位读取的数据
*******************************************************************************
*/
OPTI_S uint8 UTIL_SPI_4W_ReadWriteUint8(uint8 data)
{
  uint8 temp;
  UTIL_SPI_CS_PREV_BEGIN();
  //UTIL_SPI_CS_LOW();
  UTIL_SPI_CS_POST_FALL_DELAY();
  
  temp=UTIL_SPI_3W_ReadWriteUint8(data);
  
  UTIL_SPI_CS_PREV_RISE_DELAY();
  //UTIL_SPI_CS_HIGH();
  UTIL_SPI_CS_POST_RISE_DELAY();
  UTIL_SPI_CS_POST_END();
  
  return temp;
}


/*!
*******************************************************************************
@brief  SPI 4线写入NByte
@param  const uint8 *data  SPI写入数据的起始地址
@param  uint8 size         SPI写入数据长度
@return void               无
*******************************************************************************
*/
OPTI_S void UTIL_SPI_4W_WriteNByte(const uint8 *data,uint8 size)
{
   for(uint8_least i=0;i<size;i++)
   {
     UTIL_SPI_4W_ReadWriteUint8(data[i]);
   }
}


/*!
*******************************************************************************
@brief  SPI 4线读取NByte
@param  uint8 *data      SPI读取数据存储起始地址
@param  uint8 size       SPI读取数据长度
@return void             无
*******************************************************************************
*/
OPTI_S void UTIL_SPI_4W_ReadNByte(uint8 *data,uint8 size)
{
   for(uint8_least i=0;i<size;i++)
   {
     data[i]=UTIL_SPI_4W_ReadWriteUint8(0xFF);
   }
}


/*!
*******************************************************************************
@brief  SPI SCLK空闲
@param  void  无
@return void  无
*******************************************************************************
*/
OPTI_S void UTIL_SPI_SCLK_InActive(void)
{
  UTIL_SPI_SCLK_INACTIVE();
}

/*!
*******************************************************************************
@brief  BOARD_CC1121_GPIO_Init
@param  void  无
@return void  无
*******************************************************************************
*/


void  BOARD_CC1121_GPIO_Init(void)
{
   RCC_APB2PeriphClockCmd(
                         RCC_APB2Periph_GPIOA                           
                           |RCC_APB2Periph_GPIOC,                               
                              ENABLE);
  
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = RF_SPI_NSS|RF_SPI_SCK|RF_SPI_MOSI;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
  
  GPIO_SetBits(GPIOA,RF_SPI_NSS|RF_SPI_MOSI); 
  GPIO_ResetBits(GPIOA,RF_SPI_SCK);
  
  GPIO_InitStructure.GPIO_Pin = RF_SPI_MISO;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入，高阻
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);   

  GPIO_InitStructure.GPIO_Pin = RF_SPI_RST;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);  
  
  GPIO_SetBits(GPIOC,RF_SPI_RST); 
  
  GPIO_InitStructure.GPIO_Pin = RF_DATA_IRQ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //  高阻输入
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);   
}


void   RF_RECIVE_DATA_GPIO_Init(void)
{
  
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
  
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);
  
  EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;  
  EXTI_Init(&EXTI_InitStructure); //初始化中断线参数
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; //
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; //
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
}



void __ISR_EXTI9_5_IRQn(void)
{
  EXTI_ClearITPendingBit(EXTI_Line5);
  RF_RECEIVE_DATA.rf_receive_end_flag=true;
  asm("nop");
}

