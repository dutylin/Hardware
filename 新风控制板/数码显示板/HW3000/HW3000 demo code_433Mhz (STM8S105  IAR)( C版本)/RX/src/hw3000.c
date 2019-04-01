/******************************************************************************
 * @file     hw3000.c
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
#include "gpio.h"
#include "hw3000.h"
#include "_reg_hw3000.h"
#include "delay.h"

//extern menu _menu[24];
//extern menu *_curmenu;


volatile hw3000_mode_t 	_hw3000_mode;
volatile hw3000_state_t _hw3000_state; 		
volatile uint8_t 		_hw3000_irq_request;	//request for irq
volatile uint8_t 		_hw3000_gpio1_request;	//request for all most full 
volatile uint8_t 		_hw3000_gpio2_request;	//request for all most empty 

volatile uint16_t 		_hw3000_rx_cnt;			//receive frame count

volatile uint8_t 		_hw3000_ber_enable;
volatile uint8_t 		_hw3000_ber_request;
volatile uint8_t 		_hw3000_ber_sync;

volatile uint32_t		_hw3000_ber_num;
volatile uint32_t 		_hw3000_ber_cnt;
volatile uint32_t 		_hw3000_ber_buf;
volatile uint32_t 		_hw3000_ber_err;
volatile uint32_t 		_hw3000_ber_flag;

hw3000_data_t _hw3000_data_buf;

const uint16_t CRC16_CCITT_Table[256]=
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

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
uint16_t cal_crc_ccitt(uint8_t *ptr, uint16_t len) 
{
	uint16_t crc = 0xFFFF;  //initialization value 

	while(len--){
	   crc = (crc << 8) ^ CRC16_CCITT_Table[((crc>>8) ^ *ptr++) & 0xff];
	}
	
	return crc;
}


/******************************************************************************
 * @brief    hw3000_init
 *           
 * @note
 *
 * @param	 hw3000_mode
 *			 
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_init(hw3000_mode_t hw3000_mode)
{
	uint8_t i;
	uint16_t agc_table[16] = {0x1371,0x1351,0x0B51,0x2F71,0x2F51,0x2E51,
							  0x2E31,0x4B31,0x4731,0x4631,0x4531,0x4431,
							  0x6131,0x6031,0x6011,0x6009};
	uint16_t reg_val;

    
    hw3000_power_down();
    m928_delay(1);
    hw3000_power_up();
    

    
	hw3000_write_reg(0x4C, 0x5555);	 //Bank 0 open by default 
  		  
	while(!(hw3000_read_reg(INTFLAG) & 0x4000)); //wait for chip ready
							  
	hw3000_write_reg(0x4C, 0x55AA);	//Bank 1 open		
							  
	for (i = 0; i < 16; i++) {
		hw3000_write_reg(0x1B + i, agc_table[i]);		
	}
	hw3000_write_reg(0x03, 0x0508);
	hw3000_write_reg(0x11, 0xC630);
	
	switch (hw3000_mode.rate) {	
	case SYMBOL_RATE_1K:
		hw3000_write_reg(0x14, 0x1935);	
	
		hw3000_write_reg(0x40, 0x0008);
		hw3000_write_reg(0x41, 0x0010);
		hw3000_write_reg(0x42, 0x82D8);
		hw3000_write_reg(0x43, 0x3D38);
     
		break;
	case SYMBOL_RATE_10K:
		hw3000_write_reg(0x14, 0x1935);
	
		hw3000_write_reg(0x40, 0x0008);
		hw3000_write_reg(0x41, 0x0010);
		hw3000_write_reg(0x42, 0x82D8);
		hw3000_write_reg(0x43, 0x3D38);
		break;
	case SYMBOL_RATE_19K2:
		hw3000_write_reg(0x14, 0x1915);
		break;
	case SYMBOL_RATE_38K4:
		hw3000_write_reg(0x14, 0x1915);
		break;
	case SYMBOL_RATE_50K:
		hw3000_write_reg(0x14, 0x1915);
		break;
	case SYMBOL_RATE_100K:
		hw3000_write_reg(0x14, 0x1915);
		break;
	default:
		break;
	}
	
	switch (hw3000_mode.band) { 
	case FREQ_BAND_315MHZ:
		hw3000_write_reg(0x17, 0xF223); 
		break;
	case FREQ_BAND_433MHZ:
		hw3000_write_reg(0x17, 0xF6C2);
		break;
	case FREQ_BAND_779MHZ:
		hw3000_write_reg(0x17, 0xFB61);
		break;
	case FREQ_BAND_868MHZ:
		hw3000_write_reg(0x17, 0xFB61);
		break;
	case FREQ_BAND_915MHZ:
		hw3000_write_reg(0x17, 0xFB61);
		break;
	default:
		break;
	}
	
	hw3000_write_reg(0x51, 0x001B);
	hw3000_write_reg(0x55, 0x8003);
	hw3000_write_reg(0x56, 0x4155);
	hw3000_write_reg(0x62, 0x70ED);
	
	hw3000_write_reg(0x4C, 0x5555);	 //Bank 0 open
	hw3000_write_reg(INTIC, 0x8000); //clear por int
	
	hw3000_write_reg(MIXFW, 0x2E35);	//must modify this value	
	hw3000_write_reg(MODECTRL, 0x100F);	//must close GPIO clock for noise reason 
	
	/*osc set*/
	if (hw3000_mode.osc == OSC20MHZ) 
    {
		hw3000_write_reg(MODEMCTRL, 0x5201); //20MHz osc select	
	} 
    else 
    {
	 	hw3000_write_reg(MODEMCTRL, 0x1201); //26MHz osc select	
	}
	
	/*frequency set*/
	if (hw3000_mode.freq_mode == DEFAULT && hw3000_mode.band == FREQ_BAND_433MHZ)
    {
		hw3000_write_reg(RFCFG, 0x3312);    
		hw3000_write_reg(FREQCFG0, 0x30EA); //default mode only for 433MHz
        
	} 
    else 
    { 
		hw3000_freq_set(hw3000_mode); //from deep sleep/sleep need reconfigure this reg
	}
	
	/*data rate*/
	hw3000_rate_set(hw3000_mode);
	
	/*tx power set*/
	hw3000_power_set(hw3000_mode);
	
	/*rx power set*/
	if (hw3000_mode.lp_enable == hw3000_ENABLE) { //HOP_TIMER/(HOP_TIMER+LP_TIMER)
		reg_val = hw3000_read_reg(MODEMCTRL);
		reg_val &= 0xF0FF;
		reg_val |= 0x0200;	//LP_TIMER set
		reg_val |= 0x0004;	//LP_ENABLE
		hw3000_write_reg(MODEMCTRL, reg_val); 
		reg_val = hw3000_read_reg(HOPCFG);
		reg_val &= 0xFF0F;
		reg_val |= 0x0060;	//HOP_TIMER set
		hw3000_write_reg(HOPCFG, reg_val); 
	}
	
	/*frame set*/
	hw3000_frame_set(hw3000_mode);
    
    hw3000_write_reg(0x02, 0xc006);		
    
    hw3000_write_reg(0x14, 0x1933);//set SFD First word
    hw3000_write_reg(0x15, 0x8050);//set SFD Second word
	hw3000_write_reg(PKTCFG1,0x01FF); 

	_hw3000_state = IDLE;
}

/******************************************************************************
 * @brief    hw3000_freq_set
 *           
 * @note
 *
 * @param	 hw3000_mode
 *			 
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_freq_set(hw3000_mode_t hw3000_mode)
{
	if (hw3000_mode.osc == OSC20MHZ) {
		switch (hw3000_mode.band) {
		case FREQ_BAND_315MHZ:
			hw3000_write_reg(RFCFG, 0x5312); 
			hw3000_write_reg(FREQCFG1, 0x8010);	//315MHz
			hw3000_write_reg(FREQCFG2, 0x0000); //must first set
			hw3000_write_reg(FREQCFG0, 0x205E); //last set RF_FREQ_BASE
			break;
		case FREQ_BAND_433MHZ:
			hw3000_write_reg(RFCFG, 0x3312);
			hw3000_write_reg(FREQCFG1, 0x8013);	//433MHz
			hw3000_write_reg(FREQCFG2, 0x3333); //must first set
			hw3000_write_reg(FREQCFG0, 0x2056); //last set RF_FREQ_BASE
			break;
		case FREQ_BAND_779MHZ:
			hw3000_write_reg(RFCFG, 0x0312);
			hw3000_write_reg(FREQCFG1, 0x801C);	//779MHz
			hw3000_write_reg(FREQCFG2, 0x999A); //must first set
			hw3000_write_reg(FREQCFG0, 0x204D); //last set RF_FREQ_BASE
			break;
		case FREQ_BAND_868MHZ:
			hw3000_write_reg(RFCFG, 0x1312);
			hw3000_write_reg(FREQCFG1, 0x8019);	//868MHz
			hw3000_write_reg(FREQCFG2, 0x3333); //must first set
			hw3000_write_reg(FREQCFG0, 0x2056); //last set RF_FREQ_BASE
			break;
		case FREQ_BAND_915MHZ:
			hw3000_write_reg(RFCFG, 0x1312);
			hw3000_write_reg(FREQCFG1, 0x8013);	//916MHz
			hw3000_write_reg(FREQCFG2, 0x6666); //must first set
			hw3000_write_reg(FREQCFG0, 0x205B); //last set RF_FREQ_BASE
			break;
		default:
			break;
		}
	} else {
		switch (hw3000_mode.band) {
		case FREQ_BAND_315MHZ:
			hw3000_write_reg(RFCFG, 0x5312); 
			hw3000_write_reg(FREQCFG1, 0x8016);	//315MHz
			hw3000_write_reg(FREQCFG2, 0xC4EC); //must first set
			hw3000_write_reg(FREQCFG0, 0x2048); //last set RF_FREQ_BASE
			break;
		case FREQ_BAND_433MHZ:
			hw3000_write_reg(RFCFG, 0x3312);
			hw3000_write_reg(FREQCFG1, 0x8013);	//433MHz
			hw3000_write_reg(FREQCFG2, 0xB13B); //must first set
			hw3000_write_reg(FREQCFG0, 0x2042); //last set RF_FREQ_BASE
			break;
		case FREQ_BAND_868MHZ:
			hw3000_write_reg(RFCFG, 0x1312);
			hw3000_write_reg(FREQCFG1, 0x8018);	//868MHz
			hw3000_write_reg(FREQCFG2, 0x3B14); //must first set
			hw3000_write_reg(FREQCFG0, 0x2042); //last set RF_FREQ_BASE
			break;
		case FREQ_BAND_915MHZ:
			hw3000_write_reg(RFCFG, 0x1312);
			hw3000_write_reg(FREQCFG1, 0x800C);	//916MHz
			hw3000_write_reg(FREQCFG2, 0x9D8A); //must first set
			hw3000_write_reg(FREQCFG0, 0x2046); //last set RF_FREQ_BASE
			break;
		default:
			break;
		}
	}
}

/******************************************************************************
 * @brief    hw3000_rate_set
 *           
 * @note
 *
 * @param	 hw3000_mode
 *			 
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_rate_set(hw3000_mode_t hw3000_mode)
{
	if (hw3000_mode.osc == OSC20MHZ) {
		switch (hw3000_mode.rate) {
		case SYMBOL_RATE_1K:
			hw3000_write_reg(SYMRATE0, 0x0008); 
			hw3000_write_reg(SYMRATE1, 0x0031);
			hw3000_write_reg(FILTERBAND, 0x0002);
			switch (hw3000_mode.band) {
			case FREQ_BAND_315MHZ:
				hw3000_write_reg(DEVIATION, 0x0007); 
				break;
			case FREQ_BAND_433MHZ:
				hw3000_write_reg(DEVIATION, 0x0007); 
				break;
			case FREQ_BAND_779MHZ:
				hw3000_write_reg(DEVIATION, 0x0002); 
				break;
			case FREQ_BAND_868MHZ:
				hw3000_write_reg(DEVIATION, 0x0002); 
				break;
			case FREQ_BAND_915MHZ:
				hw3000_write_reg(DEVIATION, 0x0002); 
				break;
			default:
				break;
			}
			break;
		case SYMBOL_RATE_10K:
			hw3000_write_reg(SYMRATE0, 0x0051); 
			hw3000_write_reg(SYMRATE1, 0x00EC);
			hw3000_write_reg(FILTERBAND, 0x0025);
			switch (hw3000_mode.band) {
			case FREQ_BAND_315MHZ:
				hw3000_write_reg(DEVIATION, 0x007B); 
				break;
			case FREQ_BAND_433MHZ:
				hw3000_write_reg(DEVIATION, 0x0052); 
				break;
			case FREQ_BAND_779MHZ:
				hw3000_write_reg(DEVIATION, 0x0029); 
				break;
			case FREQ_BAND_868MHZ:
				hw3000_write_reg(DEVIATION, 0x0029); 	
				break;
			case FREQ_BAND_915MHZ:
				hw3000_write_reg(DEVIATION, 0x0029); 
				break;
			default:
				break;
			}
			break;
		case SYMBOL_RATE_19K2:
			hw3000_write_reg(SYMRATE0, 0x009D); 
			hw3000_write_reg(SYMRATE1, 0x0049);
			hw3000_write_reg(FILTERBAND, 0x002A);	
			switch (hw3000_mode.band) {
			case FREQ_BAND_315MHZ:
				hw3000_write_reg(DEVIATION, 0x007B); 
				break;
			case FREQ_BAND_433MHZ:
				hw3000_write_reg(DEVIATION, 0x0052); 
				break;
			case FREQ_BAND_779MHZ:
				hw3000_write_reg(DEVIATION, 0x0029); 
				break;
			case FREQ_BAND_868MHZ:
				hw3000_write_reg(DEVIATION, 0x0029); 	
				break;
			case FREQ_BAND_915MHZ:
				hw3000_write_reg(DEVIATION, 0x0029); 
				break;
			default:
				break;
			}		
			break;
		case SYMBOL_RATE_38K4:
			hw3000_write_reg(SYMRATE0, 0x013A); 
			hw3000_write_reg(SYMRATE1, 0x0093);
			hw3000_write_reg(FILTERBAND, 0x0033);	
			switch (hw3000_mode.band) {
			case FREQ_BAND_315MHZ:
				hw3000_write_reg(DEVIATION, 0x007B); 
				break;
			case FREQ_BAND_433MHZ:
				hw3000_write_reg(DEVIATION, 0x0052); 
				break;
			case FREQ_BAND_779MHZ:
				hw3000_write_reg(DEVIATION, 0x0029); 
				break;
			case FREQ_BAND_868MHZ:
				hw3000_write_reg(DEVIATION, 0x0029); 	
				break;
			case FREQ_BAND_915MHZ:
				hw3000_write_reg(DEVIATION, 0x0029); 
				break;
			default:
				break;
			}				
			break;
		case SYMBOL_RATE_50K:
			hw3000_write_reg(SYMRATE0, 0x0199); 
			hw3000_write_reg(SYMRATE1, 0x009A);
			hw3000_write_reg(FILTERBAND, 0x0037);
			switch (hw3000_mode.band) {
			case FREQ_BAND_315MHZ:
				hw3000_write_reg(DEVIATION, 0x007B); 
				break;
			case FREQ_BAND_433MHZ:
				hw3000_write_reg(DEVIATION, 0x0052); 
				break;
			case FREQ_BAND_779MHZ:
				hw3000_write_reg(DEVIATION, 0x0029); 
				break;
			case FREQ_BAND_868MHZ:
				hw3000_write_reg(DEVIATION, 0x0029); 	
				break;
			case FREQ_BAND_915MHZ:
				hw3000_write_reg(DEVIATION, 0x0029); 
				break;
			default:
				break;
			}				
			break;
		case SYMBOL_RATE_100K:
			hw3000_write_reg(SYMRATE0, 0x0333); 
			hw3000_write_reg(SYMRATE1, 0x0033);
			hw3000_write_reg(FILTERBAND, 0x0078);	
			switch (hw3000_mode.band) {
			case FREQ_BAND_315MHZ:
				hw3000_write_reg(DEVIATION, 0x00F6); 	
				break;
			case FREQ_BAND_433MHZ:
				hw3000_write_reg(DEVIATION, 0x00A4); 	
				break;
			case FREQ_BAND_779MHZ:
				hw3000_write_reg(DEVIATION, 0x0052); 
				break;
			case FREQ_BAND_868MHZ:
				hw3000_write_reg(DEVIATION, 0x0052); 
				break;
			case FREQ_BAND_915MHZ:
				hw3000_write_reg(DEVIATION, 0x0052); 
				break;
			default:
				break;
			}				
			break;
		default:
			break;
		}
	} else {
		switch (hw3000_mode.rate) {
		case SYMBOL_RATE_1K:
			hw3000_write_reg(SYMRATE0, 0x0008); 
			hw3000_write_reg(SYMRATE1, 0x0031);
			hw3000_write_reg(FILTERBAND, 0x0002);
			switch (hw3000_mode.band) {
			case FREQ_BAND_315MHZ:
				hw3000_write_reg(DEVIATION, 0x0006); 
				break;
			case FREQ_BAND_433MHZ:
				hw3000_write_reg(DEVIATION, 0x0004); 
				break;
			case FREQ_BAND_868MHZ:
				hw3000_write_reg(DEVIATION, 0x0002); 
				break;
			case FREQ_BAND_915MHZ:
				hw3000_write_reg(DEVIATION, 0x0002); 
				break;
			default:
				break;
			}
			break;
		case SYMBOL_RATE_10K:
			hw3000_write_reg(SYMRATE0, 0x0051); 
			hw3000_write_reg(SYMRATE1, 0x00EC);
			hw3000_write_reg(FILTERBAND, 0x0025);
			switch (hw3000_mode.band) {
			case FREQ_BAND_315MHZ:
				hw3000_write_reg(DEVIATION, 0x005F); 
				break;
			case FREQ_BAND_433MHZ:
				hw3000_write_reg(DEVIATION, 0x003F); 
				break;
			case FREQ_BAND_868MHZ:
				hw3000_write_reg(DEVIATION, 0x0020); 	
				break;
			case FREQ_BAND_915MHZ:
				hw3000_write_reg(DEVIATION, 0x0020); 
				break;
			default:
				break;
			}
			break;
		case SYMBOL_RATE_19K2:
			hw3000_write_reg(SYMRATE0, 0x009D); 
			hw3000_write_reg(SYMRATE1, 0x0049);
			hw3000_write_reg(FILTERBAND, 0x002A);	
			switch (hw3000_mode.band) {
			case FREQ_BAND_315MHZ:
				hw3000_write_reg(DEVIATION, 0x005F); 
				break;
			case FREQ_BAND_433MHZ:
				hw3000_write_reg(DEVIATION, 0x003F); 
				break;
			case FREQ_BAND_868MHZ:
				hw3000_write_reg(DEVIATION, 0x0020); 	
				break;
			case FREQ_BAND_915MHZ:
				hw3000_write_reg(DEVIATION, 0x0020); 
				break;
			default:
				break;
			}		
			break;
		case SYMBOL_RATE_38K4:
			hw3000_write_reg(SYMRATE0, 0x013A); 
			hw3000_write_reg(SYMRATE1, 0x0093);
			hw3000_write_reg(FILTERBAND, 0x0033);	
			switch (hw3000_mode.band) {
			case FREQ_BAND_315MHZ:
				hw3000_write_reg(DEVIATION, 0x005F); 
				break;
			case FREQ_BAND_433MHZ:
				hw3000_write_reg(DEVIATION, 0x003F); 
				break;
			case FREQ_BAND_868MHZ:
				hw3000_write_reg(DEVIATION, 0x0020); 	
				break;
			case FREQ_BAND_915MHZ:
				hw3000_write_reg(DEVIATION, 0x0020); 
				break;
			default:
				break;
			}				
			break;
		case SYMBOL_RATE_50K:
			hw3000_write_reg(SYMRATE0, 0x0199); 
			hw3000_write_reg(SYMRATE1, 0x009A);
			hw3000_write_reg(FILTERBAND, 0x0037);
			switch (hw3000_mode.band) {
			case FREQ_BAND_315MHZ:
				hw3000_write_reg(DEVIATION, 0x005F); 
				break;
			case FREQ_BAND_433MHZ:
				hw3000_write_reg(DEVIATION, 0x003F); 
				break;
			case FREQ_BAND_868MHZ:
				hw3000_write_reg(DEVIATION, 0x0020); 	
				break;
			case FREQ_BAND_915MHZ:
				hw3000_write_reg(DEVIATION, 0x0020); 
				break;
			default:
				break;
			}				
			break;
		case SYMBOL_RATE_100K:
			hw3000_write_reg(SYMRATE0, 0x0333); 
			hw3000_write_reg(SYMRATE1, 0x0033);
			hw3000_write_reg(FILTERBAND, 0x0078);	
			switch (hw3000_mode.band) {
			case FREQ_BAND_315MHZ:
				hw3000_write_reg(DEVIATION, 0x00BD); 	
				break;
			case FREQ_BAND_433MHZ:
				hw3000_write_reg(DEVIATION, 0x007E); 	
				break;
			case FREQ_BAND_868MHZ:
				hw3000_write_reg(DEVIATION, 0x003F); 
				break;
			case FREQ_BAND_915MHZ:
				hw3000_write_reg(DEVIATION, 0x003F); 
				break;
			default:
				break;
			}				
			break;
		default:
			break;
		}
	}
}	

/******************************************************************************
 * @brief    hw3000_power_set
 *           
 * @note
 *
 * @param	 hw3000_mode
 *			 
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_power_set(hw3000_mode_t hw3000_mode)
{
	switch (hw3000_mode.power) {
		case -40:
			hw3000_write_reg(PACFG, 0x003F); 
			break;
		case -16:
			hw3000_write_reg(PACFG, 0x013F); 
			break;
		case -10:
			hw3000_write_reg(PACFG, 0x023F); 
			break;
		case -5:
			hw3000_write_reg(PACFG, 0x043F); 
			break;
		case 0:
			hw3000_write_reg(PACFG, 0x073F); 
			break;
		case 5:
			hw3000_write_reg(PACFG, 0x0C3F); 
			break;
		case 10:
			hw3000_write_reg(PACFG, 0x183F); 
			break;
		case 15:
			hw3000_write_reg(PACFG, 0x2A3F); 
			break;
		case 18:
			hw3000_write_reg(PACFG, 0x643F); 
			break;
		case 20:
			hw3000_write_reg(PACFG, 0xFF3F); 
			break;
		default:
			break;
	}	
}

/******************************************************************************
 * @brief    hw3000_frame_set
 *           
 * @note
 *
 * @param	 hw3000_mode
 *			 
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_frame_set(hw3000_mode_t hw3000_mode)
{   
	uint16_t reg_val;
	
	if (hw3000_mode.frame_mode == FRAME) { //frame mode
		hw3000_write_reg(PKTCTRL, 0xC000);
		hw3000_write_reg(PKTCFG0, 0x8010); //SFD 4bytes, preamble 8 bytes
		hw3000_write_reg(PREACFG, 0x0206);
		if (hw3000_mode.ack_mode == hw3000_ENABLE) {
			hw3000_write_reg(ACKCFG, 0x30FF);	//re tx times and re ack time
			hw3000_write_reg(PIPECTRL, 0x0011); //PIPE 0 enable 
		} else {
			hw3000_write_reg(PIPECTRL, 0x0001); //PIPE 0 enable
		}
	} else { //direct fifo mode		
		if (hw3000_mode.tx_mode == 0) {
			reg_val = 0x0000;
		} else {
			reg_val = 0x4000;
		}
		if (hw3000_mode.rx_mode == 1) {
			reg_val |=  0x0100;
		} 
		hw3000_write_reg(PKTCTRL, reg_val);
		hw3000_write_reg(GPIOCFG0, 0xC009);	//GPIO 1 for RX FIFO almost full 
		hw3000_write_reg(GPIOCFG1, 0x08C0);	//GPIO 2 for TX FIFO almost empty
		hw3000_write_reg(FIFOTHRES, 0x8080); //FIFO threshold value, 128 for tx 128 for rx
		hw3000_write_reg(PKTCFG0, 0x4008); //SFD 4bytes, preamble 8 bytes
		hw3000_write_reg(PREACFG, 0x0206);
		hw3000_write_reg(FIFOSTA, 0x0008); //first two bytes is length
		hw3000_write_reg(LEN0RXADD, 0x0000); //no add bytes
		hw3000_write_reg(PIPECTRL, 0x0001); //PIPE 0 enable
	} 	
}

/******************************************************************************
 * @brief    hw3000_frame_tx
 *           
 * @note
 *
 * @param	 psdu
 *			 len	
 * @retval	 0 if success else failed
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
int8_t hw3000_frame_tx(hw3000_mode_t hw3000_mode, hw3000_data_t *txbuf)
{
	uint16_t reg_val;
	
	if (txbuf->len > 252)
    {
		return -1;
	}
	
	if (_hw3000_state != TX)
    {
		_hw3000_state = TX;
		
		hw3000_write_reg(TRCTRL, 0x0100); 	//tx_enable
		hw3000_write_reg(FIFOSTA, 0x0100); //flush fifo
		hw3000_write_fifo(FIFODATA, txbuf->data, txbuf->len); //write fifo
		hw3000_write_reg(FIFOCTRL, 0x0001); //ocpy = 1
		
		_hw3000_irq_request = 0;
		while (!_hw3000_irq_request); //wait for send finish
		_hw3000_irq_request = 0;
		_hw3000_state = IDLE;
		
		if (hw3000_mode.ack_mode == hw3000_ENABLE)
        {
			reg_val = hw3000_read_reg(INTFLAG);
			if (reg_val & 0x0002) 
            {
				hw3000_write_reg(FIFOCTRL, 0x0000); //ocpy = 0
				hw3000_write_reg(INTIC, 0x0001); 	//clr_int
				hw3000_write_reg(TRCTRL, 0x0000); 	//send disable
			
				return -1;
			}
		}
		hw3000_write_reg(FIFOCTRL, 0x0000); //ocpy = 0
		hw3000_write_reg(INTIC, 0x0001); 	//clr_int
		hw3000_write_reg(TRCTRL, 0x0000); 	//send disable
		
		return 0;
	}
	return -1;
}

/******************************************************************************
 * @brief    hw3000_fifo0_tx
 *           
 * @note
 *
 * @param	 hw3000_mode
 *			 txbuf	
 * @retval	 0 if success else failed
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
int8_t hw3000_fifo_tx(hw3000_mode_t hw3000_mode, hw3000_data_t *txbuf)
{	
	if (_hw3000_state != TX) {
		_hw3000_state = TX;
		
		hw3000_write_reg(TRCTRL, 0x0100); 	//tx_enable
		hw3000_write_reg(FIFOSTA, 0x0108);  //flush fifo
		
		if (hw3000_mode.tx_mode == 0) {
			hw3000_write_reg(LEN0PKLEN, txbuf->len);
		}
		
		if (txbuf->len > 256) {
			hw3000_write_fifo(FIFODATA, txbuf->data, 256); //write fifo
			txbuf->len -= 256;
		} else {
			hw3000_write_fifo(FIFODATA, txbuf->data, txbuf->len); //write fifo
			txbuf->len = 0;
		}
		txbuf->idx = 0;
		
		hw3000_write_reg(FIFOCTRL, 0x0001); //ocpy = 1
		
		_hw3000_irq_request = 0;
		_hw3000_gpio2_request = 0;
		while (1) {
			if (_hw3000_gpio2_request == 1 && txbuf->len != 0) {
				_hw3000_gpio2_request = 0;
				
				if (txbuf->len > 128) { 
					hw3000_write_fifo(FIFODATA, &txbuf->data[256+128*txbuf->idx++], 128); //write fifo
					txbuf->len -= 128;
				} else {
					hw3000_write_fifo(FIFODATA, &txbuf->data[256+128*txbuf->idx], txbuf->len); //write fifo
					txbuf->len = 0;
				}
			}
			
			if (_hw3000_irq_request == 1) {	//wait for send finish
				_hw3000_irq_request = 0;
				_hw3000_state = IDLE;
				
				hw3000_write_reg(FIFOCTRL, 0x0000); //ocpy = 0
				hw3000_write_reg(INTIC, 0x0001); //clr_int
				hw3000_write_reg(TRCTRL, 0x0000); //send disable
				
				return 0;
			}
		}
	}
	
	return -1;
}

/******************************************************************************
 * @brief    hw3000_tx_data
 *           
 * @note
 *
 * @param	 hw3000_mode
 *			 txbuf	
 * @retval	 0 if success else failed
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
int8_t hw3000_tx_data(hw3000_mode_t hw3000_mode, hw3000_data_t *txbuf)
{	
	if (hw3000_mode.frame_mode == FRAME) 
    {
		return hw3000_frame_tx(hw3000_mode, txbuf);
	} 
    else 
    {
		return hw3000_fifo_tx(hw3000_mode, txbuf);
	}
}

/******************************************************************************
 * @brief    hw3000_tx_sw
 *           
 * @note	 tx single carrier
 *
 * @param	 None
 *			 
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_tx_sw(void)
{
	hw3000_write_reg(PKTCTRL, 0xE000); //enable single carrior mode
	hw3000_write_reg(DEVIATION, 0x0000); //divation 0 Hz
	hw3000_write_reg(TRCTRL, 0x0100); //tx enable
	hw3000_write_reg(FIFODATA, 0x5555);
	hw3000_write_reg(FIFOCTRL, 0x0001); //ocpy = 1
}

/******************************************************************************
 * @brief    hw3000_cancel_sw
 *           
 * @note	 cancel single carrier
 *
 * @param	 hw3000_mode
 *			 
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_cancel_sw(hw3000_mode_t hw3000_mode)
{	
	uint16_t reg_val;
	
	hw3000_write_reg(FIFOCTRL, 0x0000); //ocpy = 0
	hw3000_write_reg(TRCTRL, 0x0000); //tx disable
	
	if (hw3000_mode.frame_mode == FRAME) {	 //增强型帧结构 
		hw3000_write_reg(PKTCTRL, 0xC000);
	} else {
		if (hw3000_mode.tx_mode == 0) {
			reg_val = 0x0000;
		} else {
			reg_val = 0x4000;
		}
		if (hw3000_mode.rx_mode == 1) {
			reg_val |=  0x0100;
		} 
	
		hw3000_write_reg(PKTCTRL, reg_val);
	}
	
	hw3000_rate_set(hw3000_mode);
}

/******************************************************************************
 * @brief    hw3000_tx_pn9
 *           
 * @note	 tx pn9
 *
 * @param	 None
 *			 
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_tx_pn9(void)
{
	hw3000_write_reg(MODECTRL, 0x150F); //DERECT_MOD REP_MOD 
	hw3000_write_reg(TRCTRL, 0x0100); //tx_enable
	m928_delay(1);
	hw3000_write_reg(MODECTRL, 0x170F);	
}

/******************************************************************************
 * @brief    hw3000_cancel_pn9
 *           
 * @note	 cancel tx pn9
 *
 * @param	 None
 *			 
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_cancel_pn9(void)
{
	hw3000_write_reg(TRCTRL, 0x0000); //tx_disable
	hw3000_write_reg(MODECTRL, 0x100F);	
}

/******************************************************************************
 * @brief    hw3000_tx_0101
 *           
 * @note	 tx 0101
 *
 * @param	 None
 *			 
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_tx_0101(void)
{
	hw3000_write_reg(MODECTRL, 0x190F); //DERECT_MOD REP_MOD 
	hw3000_write_reg(TRCTRL, 0x0100); //tx_enable
	m928_delay(1);
	hw3000_write_reg(MODECTRL, 0x1B0F);	
}

/******************************************************************************
 * @brief    hw3000_tx_0101
 *           
 * @note	 cancel tx 0101
 *
 * @param	 None
 *			 
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_cancel_0101(void)
{
	hw3000_write_reg(TRCTRL, 0x0000); //tx_disable
	hw3000_write_reg(MODECTRL, 0x100F);	
}

/******************************************************************************
 * @brief    hw3000_rx_enable
 *           
 * @note	 enable receive
 *
 * @param	 None
 *			 
 * @retval	 0 if success else failed
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
int8_t hw3000_rx_enable(void)
{
	if (_hw3000_state != TX) {
		hw3000_write_reg(TRCTRL, 0x0080); //enable rx
        
	    hw3000_write_reg(FIFOSTA, 0x0200); //flush fifo
           
		_hw3000_state = RX;
		_hw3000_irq_request = 0;
		_hw3000_gpio1_request = 0;
		
		return 0;
	}
	return -1;
}

/******************************************************************************
 * @brief    hw3000_rx_disable
 *           
 * @note	 disable receive
 *
 * @param	 None
 *			 
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_rx_disable(void)
{
	hw3000_write_reg(TRCTRL, 0x0000); //rx_disable
	hw3000_write_reg(INTIC, 0x0001); //clr_int
	
	_hw3000_state = IDLE;
	_hw3000_irq_request = 0;
	_hw3000_gpio1_request = 0;
}

/******************************************************************************
 * @brief    hw3000_rx_data
 *           
 * @note	 disable receive
 *
 * @param	 hw3000_mode
 *			 rxbuf
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
int8_t hw3000_rx_data(hw3000_mode_t hw3000_mode, hw3000_data_t *rxbuf)
{
   uint16_t reg;
	
	if (_hw3000_state == RX) 
    {
		if (hw3000_mode.frame_mode == FRAME) 
        {
            
			reg = hw3000_read_reg(FIFOCTRL);
			if (!(reg & 0xC000)) //PHR CRC check
            {   
				rxbuf->len = hw3000_read_reg(RXPHR0);
				rxbuf->len = ((rxbuf->len >> 8) - 3) & 0x00FF;
                if(rxbuf->len)
                {
                hw3000_read_fifo(FIFODATA, rxbuf->data, rxbuf->len);
				hw3000_rx_disable();
				hw3000_rx_enable();
                return 0;
                }
                else
                {
                  hw3000_rx_disable();
		          hw3000_rx_enable();
                  return -1;
                }
			}
            else
            {
                hw3000_rx_disable();
		        hw3000_rx_enable();
                return -1;
            }
		} 
        else 
        {
			hw3000_read_fifo(FIFODATA, rxbuf->data, 2);
			rxbuf->len = rxbuf->data[0] * 256 + rxbuf->data[1];
			if (rxbuf->len > 2048) 
            {
				rxbuf->len = 2048;
			}
			rxbuf->idx = 0;
			if (hw3000_mode.rx_mode == 1) 
            {
				hw3000_write_reg(LEN0PKLEN, rxbuf->len);
			}
			rxbuf->len -= 2;
			
			while (1) 
            {
				if (_hw3000_gpio1_request == 1) 
                {
					_hw3000_gpio1_request = 0;
					
					hw3000_read_fifo(FIFODATA, &rxbuf->data[2 + 128 * rxbuf->idx++], 128);
					rxbuf->len -= 128;
				}
				
				if (_hw3000_irq_request == 1)
                {
					_hw3000_irq_request = 0;
					
					hw3000_read_fifo(FIFODATA, &rxbuf->data[2 + 128 * rxbuf->idx], rxbuf->len);
					rxbuf->len = 0;
					
					break;
				}
			}
		} 
		
		hw3000_rx_disable();
		hw3000_rx_enable();
		return 0;
	} 
    else
    {
		return -1;
	}
}

/******************************************************************************
 * @brief    hw3000_rx_task
 *           
 * @note	 receive task
 *
 * @param	 None
 *			 
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_rx_task(void)
{
	uint16_t  crc, len, reg;

		
	if (_hw3000_state != RX)
    {
		_hw3000_gpio1_request = 0;
		return;
	}
    
	if (hw3000_rx_data(_hw3000_mode, &_hw3000_data_buf) == 0)
    {
		len = _hw3000_data_buf.data[0] * 256 + _hw3000_data_buf.data[1] - 2;
		crc = cal_crc_ccitt( _hw3000_data_buf.data, len);
		reg =  _hw3000_data_buf.data[len] * 256 + _hw3000_data_buf.data[len+1];
	    
		if (crc == reg)
        {
          GPIO_WriteLow(RF_LED_PORT,RF_LED_IO);
          m928_delay(20);
          GPIO_WriteHigh(RF_LED_PORT,RF_LED_IO);
        }
        else
        {
          GPIO_WriteLow(RF_LED_PORT1,RF_LED_IO1);
        }
	}
}

/******************************************************************************
 * @brief    hw3000_power_down
 *           
 * @note	 hw3000 power down
 *
 * @param	 None
 *			 
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_power_down(void)
{
	//GPIO->PA_DATA |= 0x00002000; //PDN 1
  GPIO_WriteHigh(HW3000_PDN_PORT,HW3000_PDN_IO);
}

/******************************************************************************
 * @brief    hw3000_power_up
 *           
 * @note	 hw3000 power up
 *
 * @param	 None
 *			 
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_power_up(void)
{
	//GPIO->PA_DATA &= 0xFFFFDFFF; //PDN 0
    GPIO_WriteLow(HW3000_PDN_PORT,HW3000_PDN_IO);
}

/******************************************************************************
 * @brief    hw3000_deep_sleep_down
 *           
 * @note	 hw3000 deep sleep down
 *
 * @param	 None
 *			 
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_deep_sleep_down(void)
{
	uint16_t reg;
	
	hw3000_write_reg(WDTCFG1, 0x0001);
	reg = hw3000_read_reg(MODECTRL);
	hw3000_write_reg(MODECTRL, 0x8000 | reg);
}

/******************************************************************************
 * @brief    hw3000_deep_sleep_up
 *           
 * @note	 hw3000 deep sleep up
 *
 * @param	 None
 *			 
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_deep_sleep_up(void)
{
	uint16_t reg;
	
	hw3000_write_reg(WDTCFG1, 0x0081);
	reg = hw3000_read_reg(MODECTRL);
	hw3000_write_reg(MODECTRL, 0x7FFF & reg);
}

/******************************************************************************
 * @brief    hw3000_sleep_down
 *           
 * @note	 hw3000 sleep down
 *
 * @param	 None
 *			 
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_sleep_down(void)
{
	uint16_t reg;
	
	hw3000_write_reg(WDTCFG1, 0x0081);
	reg = hw3000_read_reg(MODECTRL);
	hw3000_write_reg(MODECTRL, 0x8000 | reg);
	
}

/******************************************************************************
 * @brief    hw3000_sleep_up
 *           
 * @note	 hw3000 power up
 *
 * @param	 None
 *			 
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_sleep_up(void)
{
	uint16_t reg;
	
	hw3000_write_reg(WDTCFG1, 0x0081);
	reg = hw3000_read_reg(MODECTRL);
	hw3000_write_reg(MODECTRL, 0x7FFF & reg);
}

/******************************************************************************
 * @brief    hw3000_soft_reset0
 *           
 * @note	 hw3000 soft reset 0, all but reg val retain
 *
 * @param	 None
 *			 
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_soft_reset0(void)
{
	hw3000_write_reg(SFTRST0, 0x55AA);
}

/******************************************************************************
 * @brief    hw3000_soft_reset1
 *           
 * @note	 hw3000 soft reset 1, all reset
 *
 * @param	 None
 *			 
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_soft_reset1(void)
{
	hw3000_write_reg(SFTRST1, 0x55AA);
}

/******************************************************************************
 * @brief    hw3000_ber_init
 *           
 * @note	 
 *
 * @param	 None
 *
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_ber_init(void) {
	
	_hw3000_ber_enable = 1;
	_hw3000_ber_request = 0;
	_hw3000_ber_sync = 0;
	_hw3000_ber_flag = 0;
	_hw3000_ber_num = 0;
	_hw3000_ber_cnt = 0;
	_hw3000_ber_err = 0;
	_hw3000_ber_buf = 0x00000000;
	
	hw3000_write_reg(GPIOCFG1, 0x05DF); //GPIO2 rx data
	hw3000_write_reg(INTIE, 0x0002); //direct_ie = 1
	
	hw3000_write_reg(MODECTRL, 0x110F); //enable direct mode
	hw3000_write_reg(TRCTRL, 0x0080); //Rx_enable
	m928_delay(1);
	hw3000_write_reg(MODECTRL, 0x130F);	
}

/******************************************************************************
 * @brief    hw3000_ber_deinit
 *           
 * @note	 
 *
 * @param	 None
 *
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_ber_deinit(void) {
	_hw3000_ber_enable = 0;
	_hw3000_ber_request = 0;
	_hw3000_ber_sync = 0;
	_hw3000_ber_flag = 0;
	_hw3000_ber_cnt = 0;
	_hw3000_ber_num = 0;
	_hw3000_ber_err = 0;
	_hw3000_ber_buf = 0x00000000;
	
	hw3000_write_reg(GPIOCFG1, 0xCFDF);
	hw3000_write_reg(INTIE, 0x8001); 
	hw3000_write_reg(MODECTRL, 0x100F);
	hw3000_write_reg(TRCTRL, 0x0000); //rx_disable
}

/******************************************************************************
 * @brief    hw3000_ber_task
 *           
 * @note	 
 *
 * @param	 None
 *
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_ber_task(void)
{
	uint32_t j;
	uint8_t banner[14] = "0.00000      ";
	uint8_t time[14] =   "0000000      ";
	
	_hw3000_ber_request = 0;
	
	banner[0] = (_hw3000_ber_err / 10000) + 48;
	_hw3000_ber_err %= 10000;
	banner[2] = (_hw3000_ber_err / 1000) + 48;
	_hw3000_ber_err %= 1000;
	banner[3] = (_hw3000_ber_err / 100) + 48;
	_hw3000_ber_err %= 100;
	banner[4] = (_hw3000_ber_err / 10) + 48;
	_hw3000_ber_err %= 10;
	banner[5] = _hw3000_ber_err + 48;
	
	_hw3000_ber_num++;
	j = _hw3000_ber_num;
	
	time[0] = (j / 1000000) + 48;
	j %= 1000000;
	time[1] = (j / 100000) + 48;
	j %= 100000;
	time[2] = (j / 10000) + 48;
	j %= 10000;
	time[3] = (j / 1000) + 48;
	j %= 1000;
	time[4] = (j / 100) + 48;
	j %= 100;
	time[5] = (j / 10) + 48;
	j %= 10;
	time[6] = j + 48;
//	oled_print(2, banner, 0);
//	oled_print(3, time, 0);
}

/******************************************************************************
 * @brief    hw3000_write_reg
 *           
 * @note	 
 *
 * @param	 addr
 *			 data
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_write_reg(uint8_t addr, uint16_t data)
{
 uint8_t i=0;
 
 addr=addr|0x80;
 GPIO_WriteLow(HW3000_CSN_PORT,HW3000_CSN_IO);
 for(i=0;i<8;i++)
 {
  GPIO_WriteHigh(HW3000_CLK_PORT,HW3000_CLK_IO);
  if(addr&0x80)
  {
   GPIO_WriteHigh(HW3000_SDO_PORT,HW3000_SDO_IO);
  }
  else
  {
   GPIO_WriteLow(HW3000_SDO_PORT,HW3000_SDO_IO);
  }
  GPIO_WriteLow(HW3000_CLK_PORT,HW3000_CLK_IO);
  addr<<=1;
 }
 
 for(i=0;i<16;i++)
 {
  GPIO_WriteHigh(HW3000_CLK_PORT,HW3000_CLK_IO);
  if(data&0x8000)
  {
   GPIO_WriteHigh(HW3000_SDO_PORT,HW3000_SDO_IO);
  }
  else
  {
   GPIO_WriteLow(HW3000_SDO_PORT,HW3000_SDO_IO);
  }
  GPIO_WriteLow(HW3000_CLK_PORT,HW3000_CLK_IO);
  data<<=1;
 }
 
 GPIO_WriteHigh(HW3000_CSN_PORT,HW3000_CSN_IO);
}

/******************************************************************************
 * @brief    hw3000_read_reg
 *           
 * @note	 
 *
 * @param	 addr
 *			 
 * @retval	 reg value
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
uint16_t hw3000_read_reg(uint8_t addr)
{
 uint8_t i=0;
 uint16_t  temp=0;
 
 addr=addr&0x7F;
 GPIO_WriteLow(HW3000_CSN_PORT,HW3000_CSN_IO);
 for(i=0;i<8;i++)
 {
  GPIO_WriteHigh(HW3000_CLK_PORT,HW3000_CLK_IO);
  if(addr&0x80)
  {
   GPIO_WriteHigh(HW3000_SDO_PORT,HW3000_SDO_IO);
  }
  else
  {
   GPIO_WriteLow(HW3000_SDO_PORT,HW3000_SDO_IO);
  }
  GPIO_WriteLow(HW3000_CLK_PORT,HW3000_CLK_IO);
  addr<<=1;
 }
 
 for(i=0;i<16;i++)
 {
  temp<<=1;
  GPIO_WriteHigh(HW3000_CLK_PORT,HW3000_CLK_IO);
  GPIO_WriteLow(HW3000_CLK_PORT,HW3000_CLK_IO);
  if(GPIO_ReadInputPin(HW3000_SDI_PORT,HW3000_SDI_IO)==SET)
  {
   temp|=0x0001;
  }
  else
  {
   temp&=0xfffe;
  }

 }
 GPIO_WriteHigh(HW3000_CSN_PORT,HW3000_CSN_IO);
 
 return temp;
}

/******************************************************************************
 * @brief    hw3000_write_fifo
 *           
 * @note	 
 *
 * @param	 addr
 *			 data
 *			 length
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_write_fifo(uint8_t addr, uint8_t *data, uint16_t length)
{
 uint8_t i=0;
 uint8_t j=0;
 uint8_t temp=0;
 
 addr|=0x80;
 GPIO_WriteLow(HW3000_CSN_PORT,HW3000_CSN_IO);
 
 for(i=0;i<8;i++)
 {
  GPIO_WriteHigh(HW3000_CLK_PORT,HW3000_CLK_IO);
  if(addr&0x80)
  {
   GPIO_WriteHigh(HW3000_SDO_PORT,HW3000_SDO_IO);
  }
  else
  {
   GPIO_WriteLow(HW3000_SDO_PORT,HW3000_SDO_IO);
  }
  GPIO_WriteLow(HW3000_CLK_PORT,HW3000_CLK_IO);
  addr<<=1;
 }
 
 for(j=0;j<length;j++)
 {
  temp=data[j];
  for(i=0;i<8;i++)
  {
   GPIO_WriteHigh(HW3000_CLK_PORT,HW3000_CLK_IO);
   if(temp&0x80)
   {
    GPIO_WriteHigh(HW3000_SDO_PORT,HW3000_SDO_IO);
   }
   else
   {
    GPIO_WriteLow(HW3000_SDO_PORT,HW3000_SDO_IO);
   }
   GPIO_WriteLow(HW3000_CLK_PORT,HW3000_CLK_IO);
   temp<<=1;
  }
 }
 
 GPIO_WriteHigh(HW3000_CSN_PORT,HW3000_CSN_IO);
}

/******************************************************************************
 * @brief    hw3000_read_fifo
 *           
 * @note	 
 *
 * @param	 addr
 *			 data
 *			 length
 * @retval	 None
 *
 * @version  1.0
 * @date     2016-01-14
 * @author   sundy
 ******************************************************************************/
void hw3000_read_fifo(uint8_t addr, uint8_t *data, uint16_t length)
{
 uint8_t i=0;
 uint8_t j=0;
 uint8_t temp=0;
 
 addr=addr&0x7F;
 GPIO_WriteLow(HW3000_CSN_PORT,HW3000_CSN_IO);
 
 for(i=0;i<8;i++)
 {
  GPIO_WriteHigh(HW3000_CLK_PORT,HW3000_CLK_IO);
  if(addr&0x80)
  {
   GPIO_WriteHigh(HW3000_SDO_PORT,HW3000_SDO_IO);
  }
  else
  {
   GPIO_WriteLow(HW3000_SDO_PORT,HW3000_SDO_IO);
  }
  GPIO_WriteLow(HW3000_CLK_PORT,HW3000_CLK_IO);
  addr<<=1;
 }
 
 for(j=0;j<length;j++)
 {
  for(i=0;i<8;i++)
  {
   temp<<=1;
   GPIO_WriteHigh(HW3000_CLK_PORT,HW3000_CLK_IO);
   GPIO_WriteLow(HW3000_CLK_PORT,HW3000_CLK_IO);
   if(GPIO_ReadInputPin(HW3000_SDI_PORT,HW3000_SDI_IO)==SET)
    {
     temp|=0x01;
    }
   else
    {
     temp&=0xfe;
    }
  }
  data[j]=temp;
}
 
GPIO_WriteHigh(HW3000_CSN_PORT,HW3000_CSN_IO);
}

