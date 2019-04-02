#ifndef si4463_H
#define si4463_H
#include "stm32f0xx_hal.h"
#define true  1
#define false 0
typedef 	unsigned char	U8;
typedef 	unsigned int	U16;
typedef 	unsigned long	U32;
typedef 	char			S8;
typedef 	int				S16;
typedef 	long			S32;

#define buffer_size 20
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
  }pkt_def;
  unsigned char  pkt_data_buff[buffer_size];
}DATA_PACKET_DEF;

typedef  struct {
  unsigned char rf_receive_end_flag;
  unsigned char rf_receive_data_len;
  unsigned char rf_receive_data_buff[buffer_size];  
  unsigned char rf_pkt_rssi;
}SI443X_RF_DATA_DEF;
typedef  struct {
  unsigned char rf_tx_start_flag;
  unsigned char rf_tx_data_len;
  unsigned char rf_tx_data_buff[20];  
  unsigned char rf_tx_pkt_rssi;
}SI443X_RF_txDATA_DEF;

void spi_read(U8 data_length, U8 api_command );
void tx_data(void);
void SI4463_init(void);
void	Si4463_GpioInt(void);
unsigned char spi_byte(unsigned char data);
U8 check_cts(void);
void spi_write(unsigned char tx_length, unsigned char *p);
void spi_write_fifo(void);
void spi_read_info(void);
void sdn_reset(void);
void clr_interrupt(void);
void fifo_reset(void);
void enable_tx_interrupt(void);
void enable_rx_interrupt(void);
void tx_start(void);
void rx_start(void);
void rx_init(void);
void spi_read_fifo(void);
void rf_standby(void);
void rf_init_freq(void);
void SI446X_INT_STATUS(U8 *p );
void tx_rx_sw(void);
uint32_t uart_tx_DMA(void);
void uCenter_FREQ(unsigned char *p);
void uPA_SETTING(unsigned char *p);
void uCH_SETTING(unsigned char *p);
void uID_SETTING(unsigned char *p);
void RF_info(unsigned char *p);
void rf_tx(void);
void RF_parameters_save(void);
void RF_parameters_init(void);
void RF_parameters_read(void);
void RF_Process(void);
void RF_TX_RX_CTRL(void);
void uRF_rate_SETTING(unsigned char *p);
void RF_init(void);
void rev_ID_write(uint32_t tmp);
uint32_t rev_ID_read(void);
#endif

