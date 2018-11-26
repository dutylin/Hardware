/*===========================================================================
网址 ：http://yhmcu.taobao.com/
作者 ：李勇  原 亿和电子工作室  现 亿佰特电子科技有限公司
邮件 ：yihe_liyong@126.com
电话 ：18615799380
===========================================================================*/

#ifndef _BSP_H_
#define _BSP_H_

#include "STM8l15x_conf.h"
#include "sx1276.h"
//#include "MyTypeDef.h"

// SPI引脚定义 SCLK(PB5), MOSI(PB6), MISO(PB7)
#define PORT_SPI        GPIOB
#define PIN_SCLK        GPIO_Pin_5
#define PIN_MOSI        GPIO_Pin_6
#define PIN_MISO        GPIO_Pin_7

// LED和KEY引脚定义，LED(PB0), KEY(PC1)
#define version_a	0

//#if 0
#define PORT_LED        GPIOD
#define PIN_LED         GPIO_Pin_0
//#if 1
//#define PORT_LED        GPIOC
//#define PIN_LED         GPIO_Pin_0
//#endif 


#define LORA_AUX_PORT       GPIOB
#define LORA_AUX            GPIO_Pin_0

#define LORA_M0_PORT       GPIOB
#define LORA_M0            GPIO_Pin_2

#define LORA_M1_PORT       GPIOB
#define LORA_M1            GPIO_Pin_1

// LED操作函数，(ON)打开, (OFF)关闭，(TOG)翻转
#define LED_ON()        GPIO_ResetBits(PORT_LED, PIN_LED)
#define LED_OFF()       GPIO_SetBits(PORT_LED, PIN_LED)
#define LED_TOG()       GPIO_ToggleBits(PORT_LED, PIN_LED)

typedef unsigned char  INT8U;
typedef signed   char  INT8S;
typedef unsigned int   INT16U;
typedef signed   int   INT16S;
typedef unsigned long  INT32U;
typedef signed   long  INT32S;

void SClK_Initial(void);                // 初始化系统时钟，系统时钟 = 16MHZ
void GPIO_Initial(void);                // 初始化通用IO端口
void SPI_Initial(void);                 // 初始化SPI
INT8U SPI_ExchangeByte(INT8U input);    // 通过SPI进行数据交换

#endif //_BSP_H_

/*===========================================================================
-----------------------------------文件结束----------------------------------
===========================================================================*/
