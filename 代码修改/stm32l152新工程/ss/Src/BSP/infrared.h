#ifndef __INFRARED_H
#define __INFRARED_H
#ifdef __cplusplus
extern "C" {
#endif
#include "stm32l1xx.h"

/**********************红外按键**********************/
#define  IR_Fun1    0x01fe45ba      //功能1键设置分站
#define  IR_Fun2    0x01ff847a      //功能2键设置传感器
#define  IR_Up      0x01fed12e
#define  IR_Down    0x01ff609e
#define  IR_OK      0x01fe21de
#define  IR_Cancel  0x01feb54a
#define	 IR_CODE_LEN	 33

// NEC格式红外编码,为了简单,只判断用户码,不判断机器码和反码. 1.125ms 表示11
//	2.25ms 表示22   13.5ms表示135
#define NEC_lead  135	// NEC引导码   13.5ms
#define NEC_H	    22    // NEC红外码1  2.25ms
#define NEC_L	    11    // NEC红外码0  1.125ms

#define  IR_STATE_NO	   0	// 无码值
#define	 IR_STATE_READ	 1	// 读取码值
#define	 IR_STATE_END	   2	// 读取结束
#define	 IR_STATE_OK	   3	// 码值ok
/* Private macro -------------------------------------------------------------*/

uint32_t IR_KEY_Scan(void);
void Infrared_Config(uint32_t arr,uint16_t psc);
uint8_t IR_Process(void);

#endif


/*******************************************************************************/
