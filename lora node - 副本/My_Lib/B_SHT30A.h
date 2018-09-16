#ifndef _SHT30_T_H_
#define _SHT30_T_H_

/*--------------------------头文件引用--------------------------------*/
#include "DataType.h"
#include "stm8l15x.h"

/*-----------------------------结构体定义---------------------------------*/

/*-----------------------------宏定义---------------------------------*/

//#define         INT0      (0x01<<5)
#define         SCL0      GPIO_Pin_2
#define         SDA0      GPIO_Pin_1

//引脚定义
#define SHT30_SCL0_O    GPIO_Init(GPIOB, SCL0, GPIO_Mode_Out_PP_High_Fast)         //推挽输出
#define SHT30_SCL0_H    GPIO_SetBits(GPIOB,SCL0)
#define SHT30_SCL0_L    GPIO_ResetBits(GPIOB,SCL0)

#define SHT30_SCL0_I    GPIO_Init(GPIOB, SCL0, GPIO_Mode_In_FL_No_IT)       //悬浮输入
#define SHT30_SCL0_DAT  ( (GPIO_ReadInputDataBit(GPIOB, SCL0))>>2 )

#define SHT30_SDA0_O    GPIO_Init(GPIOB, SDA0, GPIO_Mode_Out_PP_High_Fast)
#define SHT30_SDA0_H    GPIO_SetBits(GPIOB,SDA0)
#define SHT30_SDA0_L   GPIO_ResetBits(GPIOB,SDA0)

#define SHT30_SDA0_I    GPIO_Init(GPIOB, SDA0, GPIO_Mode_In_FL_No_IT)       //悬浮输入       //悬浮输入
#define SHT30_SDA0_DAT  ( (GPIO_ReadInputDataBit(GPIOB, SDA0))>>1 )


#define	SHT30_SlaveAddress	  		(0x44<<1)						//7位地址0x44 左移1位 0x45 -- 0x8A

#define noACK 0                                 			//用于判断是否结束通讯 
#define ACK   1                                 			//结束数据传输 

//命令
#define SHT30_SINGLE_H_MEASURE_EN					0x2C06			
#define SHT30_SINGLE_M_MEASURE_EN					0x2C0D			
#define SHT30_SINGLE_L_MEASURE_EN					0x2C10			
#define SHT30_SINGLE_H_MEASURE_DIS				0x2400			
#define SHT30_SINGLE_M_MEASURE_DIS				0x240B			
#define SHT30_SINGLE_L_MEASURE_DIS				0x2416			

#define SHT30_PERIODOC_H_MEASURE_500MS		0x2032			
#define SHT30_PERIODOC_M_MEASURE_500MS		0x2024			
#define SHT30_PERIODOC_L_MEASURE_500MS		0x202F			
#define SHT30_PERIODOC_H_MEASURE_1S				0x2130			
#define SHT30_PERIODOC_M_MEASURE_1S				0x2126			//
#define SHT30_PERIODOC_L_MEASURE_1S				0x212D			//
#define SHT30_PERIODOC_H_MEASURE_2S				0x2236			//
#define SHT30_PERIODOC_M_MEASURE_2S				0x2220			//
#define SHT30_PERIODOC_L_MEASURE_2S				0x222B			//
#define SHT30_PERIODOC_H_MEASURE_4S				0x2334			//
#define SHT30_PERIODOC_M_MEASURE_4S				0x2322			//
#define SHT30_PERIODOC_L_MEASURE_4S				0x2329			//
#define SHT30_PERIODOC_H_MEASURE_10S			0x2737			//
#define SHT30_PERIODOC_M_MEASURE_10S			0x2721			//
#define SHT30_PERIODOC_L_MEASURE_10S			0x272A			//

#define SHT30_PERIODOC_MEASURE_READ			0xE000			//重复测量读取数据

#define	SHT30_SOFT_RESET									0x30A2			//软复位

#define SHT30_HEATER_EN										0x306D			//加热使能
#define SHT30_HEATER_DIS									0x3066			//加热关闭

#define SHT30_READ_STATUS									0xF32D			//读状态寄存器
#define SHT30_CLEAR_STATUS								0x3041			//清状态寄存器

// CRC
#define POLYNOMIAL  0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001



/*---------------------------外部变量声明-----------------------------*/

/*-----------------------------函数声明-------------------------------*/
vid SHT30_DelayMs(uint8 k);
vid SHT30_Delay1us(uint16 vTemp);
vid SHT30_Init(vid);
vid SHT30_Start(vid);                          //起始信号
vid SHT30_Stop(vid);                           //停止信号
vid SHT30_SendACK(uint8 ack);                  //应答ACK
uint8 SHT30_RecvACK(vid);                      //读ack
uint8 SHT30_SendByte(uint8 dat);               //IIC单个字节写
uint8 SHT30_RecvByte(vid);                     //IIC单个字节读
uint8 SHT30_CheckCrc8(uint8 *vDat, uint8 vLen);//CRC8 校验

uint16 SHT30_ReadSerialNumber(vid);
uint8 SHT30_Single_Measure(uint8 *vBuf);				//单次采集数据
uint8 SHT30_Periodic_Measure(uint16 vCmd);			//设置周期采集
uint8 SHT30_Periodic_Measure_Read(uint8 *vBuf);	//周期采集读取数据
uint8 SHT30_Soft_Reset(vid);												//软复位
uint8 SHT30_Heater(uint16 vCmd);								//传感器加热
uint16 SHT30_ReadStaus(vid);												//读取状态寄存器
uint8 SHT30_ClearStaus(vid);												//清楚状态寄存器
void SHT30_calc(uint8 *vBuf, uint8 *vTemSymbol, uint16 *vTem, uint16 *vHum);	//计算采集数据
void SHT30_Get_TH(uint8 *vTemSymbol, uint16 *vTem, uint16 *vHum);							//获取温湿度数据

#endif

