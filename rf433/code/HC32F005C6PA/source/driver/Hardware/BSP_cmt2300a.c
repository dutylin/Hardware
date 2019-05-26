#include "BSP_cmt2300a.h"
void cmt2300a_GPIO_Init()
{
  GPIO_Init(SPI_PORT,CSB_PIN, GPIO_Mode_Out_PP_High_Slow);
  GPIO_Init(SPI_PORT,FCSB_PIN, GPIO_Mode_Out_PP_High_Slow);
  GPIO_Init(SPI_PORT,SCLK_PIN, GPIO_Mode_Out_PP_High_Slow);
  GPIO_Init(SPI_PORT,SDIO_PIN, GPIO_Mode_Out_PP_High_Slow);  
}
