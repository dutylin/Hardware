#ifndef  __GLOBAL_H
#define __GLOBAL_H
#ifdef __cplusplus
extern "C" {
#endif

#ifdef GLOBALS
#define EXT
#else
#define EXT extern
#endif
#define DEBUG  0
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "stm32l1xx.h"
#include "misc.h"
#include "system_stm32l1xx.h"
#include "stm32l1xx_adc.h"
#include "stm32l1xx_dma.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_rtc.h"
#include "stm32l1xx_exti.h"
#include "stm32l1xx_spi.h"
#include "stm32l1xx_pwr.h"
#include "stm32l1xx_usart.h"
#include "stm32l1xx_tim.h"
#include "stm32l1xx_flash.h"
#include "stm32l1xx_syscfg.h"
/************************定义发送结构体**************************/
typedef struct
{
    uint8_t Invalid[5];   //无效字节
    uint8_t Start_Flag;		//接收头标志
    uint8_t ID;						//传感器ID
    uint8_t Type;					//传感器类型
    uint8_t Channel;			//传感器通道
    uint8_t T_Data_H[4];	//传感器数据高位
    uint8_t T_Data_L[4];	//传感器数据低位
    uint8_t Battery_Power;//传感器电池
    uint8_t End_Flag;			//接收尾标志
} CC1110Tx_Msg;
/************************定义传感器属性结构体**************************/
typedef struct
{
    uint8_t Senor_ID;
    uint8_t Senor_Num;
} SENOR;

typedef enum {Low = 0, High = !Low} PDStatus,WPStatus,CCStatus;

/************************flash读写相关*************************/
EXT uint8_t CC1110_Send_Flag;
/************************电源控制相关*************************/
enum {Power_OFF=0,Power_ON=!Power_OFF};//5V电源控制标志
/************************调用函数相关*************************/
void delay_init(void);      //延时
void delay_ms(uint16_t nms);//延时毫秒
void delay_us(uint32_t nus);//延时纳秒
void Battery_Monitor_Config (void);
uint8_t Battery_Monitor(void);//电池电压监测
void Bsp_Config (void);     //硬件初始化
PDStatus PD_WAKEUP_status (void);     //光照唤醒标志
void EnterLowPower(void);
void Wake_Config(void);
void WAKEUP_Config (void);
void RCC_Config(void);
#ifdef __cplusplus
}
#endif
#endif



