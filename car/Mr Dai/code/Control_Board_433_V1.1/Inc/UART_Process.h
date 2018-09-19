#ifndef UART_Process_H_
#define UART_Process_H_
typedef  struct{
	unsigned int   len;
	uint32_t dev_sn[3];//设备的序列号  
	unsigned char  uart_pbuff[30];
}UART_INFO_DEF;
typedef  union {                                  /*T&H DATA*/
  struct {
    uint32_t  pkt_id;
    unsigned char   pkt_tmp_integer;
    unsigned char   pkt_tmp_decimal;  
    unsigned char   pkt_hum_integer;
    unsigned char   pkt_hum_decimal;  
    unsigned char   pkt_status;
    unsigned char   pkt_voltage;
    unsigned char   pkt_serial_num;  
    unsigned char   pkt_rssi;
    unsigned short int     pkt_crc;		
  }pkt_def;
  unsigned char  pkt_data_buff[14];
}DATA_PACKET_DEF;

//unsigned char Uart_Param(unsigned char *pbuff, UART_INFO_DEF UART_INFO,unsigned char len);

#endif
