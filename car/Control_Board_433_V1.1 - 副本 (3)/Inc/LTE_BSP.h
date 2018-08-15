#ifndef LTE_BSP_H_
#define LTE_BSP_H_
#include "stm32f4xx_hal.h"
#define ON  1
#define OFF 0
#define MAX_STEP   5
#define buffer_size  512
#define buffer_size2  256
#define Max_Network_thres 30


void Send_Comm(unsigned char* comm,unsigned short len);
void LTE_hw_init(void);
void LET_reset(void);
void LET_power_ctrl(void);
void LET_gps_power_ctrl(unsigned char  flag);
void LTE_power_on(void);
void dma_clear_flag(void);
#endif
