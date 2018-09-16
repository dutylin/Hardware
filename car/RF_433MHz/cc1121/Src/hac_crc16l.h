#ifndef __UTIL_CRC16L_H__
#define __UTIL_CRC16L_H__
//#include "std_int.h"
//#include "port_cfg.h"
#include "stm32f0xx.h"
#ifdef __cplusplus
extern "C"{
#endif
  
uint16_t crc16l_update_word(uint16_t crc, uint16_t data);
uint16_t crc16l_dis_update_word(uint16_t crc_out,uint16_t data);
uint16_t crc16l_update_byte(uint16_t crc_in,uint8_t data);
uint16_t crc16l_dis_update_byte(uint16_t crc_out,uint8_t data);
uint16_t crc16l_update_nibble(uint16_t crc_in,uint8_t data);
uint16_t crc16l_dis_update_nibble(uint16_t crc_out,uint8_t data);
uint16_t crc16l_update_bit(uint16_t crc_in,uint8_t data);
uint16_t crc16l_dis_update_bit(uint16_t crc_out,uint8_t data);
uint16_t crc16l_calculate(uint16_t crc_in,const uint8_t *data,uint16_t size);
uint16_t crc16l_check(const uint8_t *data,uint16_t size);

#ifdef __cplusplus
}
#endif
  
#endif