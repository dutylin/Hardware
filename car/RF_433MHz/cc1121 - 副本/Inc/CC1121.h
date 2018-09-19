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
}CC1121_RF_RX_DATA;
typedef  struct {
  unsigned char rf_tx_start_flag;
  unsigned char rf_tx_data_len;
  unsigned char rf_tx_data_buff[20];  
  unsigned char rf_tx_pkt_rssi;
}CC1121_RF_txDATA_DEF;
/***********************************CC1121 SetModule**************************/

#define	EXTEND_G2	0xA0			//扩展寄存器的第2组最高有效地址

#define	FRAME_SYN	0x5F		//数据中的帧同步码
#define	PREAMBLE_CODE	0xAA		//前导码
#define	END_CODE	0xFF		//结束符，结束符是加扰码的反码
#define	PN9_INI		0x89		//PN9码初始值
		
#define	MAXDATALEN	48		//1帧数据最长长度,不能>100(实际测试串口9600n81时，空中自动组包的最大有效字节32个，取48个也可)
#define	MAXFECLEN	2*(MAXDATALEN+10)+8	//经过FEC编码2倍后，数据包的可能最大长度，最多增加10个透明处理的字符。(包括结束符的8个字节)
#define	PKT_LEN		0x0D		// 桢长度为13字节 

#define	CC112X_FREQ2_init	0x6C//430.0000MHz
#define	CC112X_FREQ1_init	0x40// 
#define	CC112X_FREQ0_init	0x00// 

#define	CC112X_FREQ2B_init	0x77//479.5078MHz
#define	CC112X_FREQ1B_init	0xE0// 
#define	CC112X_FREQ0B_init	0x80// 



#define	PW0			0x75//8mw
#define	PW1			0x78//10mw
#define	PW2			0x7A//15mw
#define	PW3			0x7C//20mw
#define	PW4			0x7E//25mw
#define	PW5			0x7F//30mw
/*************************************cc1121 CMD define************************/


#define	BARKER_CONST1	0x8D	//巴克码常数1,最高位无效, 	10001101	//31位帧同步码
#define	BARKER_CONST2	0xD4	//巴克码常数2		11010100
#define	BARKER_CONST3	0x25	//巴克码常数3		00100101
#define	BARKER_CONST4	0x9F	//巴克码常数4		10011111


#define	USERENDBYTE	0x55
#define	USERSTARTBYTE	0x73
#define	RSSI_OFFSET	102		////是补偿

#define	CC112x_SRES	0x30
#define	CC112x_SFSTXON	0x31
#define	CC112x_SXOFF	0x32
#define	CC112x_SCAL	0x33
#define	CC112x_SRX	0x34
#define	CC112x_STX	0x35
#define	CC112x_SIDLE	0x36
#define	CC112x_SAFC	0x37
#define	CC112x_SWOR	0x38
#define	CC112x_SPWD	0x39
#define	CC112x_SFRX	0x3A
#define	CC112x_SFTX	0x3B
#define	CC112x_SWORRST	0x3C
#define	CC112x_SNOP	0x3D

#define	CC112x_FIFO	0x3F

/*******************************register address*******************************/

#define CC112X_IOCFG3             0x00
#define CC112X_IOCFG2             0x01
#define CC112X_IOCFG1             0x02
#define CC112X_IOCFG0             0x03
#define CC112X_SYNC3              0x04
#define CC112X_SYNC2              0x05
#define CC112X_SYNC1              0x06
#define CC112X_SYNC0              0x07
#define CC112X_SYNC_CFG1          0x08
#define CC112X_SYNC_CFG0          0x09
#define CC112X_DEVIATION_M        0x0A
#define CC112X_MODCFG_DEV_E       0x0B
#define CC112X_DCFILT_CFG         0x0C
#define CC112X_PREAMBLE_CFG1      0x0D
#define CC112X_PREAMBLE_CFG0      0x0E
#define CC112X_FREQ_IF_CFG        0x0F
#define CC112X_IQIC               0x10
#define CC112X_CHAN_BW            0x11
#define CC112X_MDMCFG1            0x12
#define CC112X_MDMCFG0            0x13
#define CC112X_DRATE2             0x14
#define CC112X_DRATE1             0x15
#define CC112X_DRATE0             0x16
#define CC112X_AGC_REF            0x17
#define CC112X_AGC_CS_THR         0x18
#define CC112X_AGC_GAIN_ADJUST    0x19
#define CC112X_AGC_CFG3           0x1A
#define CC112X_AGC_CFG2           0x1B
#define CC112X_AGC_CFG1           0x1C
#define CC112X_AGC_CFG0           0x1D
#define CC112X_FIFO_CFG           0x1E
#define CC112X_DEV_ADDR           0x1F
#define CC112X_SETTLING_CFG       0x20
#define CC112X_FS_CFG             0x21
#define CC112X_WOR_CFG1           0x22
#define CC112X_WOR_CFG0           0x23
#define CC112X_WOR_EVENT0_MSB     0x24
#define CC112X_WOR_EVENT0_LSB     0x25
#define CC112X_PKT_CFG2           0x26
#define CC112X_PKT_CFG1           0x27
#define CC112X_PKT_CFG0           0x28
#define CC112X_RFEND_CFG1         0x29
#define CC112X_RFEND_CFG0         0x2A
#define CC112X_PA_CFG2            0x2B
#define CC112X_PA_CFG1            0x2C
#define CC112X_PA_CFG0            0x2D
#define CC112X_PKT_LEN            0x2E

#define CC112X_IF_MIX_CFG         0x00
#define CC112X_FREQOFF_CFG        0x01
#define CC112X_TOC_CFG            0x02
#define CC112X_MARC_SPARE         0x03
#define CC112X_ECG_CFG            0x04
#define CC112X_SOFT_TX_DATA_CFG   0x05
#define CC112X_EXT_CTRL           0x06
#define CC112X_RCCAL_FINE         0x07
#define CC112X_RCCAL_COARSE       0x08
#define CC112X_RCCAL_OFFSET       0x09
#define CC112X_FREQOFF1           0x0A
#define CC112X_FREQOFF0           0x0B
#define CC112X_FREQ2              0x0C
#define CC112X_FREQ1              0x0D
#define CC112X_FREQ0              0x0E
#define CC112X_IF_ADC2            0x0F
#define CC112X_IF_ADC1            0x10
#define CC112X_IF_ADC0            0x11
#define CC112X_FS_DIG1            0x12
#define CC112X_FS_DIG0            0x13
#define CC112X_FS_CAL3            0x14
#define CC112X_FS_CAL2            0x15
#define CC112X_FS_CAL1            0x16
#define CC112X_FS_CAL0            0x17
#define CC112X_FS_CHP             0x18
#define CC112X_FS_DIVTWO          0x19
#define CC112X_FS_DSM1            0x1A
#define CC112X_FS_DSM0            0x1B
#define CC112X_FS_DVC1            0x1C
#define CC112X_FS_DVC0            0x1D
#define CC112X_FS_LBI             0x1E
#define CC112X_FS_PFD             0x1F
#define CC112X_FS_PRE             0x20
#define CC112X_FS_REG_DIV_CML     0x21
#define CC112X_FS_SPARE           0x22
#define CC112X_FS_VCO4            0x23
#define CC112X_FS_VCO3            0x24
#define CC112X_FS_VCO2            0x25
#define CC112X_FS_VCO1            0x26
#define CC112X_FS_VCO0            0x27
#define CC112X_GBIAS6             0x28
#define CC112X_GBIAS5             0x29
#define CC112X_GBIAS4             0x2A
#define CC112X_GBIAS3             0x2B
#define CC112X_GBIAS2             0x2C
#define CC112X_GBIAS1             0x2D
#define CC112X_GBIAS0             0x2E
#define CC112X_IFAMP              0x2F
#define CC112X_LNA                0x30
#define CC112X_RXMIX              0x31
#define CC112X_XOSC5              0x32
#define CC112X_XOSC4              0x33
#define CC112X_XOSC3              0x34
#define CC112X_XOSC2              0x35
#define CC112X_XOSC1              0x36
#define CC112X_XOSC0              0x37
#define CC112X_ANALOG_SPARE       0x38
#define CC112X_PA_CFG3            0x39
#define CC112X_IRQ0M              0x3F
#define CC112X_IRQ0F              0x40
#define CC112X_WOR_TIME1          0x64
#define CC112X_WOR_TIME0          0x65
#define CC112X_WOR_CAPTURE1       0x66
#define CC112X_WOR_CAPTURE0       0x67
#define CC112X_BIST               0x68
#define CC112X_DCFILTOFFSET_I1    0x69
#define CC112X_DCFILTOFFSET_I0    0x6A
#define CC112X_DCFILTOFFSET_Q1    0x6B
#define CC112X_DCFILTOFFSET_Q0    0x6C
#define CC112X_IQIE_I1            0x6D
#define CC112X_IQIE_I0            0x6E
#define CC112X_IQIE_Q1            0x6F
#define CC112X_IQIE_Q0            0x70
#define CC112X_RSSI1              0x71
#define CC112X_RSSI0              0x72
#define CC112X_MARCSTATE          0x73
#define CC112X_LQI_VAL            0x74
#define CC112X_PQT_SYNC_ERR       0x75
#define CC112X_DEM_STATUS         0x76
#define CC112X_FREQOFF_EST1       0x77
#define CC112X_FREQOFF_EST0       0x78
#define CC112X_AGC_GAIN3          0x79
#define CC112X_AGC_GAIN2          0x7A
#define CC112X_AGC_GAIN1          0x7B
#define CC112X_AGC_GAIN0          0x7C
#define CC112X_SOFT_RX_DATA_OUT   0x7D
#define CC112X_SOFT_TX_DATA_IN    0x7E
#define CC112X_ASK_SOFT_RX_DATA   0x7F
#define CC112X_RNDGEN             0x80
#define CC112X_MAGN2              0x81
#define CC112X_MAGN1              0x82
#define CC112X_MAGN0              0x83
#define CC112X_ANG1               0x84
#define CC112X_ANG0               0x85
#define CC112X_CHFILT_I2          0x86
#define CC112X_CHFILT_I1          0x87
#define CC112X_CHFILT_I0          0x88
#define CC112X_CHFILT_Q2          0x89
#define CC112X_CHFILT_Q1          0x8A
#define CC112X_CHFILT_Q0          0x8B
#define CC112X_GPIO_STATUS        0x8C
#define CC112X_FSCAL_CTRL         0x8D
#define CC112X_PHASE_ADJUST       0x8E
#define CC112X_PARTNUMBER         0x8F
#define CC112X_PARTVERSION        0x90
#define CC112X_SERIAL_STATUS      0x91
#define CC112X_RX_STATUS          0x92
#define CC112X_TX_STATUS          0x93
#define CC112X_MARC_STATUS1       0x94
#define CC112X_MARC_STATUS0       0x95
#define CC112X_PA_IFAMP_TEST      0x96
#define CC112X_FSRF_TEST          0x97
#define CC112X_PRE_TEST           0x98
#define CC112X_PRE_OVR            0x99
#define CC112X_ADC_TEST           0x9A
#define CC112X_DVC_TEST           0x9B
#define CC112X_ATEST              0x9C
#define CC112X_ATEST_LVDS         0x9D
#define CC112X_ATEST_MODE         0x9E
#define CC112X_XOSC_TEST1         0x9F
#define CC112X_XOSC_TEST0         0xA0
#define CC112X_NUM_TXBYTES        0xD6
#define CC112X_NUM_RXBYTES        0xD7

/******************************************************************************/


#define  CC1121_WriteExtend_Addr      0x2F
#define  CC1121_ReadExtend_Addr      0xAF


typedef  struct {
  unsigned char reg_addr;
  unsigned char reg_value;
}CC1121_REG_Def;

void  RF_CC1121_Init(void);
void  RF_CC1121_Calibration(void);
void  RF_CC1121_Change_Freq(void);
void  RF_CC1121_Get_Data(void);
void RF_CC1121_Set_RX_Module(void);
unsigned char  RF_CC1121_Get_STATUS(void);
unsigned char RF_Part_Number(void);

void RF_Process(void);
void RF_init(void);

#endif

