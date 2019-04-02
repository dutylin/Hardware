#include "global.h"
#include "spi.h"

/**************************************************************
函数：static void SPI_Rcc_Configuration(void)
功能：SPI时钟配置
**************************************************************/
static void SPI_Rcc_Configuration(void)
{
    RCC->AHBENR |= RCC_AHBPeriph_GPIOB;
    RCC->APB1ENR |= RCC_APB1Periph_SPI2;
}
/**************************************************************
函数：static void SPI_GPIO_Config(void)
功能：SPI1/2配置
**************************************************************/
static void SPI_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13,GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);
    /*********************SPI2配置脚数码管***********************/
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

}
/**************************************************************
函数：void SPI_Configuration(void)
功能：SPI初始化
**************************************************************/
void SPI_Configuration(void)
{
    //SPI_InitTypeDef SPI_InitStruct;
    
    /*SPI_GPIO_Config();
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
    SPI_InitStruct.SPI_Direction= SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStruct);
    SPI_Cmd(SPI2,ENABLE);*/
		  SPI_InitTypeDef  SPI_InitStructure;
	  GPIO_InitTypeDef GPIO_InitStructure;
    //SPI_Rcc_Configuration();
	  RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE);

	  GPIO_PinAFConfig(GPIOB, GPIO_PinSource13,GPIO_AF_SPI2);//救命的语句啊
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);
	  /* SPI2引脚定义  PB13--SPI2_SCK  PB15--SPI2_MOSI */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15 ;
	  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //推挽输出
	  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
	  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
	  /* PB12--OLED RST  引脚定义 */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12  ;
	  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
	  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
      /* PC6--OLED C/D 引脚定义 */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6  ;
	  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
				GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
	  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);
	  /* 配置SPI2 OLED片选  PB0 */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			  					 
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;		   //输出模式最大速度50MHz
   // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		   //通用推挽输出模式
	 		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
	  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	  /* SPI2 Config -------------------------------------------------------------*/
	
	  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;   //SPI_Direction_2Lines_FullDuplex;
		//SPI_InitStructure.SPI_Direction= SPI_Direction_2Lines_FullDuplex;
	  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
		//SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	  SPI_InitStructure.SPI_CRCPolynomial = 7;
	  SPI_Init(SPI2, &SPI_InitStructure);
	  /* Enable SPI2 */
	  SPI_Cmd(SPI2, ENABLE);
}
/**************************************************************
函数：uint16_t SPI2_Receive_byte(void)
功能：SPI2接收字节
**************************************************************/
uint16_t SPI2_ReadSend_byte(uint16_t data)
{
    while((SPI2->SR & SPI_I2S_FLAG_TXE)==(uint16_t)RESET);
    SPI2->DR =data;
    while((SPI2->SR & SPI_I2S_FLAG_RXNE)==(uint16_t)RESET);
    return SPI2->DR;
}



