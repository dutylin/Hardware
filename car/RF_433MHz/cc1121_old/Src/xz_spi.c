

#include "xz_spi.h"
#include "xz_cc1121_init.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx.h"
#define spi_interface
extern SPI_HandleTypeDef hspi1;
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
@param  unsigned char  data  SPI移位输出的数据
@return unsigned char  返回SPI移位读取的数据
*******************************************************************************
*/
#if UTIL_SPI_MOST_BIT_FIRST==1
unsigned char  UTIL_SPI_3W_ReadWriteUint8(unsigned char  data)   //移位顺序为高BIT最先移出
{
	unsigned char i;  //这是使用uint8_least,作用是在不同的平台下，以最快的变量类型进行循环运算
	unsigned char  temp;

	temp = 0;

	//CHPA为0，SCLK第0个变沿（即CS下降沿，且SPI从机就绪）时，移出数据位，延时半个周期
	//SCLK第1个变沿时，读入数据位，延时半个周期，SCLK第二次变沿
#if UTIL_SPI_CPHA!=1
	#ifdef spi_interface
		HAL_SPI_Transmit(&hspi1, &data,1, 2);
//		HAL_SPI_Receive(&hspi1, &data, 1, 10);
	#else
		for(i = 0; i < 8; i ++)
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

	#endif
#else
	#ifdef spi_interface
		HAL_SPI_Transmit(&hspi1, &data,1, 2);
//		HAL_SPI_Receive(&hspi1, &data, 1, 10);
	#else
	//CHPA为1，先延时半个周期，SCLK第1个变沿时，移出数据位
	//延时半个周期，SCLK第2个变沿时，读入数据位
		for(i = 0; i < 8; i ++)
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
#endif

	return temp;
}

#else

unsigned char  UTIL_SPI_3W_ReadWriteUint8(unsigned char  data)   //移位顺序为低BIT最先移出
{
	uint8_least i;  //这是使用uint8_least,作用是在不同的平台下，以最快的变量类型进行循环运算
	unsigned char  temp;

	temp = 0;

	//CHPA为0，SCLK第0个变沿（即CS下降沿，且SPI从机就绪）时，移出数据位，延时半个周期
	//SCLK第1个变沿时，读入数据位，延时半个周期，SCLK第二次变沿
#if UTIL_SPI_CPHA!=1
	#ifdef spi_interface
		HAL_SPI_Transmit(&hspi1, &data,1, 2);
//		HAL_SPI_Receive(&hspi1, &data, 1, 10);
	#else
		for(i = 0; i < 8; i ++)
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
    #endif
#else
    #ifdef spi_interface
		HAL_SPI_Transmit(&hspi1, &data,1, 2);
//		HAL_SPI_Receive(&hspi1, &data, 1, 10);
	#else
	//CHPA为1，先延时半个周期，SCLK第1个变沿时，移出数据位
	//延时半个周期，SCLK第2个变沿时，读入数据位
		for(i = 0; i < 8; i ++)
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
#endif

	return temp;
}
#endif


/*!
*******************************************************************************
@brief  SPI 3线写入NByte
@param  const unsigned char  *data  SPI写入数据的起始地址
@param  unsigned char  size         SPI写入数据长度
@return void               无
*******************************************************************************
*/
void UTIL_SPI_3W_WriteNByte(const unsigned char  *data,unsigned char  size)
{
	unsigned char i;
	for( i=0; i<size; i++)
	{
		UTIL_SPI_3W_ReadWriteUint8(data[i]);
	}
}


/*!
*******************************************************************************
@brief  SPI 3线读取NByte
@param  unsigned char  *data      SPI读取数据存储起始地址
@param  unsigned char  size       SPI读取数据长度
@return void             无
*******************************************************************************
*/
void UTIL_SPI_3W_ReadNByte(unsigned char  *data,unsigned char  size)
{
	unsigned char i;
	for( i=0; i<size; i++)
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
void UTIL_SPI_CS_Begin(void)
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
void UTIL_SPI_CS_End(void)
{
	UTIL_SPI_CS_PREV_RISE_DELAY();

	UTIL_SPI_CS_HIGH();

	UTIL_SPI_CS_POST_RISE_DELAY();
	UTIL_SPI_CS_POST_END();
}


/*!
*******************************************************************************
@brief  SPI 3线移位输出，读取一个字节，包括CS操作
@param  unsigned char  data  SPI移位输出的数据
@return unsigned char  返回SPI移位读取的数据
*******************************************************************************
*/
unsigned char  UTIL_SPI_4W_ReadWriteUint8(unsigned char  data)
{
	unsigned char  temp;
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
@param  const unsigned char  *data  SPI写入数据的起始地址
@param  unsigned char  size         SPI写入数据长度
@return void               无
*******************************************************************************
*/
void UTIL_SPI_4W_WriteNByte(const unsigned char  *data,unsigned char  size)
{
	unsigned char i;
	for( i=0; i<size; i++)
	{
		UTIL_SPI_4W_ReadWriteUint8(data[i]);
	}
}


/*!
*******************************************************************************
@brief  SPI 4线读取NByte
@param  unsigned char  *data      SPI读取数据存储起始地址
@param  unsigned char  size       SPI读取数据长度
@return void             无
*******************************************************************************
*/
void UTIL_SPI_4W_ReadNByte(unsigned char  *data,unsigned char  size)
{
	unsigned char i;
	for( i=0; i<size; i++)
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
void UTIL_SPI_SCLK_InActive(void)
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
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();


	GPIO_InitStruct.Pin =  RF_SPI_NSS|RF_SPI_SCK|RF_SPI_MOSI;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed =GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init( GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA,RF_SPI_NSS|RF_SPI_MOSI,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,RF_SPI_SCK,GPIO_PIN_RESET);



	GPIO_InitStruct.Pin = RF_SPI_MISO;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  //浮空输入，高阻
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = RF_SPI_RST;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  //浮空输入，高阻
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


	GPIO_InitStruct.Pin = RF_SPI_RST;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  //浮空输入，高阻
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA,RF_SPI_RST,GPIO_PIN_SET);

	GPIO_InitStruct.Pin = RF_DATA_IRQ;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  //浮空输入，高阻
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);


}


void   RF_RECIVE_DATA_GPIO_Init(void)
{


}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==GPIO_PIN_2)
	{
		//nIRQ=0;
		RF_RECEIVE_DATA.rf_receive_end_flag=true;
//		if(tx_rx_switch==0)
//		{
//			rf_rx_FLAG=1;
//		}
		//rf_rx_irq();
	}
}



