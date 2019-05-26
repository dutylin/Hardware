

#ifndef UART_H
#define UART_H


#include "stdio.h"
#include <stdarg.h>


#define BUFFERSIZE	32


#define UART1_PORT		3
#define UART1_TXD		5
#define UART1_RXD		6
#ifdef _RAISONANCE_
#define PUTCHAR_PROTOTYPE int putchar (char c)
#define GETCHAR_PROTOTYPE int getchar (void)
#elif defined (_COSMIC_)
#define PUTCHAR_PROTOTYPE char putchar (char c)
#define GETCHAR_PROTOTYPE char getchar (void)
#else /* _IAR_ */
#define PUTCHAR_PROTOTYPE int putchar (int c)
#define GETCHAR_PROTOTYPE int getchar (void)
#endif /* _RAISONANCE_ */



void USART_SendStr(unsigned char *Str); 
 void UART_Config(void);
 
// #ifdef _IAR_
//int fputc(int ch, FILE *f);
//#endif

 
#endif