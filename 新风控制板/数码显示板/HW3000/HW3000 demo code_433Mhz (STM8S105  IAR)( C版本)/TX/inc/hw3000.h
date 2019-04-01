/******************************************************************************
 * @file     hw3000.h
 * @brief    hw3000 driver
 *           
 *
 * @version  1.0
 * @date     2016-01-14
 *
 * @author   Sundy
 *
 * @note
 * @Copyright (C) 2016 Shanghai Eastsoft Microelectronics C0., Ltd.
 ******************************************************************************/
#ifndef __M928_HW3000_H
#define __M928_HW3000_H

#include <stdint.h>

#define	HW3000_BUF_LEN	64

#define hw3000_ENABLE	1
#define hw3000_DISABLE 0

typedef struct {
	uint8_t  data[HW3000_BUF_LEN];
	uint16_t len;
	uint16_t idx;
} hw3000_data_t;

typedef enum {
	FREQ_BAND_315MHZ = 0,
	FREQ_BAND_433MHZ = 1,
	FREQ_BAND_779MHZ = 2,
	FREQ_BAND_868MHZ = 3,
	FREQ_BAND_915MHZ = 4
} frequency_band_t;

typedef enum {
	DEFAULT = 0,
	DIRECT = 1
} frequency_mode_t;

typedef enum {
	SYMBOL_RATE_1K = 0,
	SYMBOL_RATE_10K = 1,
	SYMBOL_RATE_19K2 = 2,
	SYMBOL_RATE_38K4 = 3,
	SYMBOL_RATE_50K = 4,
	SYMBOL_RATE_100K = 5,
} symbol_rate_t;

typedef enum {
	FIFO = 0,
	FRAME = 1
} frame_mode_t;

typedef enum {
	OSC20MHZ = 0,
	OSC26MHZ = 1
} osc_mode_t;

typedef enum {
	POWER_DOWN = 0,
	DEEP_SLEEP = 1,
	SLEEP = 2,
	IDLE = 3,
	TX = 4,
	RX = 5
} hw3000_state_t;

typedef struct {
	osc_mode_t			osc;			//osc selection
	frequency_band_t 	band;			//freq band setting
	symbol_rate_t 		rate;			//data rate setting
	frequency_mode_t	freq_mode;		//0 default mode only for 433Mhz 1 direct mode for all
	int8_t 				power;			//tx power set
	frame_mode_t		frame_mode;		//0 fifo mode, 1 frame mode
	uint8_t				ack_mode;		//0 disable, 1 enable
	uint8_t				tx_mode;		//0 tx length by LEN0_PKLEN, 1 by fifo pointer
	uint8_t 			rx_mode;		//0 automatic 1 by LEN0_PKLEN
	uint8_t				lp_enable;		//0 low power mode disable, 1 enable
} hw3000_mode_t;

/******************************************************************************
 * @brief    cal_crc_ccitt
 *           
 * @note
 *
 * @param	 ptr
 *			 len
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
uint16_t cal_crc_ccitt(uint8_t *ptr, uint16_t len) ;

/******************************************************************************
 * @brief    hw3000_port_init
 *           
 * @note
 *
 * @param	 None
 *			 
 * @retval	 None
 ******************************************************************************/
void hw3000_port_init(void);

/******************************************************************************
 * @brief    hw3000_init
 *           
 * @note
 *
 * @param	 hw3000_mode
 *			 
 * @retval	 None
 ******************************************************************************/
void hw3000_init(hw3000_mode_t hw3000_mode);


/******************************************************************************
 * @brief    hw3000_freq_set
 *           
 * @note
 *
 * @param	 hw3000_mode
 *			 
 * @retval	 None
 ******************************************************************************/
void hw3000_freq_set(hw3000_mode_t hw3000_mode);

/******************************************************************************
 * @brief    hw3000_rate_set
 *           
 * @note
 *
 * @param	 hw3000_mode
 *			 
 * @retval	 None
 ******************************************************************************/
void hw3000_rate_set(hw3000_mode_t hw3000_mode);

/******************************************************************************
 * @brief    hw3000_power_set
 *           
 * @note
 *
 * @param	 hw3000_mode
 *			 
 * @retval	 None
 ******************************************************************************/
void hw3000_power_set(hw3000_mode_t hw3000_mode);

/******************************************************************************
 * @brief    hw3000_frame_set
 *           
 * @note
 *
 * @param	 hw3000_mode
 ******************************************************************************/
void hw3000_frame_set(hw3000_mode_t hw3000_mode);

/******************************************************************************
 * @brief    hw3000_frame_tx
 *           
 * @note
 *
 * @param	 psdu
 *			 len	
 * @retval	 0 if success else failed
 ******************************************************************************/
int8_t hw3000_frame_tx(hw3000_mode_t hw3000_mode, hw3000_data_t *txbuf);


/******************************************************************************
 * @brief    hw3000_fifo0_tx
 *           
 * @note
 *
 * @param	 hw3000_mode
 *			 txbuf	
 * @retval	 0 if success else failed
 ******************************************************************************/
int8_t hw3000_fifo_tx(hw3000_mode_t hw3000_mode, hw3000_data_t *txbuf);

/******************************************************************************
 * @brief    hw3000_tx_data
 *           
 * @note
 *
 * @param	 hw3000_mode
 *			 txbuf	
 * @retval	 0 if success else failed
 ******************************************************************************/
int8_t hw3000_tx_data(hw3000_mode_t hw3000_mode, hw3000_data_t *txbuf);

/******************************************************************************
 * @brief    hw3000_tx_sw
 *           
 * @note	 tx single carrier
 *
 * @param	 None
 *			 
 * @retval	 None
 ******************************************************************************/
void hw3000_tx_sw(void);

/******************************************************************************
 * @brief    hw3000_cancel_sw
 *           
 * @note	 cancel single carrier
 *
 * @param	 hw3000_mode
 ******************************************************************************/
void hw3000_cancel_sw(hw3000_mode_t hw3000_mode);

/******************************************************************************
 * @brief    hw3000_tx_pn9
 *           
 * @note	 tx pn9
 *
 * @param	 None
 *			 
 * @retval	 None
 ******************************************************************************/
void hw3000_tx_pn9(void);

/******************************************************************************
 * @brief    hw3000_cancel_pn9
 *           
 * @note	 cancel tx pn9
 *
 * @param	 None
 *			 
 * @retval	 None
 ******************************************************************************/
void hw3000_cancel_pn9(void);

/******************************************************************************
 * @brief    hw3000_tx_0101
 *           
 * @note	 tx 0101
 *
 * @param	 None
 *			 
 * @retval	 None
 ******************************************************************************/
void hw3000_tx_0101(void);

/******************************************************************************
 * @brief    hw3000_tx_0101
 *           
 * @note	 cancel tx 0101
 *
 * @param	 None
 *			 
 * @retval	 None
 ******************************************************************************/
void hw3000_cancel_0101(void);

/******************************************************************************
 * @brief    hw3000_rx_enable
 *           
 * @note	 enable receive
 *
 * @param	 None
 *			 
 * @retval	 0 if success else failed
 ******************************************************************************/
int8_t hw3000_rx_enable(void);

/******************************************************************************
 * @brief    hw3000_rx_disable
 *           
 * @note	 disable receive
 *
 * @param	 None
 *			 
 * @retval	 None
 ******************************************************************************/
void hw3000_rx_disable(void);

/******************************************************************************
 * @brief    hw3000_rx_data
 *           
 * @note	 disable receive
 *
 * @param	 hw3000_mode
 *			 rxbuf
 * @retval	 None
 ******************************************************************************/
int8_t hw3000_rx_data(hw3000_mode_t hw3000_mode, hw3000_data_t *rxbuf);

/******************************************************************************
 * @brief    hw3000_rx_task
 *           
 * @note	 receive task
 *
 * @param	 None
 *			 
 * @retval	 None
 ******************************************************************************/
void hw3000_rx_task(void);

/******************************************************************************
 * @brief    hw3000_power_down
 *           
 * @note	 hw3000 power down
 *
 * @param	 None
 *			 
 * @retval	 None
 ******************************************************************************/
void hw3000_power_down(void);

/******************************************************************************
 * @brief    hw3000_power_up
 *           
 * @note	 hw3000 power up
 *
 * @param	 None
 *			 
 * @retval	 None
 ******************************************************************************/
void hw3000_power_up(void);

/******************************************************************************
 * @brief    hw3000_deep_sleep_down
 *           
 * @note	 hw3000 deep sleep down
 *
 * @param	 None
 ******************************************************************************/
void hw3000_deep_sleep_down(void);

/******************************************************************************
 * @brief    hw3000_deep_sleep_up
 *           
 * @note	 hw3000 deep sleep up
 *
 * @param	 None
 *			 
 * @retval	 None
 ******************************************************************************/
void hw3000_deep_sleep_up(void);

/******************************************************************************
 * @brief    hw3000_sleep_down
 *           
 * @note	 hw3000 sleep down
 *
 * @param	 None
 *			 
 * @retval	 None
 ******************************************************************************/
void hw3000_sleep_down(void);

/******************************************************************************
 * @brief    hw3000_sleep_up
 *           
 * @note	 hw3000 power up
 *
 * @param	 None
 *			 
 * @retval	 None
 ******************************************************************************/
void hw3000_sleep_up(void);

/******************************************************************************
 * @brief    hw3000_soft_reset0
 *           
 * @note	 hw3000 soft reset 0, all but reg val retain
 *
 * @param	 None
 *			 
 * @retval	 None
 ******************************************************************************/
void hw3000_soft_reset0(void);

/******************************************************************************
 * @brief    hw3000_soft_reset1
 *           
 * @note	 hw3000 soft reset 1, all reset
 *
 * @param	 None
 *			 
 * @retval	 None
 ******************************************************************************/
void hw3000_soft_reset1(void);

/******************************************************************************
 * @brief    hw3000_ber_init
 *           
 * @note	 
 *
 * @param	 None
 *
 * @retval	 None
 ******************************************************************************/
void hw3000_ber_init(void);

/******************************************************************************
 * @brief    hw3000_ber_deinit
 *           
 * @note	 
 *
 * @param	 None
 *
 * @retval	 None
 ******************************************************************************/
void hw3000_ber_deinit(void);

/******************************************************************************
 * @brief    hw3000_ber_task
 *           
 * @note	 
 *
 * @param	 None
 *
 * @retval	 None
 ******************************************************************************/
void hw3000_ber_task(void);

/******************************************************************************
 * @brief    hw3000_write_reg
 *           
 * @note	 
 *
 * @param	 addr
 *			 data
 * @retval	 None
 ******************************************************************************/
void hw3000_write_reg(uint8_t addr, uint16_t data);

/******************************************************************************
 * @brief    hw3000_read_reg
 *           
 * @note	 
 *
 * @param	 addr
 *			 
 * @retval	 reg value
 ******************************************************************************/
uint16_t hw3000_read_reg(uint8_t addr);

/******************************************************************************
 * @brief    hw3000_write_fifo
 *           
 * @note	 
 *
 * @param	 addr
 *			 data
 *			 length
 * @retval	 None
 ******************************************************************************/
void hw3000_write_fifo(uint8_t addr, uint8_t *data, uint16_t length);

/******************************************************************************
 * @brief    hw3000_read_fifo
 *           
 * @note	 
 *
 * @param	 addr
 *			 data
 *			 length
 * @retval	 None
 ******************************************************************************/
void hw3000_read_fifo(uint8_t addr, uint8_t *data, uint16_t length);

#endif
