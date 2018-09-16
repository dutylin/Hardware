#ifndef CRC_H
#define CRC_H

#define MSG_CRC 125
#define X25_INIT_CRC 0xffff


unsigned int CRC_decode(unsigned char *payload_buf,unsigned char CRC_len);
void CRC_cal(unsigned char *payload_buf,unsigned char *crc_pbuffer,unsigned char CRC_len);
void crc_accumulate(unsigned char *data, unsigned int *crcAccum);
void crc_init(unsigned int* crcAccum);
unsigned int calc_crc(const unsigned char *buf, const unsigned int len);
unsigned int crc_calculate(unsigned char *pBuffer, unsigned int length);
#endif
