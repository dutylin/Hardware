#ifndef __UTIL_CRC16L_H__
#define __UTIL_CRC16L_H__

#include "std_int.h"
#include "port_cfg.h"

#ifdef __cplusplus
extern "C"{
#endif
  
uint16 crc16l_update_word(uint16 crc, uint16 data);
uint16 crc16l_dis_update_word(uint16 crc_out,uint16 data);
uint16 crc16l_update_byte(uint16 crc_in,uint8 data);
uint16 crc16l_dis_update_byte(uint16 crc_out,uint8 data);
uint16 crc16l_update_nibble(uint16 crc_in,uint8 data);
uint16 crc16l_dis_update_nibble(uint16 crc_out,uint8 data);
uint16 crc16l_update_bit(uint16 crc_in,uint8 data);
uint16 crc16l_dis_update_bit(uint16 crc_out,uint8 data);
uint16 crc16l_calculate(uint16 crc_in,const uint8 *data,uint16 size);
uint16 crc16l_check(const uint8 *data,uint16 size);

#ifdef __cplusplus
}
#endif
  
#endif
