#include <stdio.h>
#include "stm32f0xx_hal.h"
#include "main.h"

extern UART_HandleTypeDef huart2;

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
//int fputc(int ch, FILE *f)
//{     
// uint8_t temp[1];
//  temp[0]=ch;	
//   HAL_UART_Transmit(&huart2, temp, 1, 2);
//}
#endif 
