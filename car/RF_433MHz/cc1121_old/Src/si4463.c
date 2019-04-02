// ========================================================
/// @file       DK_SPI.c
/// @brief      RF IO Configuration
/// @version    V1.0
/// @date       2016/08/19
/// @company    WooZoom Co., Ltd.
/// @website    http://www.woozoom.net
/// @author     ZhangMeng
/// @mobile     +86-13804023611
// ========================================================
#include "crc.h"
#include "time.h"
#include "radio_config_si4463.h"
#include "RF_air_rate.h"
#include "si4463_gpio.h"
#include "systime.h"
#include "Si4463.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx.h"
#include "flash.h"
#include "RF_parameter_setting.h"
/* USER CODE BEGIN 0 */

#define down_load_id 1
//RF Register define

#define PART_INFO                       	0x01
#define FUNC_INFO                       	0x10
#define SET_PROPERTY                    	0x11
#define GET_PROPERTY                    	0x12
#define GPIO_PIN_CFG                    	0x13
#define GET_ADC_READING                 	0x14
#define FIFO_INFO                       	0x15
#define PACKET_INFO                     	0x16
#define IRCAL                           	0x17
#define PROTOCOL_CFG                    	0x18
#define GET_INT_STATUS                  	0x20
#define GET_PH_STATUS                   	0x21
#define GET_MODEM_STATUS              		0x22
#define GET_CHIP_STATUS                 	0x23
#define START_TX                        	0x31
#define START_RX                        	0x32
#define REQUEST_DEVICE_STAT         			0x33
#define CHANGE_STATE                    	0x34
#define READ_CMD_BUFF                   	0x44
#define FRR_A_READ                      	0x50
#define FRR_B_READ                      	0x51
#define FRR_C_READ                      	0x53
#define FRR_D_READ                      	0x57
#define WRITE_TX_FIFO                   	0x66
#define READ_RX_FIFO                    	0x77
#define START_MFSK                      	0x35
#define RX_HOP                          	0x36


// payload length
#define   payload_length  				58


// WDS generate parameters
const unsigned char uRF_FREQ_CONTROL[3]= {0x08, 0x00, 0x00};
// unsigned char RF_FREQ_CONTROL_INTE_8_data[] = 		{RF_FREQ_CONTROL_INTE_8};
const  unsigned char RF_FREQ_CONTROL_INTE_8_data[] = 		{0x11, 0x40, 0x08, 0x00, 0x3C, 0x08, 0x00, 0x00, 0x22, 0x22, 0x20, 0xFF};
const   unsigned char RF_POWER_UP_data[] = 			   		{ RF_POWER_UP};
const   unsigned char RF_FRR_CTL_A_MODE_4_data[] = 		   	{ RF_FRR_CTL_A_MODE_4};
const   unsigned char RF_MODEM_FREQ_DEV_0_1_data[] = 		   	{ RF_MODEM_FREQ_DEV_0_1};
const   unsigned char RF_MODEM_AGC_CONTROL_1_data[] = 		{ RF_MODEM_AGC_CONTROL_1};
const   unsigned char RF_MODEM_MOD_TYPE_12_data[]=			{RF_MODEM_MOD_TYPE_12};
const   unsigned char RF_MODEM_TX_RAMP_DELAY_12_data[]=				{RF_MODEM_TX_RAMP_DELAY_12};
const   unsigned char BCR_NCO_OFFSET_2_12_data[]=					{RF_MODEM_BCR_NCO_OFFSET_2_12};
const   unsigned char RF_MODEM_AFC_LIMITER_1_3_data[]=						{RF_MODEM_AFC_LIMITER_1_3};
const   unsigned char AGC_WINDOW_SIZE_12_data[]=				{RF_MODEM_AGC_WINDOW_SIZE_12};
const   unsigned char RF_MODEM_RAW_CONTROL_8_data[]=					{RF_MODEM_RAW_CONTROL_10};
const   unsigned char COE13_7_0_12_data[]=	{RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12};
const   unsigned char COE1_7_0_12_data[]=	{RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12};
const   unsigned char COE7_7_0_12_data[]=	{RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12};
const   unsigned char RF_SYNTH_PFDCP_CPFF_7_data[]=					{RF_SYNTH_PFDCP_CPFF_7};
const   unsigned char RF_MODEM_RAW_SEARCH2_2_data[]= {RF_MODEM_RAW_SEARCH2_2};

//spi uart dma extern define
extern UART_HandleTypeDef huart1;
//extern IWDG_HandleTypeDef hiwdg;
extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern uint32_t uBrate;
extern uint32_t uart_rx_len;
//extern unsigned char rf_rx_FLAG;
extern unsigned char recv_end_flag;
extern uint32_t uart_dma_state;

unsigned char tx_ph_data[buffer_size] ;  // test signal,The 10th data is a former nine checksum
unsigned char rx_ph_data[buffer_size];
unsigned char setting_mode_flag=0;

U8 version[4]=	{'V',1,'.',0}; //version
U8 rf_id[4]=		{'s','w',0,1};//header match
U8 freq_channel	=	0;//frequency channel num
U8 POWER		=	127;//SET power max 127
unsigned char check_cnt;
//uint32_t uart_real_tx=0;
uint32_t uart_tx_cnt;//uart send data cnt from rf
//uint32_t packet_index_len=0;
//uint32_t rf_rx_cnt2;
uint32_t packet_overrun_div;//divede packet to send by rf
uint32_t tmp1=0,tmp2=0;
uint32_t rf_tx_cnt=0; //rf real tx data cnt
uint32_t rf_rx_cnt=0; //rf real rx data cnt
uint32_t rf_rx_base_addr=0;//every packet base addr index

U8 spi_read_buf[20];  //status buffer
U8 tx_len;
U8 rx_len;

unsigned char nIRQ=1;//irq flag
unsigned char tx_rx_switch=0;//tx_rx flag
unsigned int rf_tx_index=0;//tx index
unsigned int rf_rx_index=0;//rx index
unsigned int uart_tx_index=0;//uart tx index
unsigned int rf_pack_bytes;//temp cnt
unsigned int first_tx=0;
//unsigned int rf_ID_original;
unsigned char state_idle;
uint32_t real_tx_len;
unsigned int rf_air_rate=1200;

void uRF_rate_SETTING(unsigned char *p)
{
	rf_air_rate=(((unsigned int)p[0])<<8)+p[1];
}

//set rf id
void uID_SETTING(unsigned char *p)
{
	rf_id[3]=(unsigned char)p[1];
	rf_id[2]=(unsigned char)p[0];
//	rf_id[2]=(unsigned char)y;
//	rf_id[3]=(unsigned char)y;

}

//set rf freq
//void uCenter_FREQ(unsigned char *p)
//{
//	uRF_FREQ_CONTROL[0]=p[0];
//	uRF_FREQ_CONTROL[0]=p[1];
//	uRF_FREQ_CONTROL[0]=p[2];
//}

//tx power max:127
void uPA_SETTING(unsigned char *p)
{
	switch(p[0])
	{
	case 5:
		POWER=31;
		break;
	case 10:
		POWER=63;
		break;
	case 15:
		POWER=95;
		break;
	case 20:
		POWER=127;
		break;
	default:
		POWER=80;
		break;

	}

}

//channel set 0-255
void uCH_SETTING(unsigned char *p)
{
	freq_channel=p[0];
}

//rf firmware information
void RF_info(unsigned char *p)
{

	p[0]='r';
	p[1]='o';
	p[2]='o';
	p[3]='z';
	p[4]='o';
	p[5]='o';
	p[6]='m';


	// uart burate
	p[7]=(unsigned char)(uBrate>>8);
	p[8]=(unsigned char)(uBrate);


//air burate
	p[9]=(unsigned char)(rf_air_rate>>8);
	p[10]=(unsigned char)(rf_air_rate);

//rf id
	p[11]=rf_id[2];
	p[12]=rf_id[3];

	//rf pa
	switch(POWER)
	{
	case 31:
		p[13]=5;
		break;
	case 63:
		p[13]=10;
		break;
	case 95:
		p[13]=15;
		break;
	case 127:
		p[13]=20;
		break;
	default:
		p[13]=15;
		break;

	}



	//rf channel
	p[14]=freq_channel;


//	p[15]=version[1];
//	p[16]=version[3];


}
static uint8_t CpuID[4];
static uint32_t Lock_Code;
static uint32_t Lock_Code1;
void GetLockCode(void)
{
//??CPU??ID
	CpuID[0]=*(volatile uint8_t*)(0x1FFFF7AC);
	CpuID[1]=*(volatile uint8_t*)(0x1FFFF7Ad);
	CpuID[2]=*(volatile uint8_t*)(0x1FFFF7Ae);
	CpuID[3]=*(volatile uint8_t*)(0x1FFFF7Af);
//????,????????
	Lock_Code1=(CpuID[0]<<24)+(CpuID[1]<<16)+(CpuID[2]<<8)+CpuID[3];

}
void rev_ID_write(uint32_t tmp)
{
	//	flash_init();
	flash_write(0,tmp,1);
	//RF_parameters_save();
}
uint32_t rev_ID_read(void)
{
	uint32_t addr;
	uint32_t tmp;
	addr=0;
	flash_read(addr,&tmp,1);
	return tmp;
}

void RF_parameters_init()
{
	uint32_t WR_FLAG;
	flash_read(1,&WR_FLAG,1);
#if down_load_id
	GetLockCode();
	if(WR_FLAG!=0x5a)
	{
		flash_init();

		flash_write(1,0x5a,1);
		RF_parameters_save();
	}

#else
	if(WR_FLAG!=0x5a)
	{
#if first_wr
		GetLockCode();
#endif

		flash_init();
		flash_write(1,0x5a,1);
		RF_parameters_save();
	}
#endif

}

//rf parameters save in flash
void RF_parameters_save()
{
	uint32_t addr;
	addr=2;

	Lock_Code=	rev_ID_read();
	flash_init();
#if down_load_id
	rev_ID_write(Lock_Code1);
#else
	rev_ID_write(Lock_Code);
#endif


	flash_write(1,0x5a,1);
	flash_write(addr++,uBrate,1);	//uart baudrate set

	flash_write(addr++,freq_channel,1);	//rf channel

	flash_write(addr++,POWER,1);	//rf tx power

	flash_write(addr++,rf_air_rate,1);	//rf center freq,3bytes
	flash_write(addr++,RF_FREQ_CONTROL_INTE_8_data[6],1);
	flash_write(addr++,RF_FREQ_CONTROL_INTE_8_data[7],1);

	flash_write(addr++,rf_id[2],1);	//rf id,2bytes
	flash_write(addr++,rf_id[3],1);
}
void default_rf_P(void)
{
	uBrate= 9600;
	freq_channel=0;
	POWER=127;
	rf_air_rate=1200;
	rf_id[0]='s';
	rf_id[1]='w';
	rf_id[2]='w';
	rf_id[3]='x';

}
//rf parameters read from mcu flash
void RF_parameters_read()
{
	uint32_t addr;
	uint32_t tmp;
err:
	addr=2;
	Lock_Code=	rev_ID_read();
	GetLockCode();
	if(Lock_Code != Lock_Code1)
	{
		goto err;
	} // ??
	flash_read(addr++,&tmp,1);
	uBrate = (unsigned int)tmp;

	flash_read(addr++,&tmp,1);
	freq_channel=(U8)tmp;

	flash_read(addr++,&tmp,1);
	POWER=(U8)tmp;

	flash_read(addr++,&tmp,1);
	rf_air_rate=(unsigned int)tmp;

	flash_read(addr++,&tmp,1);
	flash_read(addr++,&tmp,1);


	flash_read(addr++,&tmp,1);
	rf_id[2]=(U8)tmp;

	flash_read(addr++,&tmp,1);
	rf_id[3]=(U8)tmp;

	if(uBrate<=0||uBrate>19200||POWER<=0||POWER>127||rf_air_rate<1200||rf_air_rate>19200)
	{
		default_rf_P();
	}

}


//void rf_tx()
//{
//	while(tx_rx_switch==1)
//	{
////		HAL_IWDG_Refresh(&hiwdg);
//		if(first_tx==0)
//		{
//			first_tx=1;
//			tx_data();		// Tx data
//		}
//		else
//		{
//			if(nIRQ==0)
//			{
//				tx_data();		// Tx data
//			}
//		}
//	}
//}
//void rf_rx()
//{
//	if(!nIRQ)
//	{
//		state_idle=2;
//		clr_interrupt();   // clear interrupt
//		if((spi_read_buf[4] &0x08) == 0)  // crc error check
//		{
//			spi_read_fifo();
//		}
//		rx_init();
//		//rf_rx_FLAG=0;
//		fifo_reset();
//		state_idle=0;
//	}
//}
//void RF_TX_RX_CTRL()
//{
//		if(recv_end_flag==1)
//		{
//			recv_end_flag=0;
//			setting_mode_flag=setting_rf_bySSCOM();
//			if(setting_mode_flag==0)
//			{
//				tx_rx_sw();
//			}
////			else
////			{
////				rf_tx_cnt=0;
////        uart_rx_len = 0;
////			}
//		}
//}
//void RF_Process()
//{
//		if(tx_rx_switch==0&&state_idle!=1)
//		{
//			rf_rx();
////			if(setting_mode_flag==0)
////			{
////				if(uart_dma_state==0x00000001)
////				{
////					uart_tx_DMA();   //transmit data by uart
////				}
////			}
//		}
//		rf_tx();//tx data
//}

//void SI4463_init(void)
//{
//	U8 app_command_buf[20];

//	//spi_write(0x07, RF_GPIO_PIN_CFG_data);
//	app_command_buf[0] = 0x13;			// SET GPIO PORT
//	app_command_buf[1]  = 0x14; 		// gpio 0 ,Rx data
//	app_command_buf[2]  = 0x02;    		// gpio1, output 0
//	app_command_buf[3]  = 0x21;  		// gpio2, hign while in receive mode
//	app_command_buf[4]  = 0x20; 		// gpio3, hign while in transmit mode
//	app_command_buf[5]  = 0x27;   		// nIRQ
//	app_command_buf[6]  = 0x0b;  		// sdo
//	spi_write(7, app_command_buf);

//	// spi_write(0x05, RF_GLOBAL_XO_TUNE_1_data);
//	app_command_buf[0] = 0x11;
//	app_command_buf[1]  = 0x00;
//	app_command_buf[2]  = 0x01;
//	app_command_buf[3]  = 0x00;
//	app_command_buf[4]  = 125;  			// freq  adjustment
//	spi_write(5, app_command_buf);

//	// spi_write(0x05, RF_GLOBAL_CONFIG_1_data);
//	app_command_buf[0] = 0x11;
//	app_command_buf[1]  = 0x00;
//	app_command_buf[2]  = 0x01;
//	app_command_buf[3]  = 0x03;
//	app_command_buf[4]  = 0x40;  		// tx = rx = 64 byte,PH,high performance mode
//	spi_write(5, app_command_buf);

//	spi_write(0x08, (unsigned char *)RF_FRR_CTL_A_MODE_4_data);    // disable all fast response register

//	// spi_write(0x0D, RF_PREAMBLE_TX_LENGTH_9_data); // set Preamble
//	app_command_buf[0] = 0x11;
//	app_command_buf[1]  = 0x10;
//	app_command_buf[2]  = 0x09;
//	app_command_buf[3]  = 0x00;
//	app_command_buf[4]  = 0x08;							//  8 bytes Preamble
//	app_command_buf[5]  = 0x14;							//  detect 20 bits
//	app_command_buf[6]  = 0x00;
//	app_command_buf[7]  = 0x0f;
//	app_command_buf[8]  = 0x31;  						//  no manchest.1010.??
//	app_command_buf[9]  = 0x00;
//	app_command_buf[10]  = 0x00;
//	app_command_buf[11]  = 0x00;
//	app_command_buf[12]  = 0x00;
//	spi_write(13, app_command_buf);

//	//  RF_SYNC_CONFIG_5_data,							// set sync
//	app_command_buf[0] = 0x11;
//	app_command_buf[1]  = 0x11;
//	app_command_buf[2]  = 0x05;
//	app_command_buf[3]  = 0x00;
//	app_command_buf[4]  = 0x01;   						// no manchest , 2 bytes
//	app_command_buf[5]  = 0x2d;   						// sync byte3
//	app_command_buf[6]  = 0xd4;							// sync byte2
//	app_command_buf[7]  = 0x00;							// sync byte1
//	app_command_buf[8]  = 0x00;							// sync byte0
//	spi_write(9, app_command_buf);


//	//  packet crc
//	app_command_buf[0] = 0x11;
//	app_command_buf[1]  = 0x12;
//	app_command_buf[2]  = 0x01;
//	app_command_buf[3]  = 0x00;
//	app_command_buf[4]  = 0x81;							// CRC = itu-c, enable crc
//	spi_write(5, app_command_buf);

//	// packet   gernale configuration
//	app_command_buf[0] = 0x11;
//	app_command_buf[1]  = 0x12;
//	app_command_buf[2]  = 0x01;
//	app_command_buf[3]  = 0x06;
//	app_command_buf[4]  = 0x02;							// CRC MSB, data MSB
//	spi_write(5, app_command_buf);

//	// spi_write(0x07, RF_PKT_LEN_3_data);
//	app_command_buf[0] = 0x11;
//	app_command_buf[1]  = 0x12;
//	app_command_buf[2]  = 0x03;
//	app_command_buf[3]  = 0x08;
//	app_command_buf[4]  = 0x0a;
//	app_command_buf[5]  = 0x01;
//	app_command_buf[6]  = 0x00;
//	spi_write(7, app_command_buf);

//	app_command_buf[0]  = 0x11;
//	app_command_buf[1]  = 0x12;
//	app_command_buf[2]  = 0x0c;
//	app_command_buf[3]  = 0x0d;
//	app_command_buf[4]  = 0x00;
//	app_command_buf[5]  = 0x05;
//	app_command_buf[6]  = 0x04;
//	app_command_buf[7]  = 0xaa;
//	app_command_buf[8]  = 0x00;
//	app_command_buf[9]  = payload_length;
//	app_command_buf[10]  = 0x00;
//	app_command_buf[11]  = 0xaa;
//	app_command_buf[12]  = 0x00;
//	app_command_buf[13]  = 0x00;
//	app_command_buf[14]  = 0x00;
//	app_command_buf[15]  = 0x00;
//	spi_write(16, app_command_buf);					// set length of Field 1 -- 4

//	// spi_write(0x0C, RF_PKT_FIELD_4_LENGTH_12_8_8_data);
//	app_command_buf[0] = 0x11;
//	app_command_buf[1]  = 0x12;
//	app_command_buf[2]  = 0x08;
//	app_command_buf[3]  = 0x19;
//	app_command_buf[4]  = 0x00;
//	app_command_buf[5]  = 0x00;
//	app_command_buf[6]  = 0x00;
//	app_command_buf[7]  = 0x00;
//	app_command_buf[8]  = 0x00;
//	app_command_buf[9]  = 0x00;
//	app_command_buf[10]  = 0x00;
//	app_command_buf[11]  = 0x00;
//	spi_write(12, app_command_buf);

//	spi_write(0x10, (unsigned char *)RF_MODEM_MOD_TYPE_12_data);
//	spi_write(0x05, (unsigned char *)RF_MODEM_FREQ_DEV_0_1_data);

//	spi_write(0x10, (unsigned char *)RF_MODEM_TX_RAMP_DELAY_12_data);
//	spi_write(0x10, (unsigned char *)BCR_NCO_OFFSET_2_12_data);
//	spi_write(0x10, (unsigned char *)RF_MODEM_TX_RAMP_DELAY_12_data);
//	spi_write(0x07, (unsigned char *)RF_MODEM_AFC_LIMITER_1_3_data);
//	//spi_write(0x10, BCR_NCO_OFFSET_2_12_data);

//	spi_write(0x05, (unsigned char *)RF_MODEM_AGC_CONTROL_1_data);
//	spi_write(0x10, (unsigned char *)AGC_WINDOW_SIZE_12_data);
//	spi_write(0x0c, (unsigned char *)RF_MODEM_RAW_CONTROL_8_data);
//	//	spi_write(0x10, (unsigned char *)AGC_WINDOW_SIZE_12_data);

//	// spi_write(0x05, RF_MODEM_RSSI_COMP_1_data);
//	app_command_buf[0] = 0x11;
//	app_command_buf[1] = 0x20;
//	app_command_buf[2] = 0x01;
//	app_command_buf[3] = 0x4e;
//	app_command_buf[4]  = 0x40;
//	spi_write(5, app_command_buf);

//	spi_write(0x10, (unsigned char *)COE13_7_0_12_data);
//	spi_write(0x10, (unsigned char *)COE1_7_0_12_data);
//	spi_write(0x10, (unsigned char *)COE7_7_0_12_data);

//	// RF_PA
//	app_command_buf[0] = 0x11;
//	app_command_buf[1]  = 0x22;
//	app_command_buf[2]  = 0x04;
//	app_command_buf[3]  = 0x00;
//	app_command_buf[4]  = 0x08;
//	app_command_buf[5]  = POWER;							// set max power
//	app_command_buf[6]  =0x00;
//	app_command_buf[7]  = 0x3d;
//	spi_write(8, app_command_buf);

//	spi_write(0x0B, (unsigned char *)RF_SYNTH_PFDCP_CPFF_7_data);

//	// header match
//	app_command_buf[0] = 0x11;
//	app_command_buf[1]  = 0x30;
//	app_command_buf[2]  = 0x0c;
//	app_command_buf[3]  = 0x00;
//	app_command_buf[4]  = rf_id[0];
//	app_command_buf[5]  = 0xff;
//	app_command_buf[6]  = 0x40;
//	app_command_buf[7]  = rf_id[1];
//	app_command_buf[8]  = 0xff;
//	app_command_buf[9]  = 0x01;
//	app_command_buf[10] = rf_id[2];
//	app_command_buf[11]  =0xff;
//	app_command_buf[12]  =0x02;
//	app_command_buf[13]  = rf_id[3];
//	app_command_buf[14]  = 0xff;
//	app_command_buf[15]  =0x03;
//	spi_write(16, app_command_buf); 					// ????í·??

//	spi_write(6, (unsigned char *)RF_MODEM_RAW_SEARCH2_2_data);
//
//
////	RF_FREQ_CONTROL_INTE_8_data[5] =uRF_FREQ_CONTROL[0];
//// 	RF_FREQ_CONTROL_INTE_8_data[6] =uRF_FREQ_CONTROL[1];
//// 	RF_FREQ_CONTROL_INTE_8_data[7] =uRF_FREQ_CONTROL[2];
//	spi_write(12, (unsigned char *)RF_FREQ_CONTROL_INTE_8_data); 	    // set frequency
//}
//void fifo_cfg(U8 smode)
//{
//	U8 app_command_buf[20];
//	app_command_buf[0]  = 0x11;
//	app_command_buf[1]  = 0x12;
//	app_command_buf[2]  = 0x0c;
//	app_command_buf[3]  = 0x0d;
//	app_command_buf[4]  = 0x00;
//	app_command_buf[5]  = 0x05;
//	app_command_buf[6]  = 0x04;
//	app_command_buf[7]  = 0xaa;
//	app_command_buf[8]  = 0x00;
//	if(smode)
//		app_command_buf[9]  = (payload_length+5);
//	else
//		app_command_buf[9]  = tx_len;
//	app_command_buf[10]  = 0x00;
//	app_command_buf[11]  = 0xaa;
//	app_command_buf[12]  = 0x00;
//	app_command_buf[13]  = 0x00;
//	app_command_buf[14]  = 0x00;
//	app_command_buf[15]  = 0x00;
//	spi_write(16, app_command_buf);				// éè?? Field 1--43¤?è
//}
//void fifo_reset(void)			// ?′??·￠é?oí?óê? FIFO
//{
//	U8 p[2];
//	p[0] = FIFO_INFO;
//	p[1] = 0x03;   // reset tx ,rx fifo
//	spi_write(2,p);
//}

//void clr_interrupt(void)		// clar interrupt
//{
//	U8 p[4];
//	nIRQ=1;
//	p[0] = GET_INT_STATUS;
//	p[1] = 0;   // clr  PH pending
//	p[2] = 0;   // clr modem_pending
//	p[3] = 0;   // clr chip pending
//	spi_write(4,p);
//	spi_read(9,GET_INT_STATUS);
//}
//
//void enable_rx_interrupt(void)
//{
//	U8 p[7];

//	p[0] = 0x11;
//	p[1] = 0x01;  // 0x0100
//	p[2] = 0x03;// 3 parameters
//	p[3] = 0x00;   // 0100
//	p[4] = 0x03;   // ph, modem int
//	p[5] = 0x18; // 0x10;   // Pack received int
//	p[6] = 0x00;   //preamble int, sync int setting
//	spi_write(0x07, p);  // enable  packet receive interrupt
//}

//void enable_tx_interrupt(void)
//{
//	U8 p[6];

//	p[0] = 0x11;
//	p[1] = 0x01;
//	p[2] = 0x02;
//	p[3] = 0x00;
//	p[4] = 0x01;
//	p[5] = 0x20;
//	spi_write(0x06, p);
//}

//void rf_standby(void)
//{
//	U8 p[2];
//
//	p[0] = CHANGE_STATE ;	// CHANGE_STATE
//	p[1] = 0x01 ; // sleep mode
//	spi_write(2, p);
//}

//void tx_start(void)
//{
//	U8 p[5];
//
//	p[0] = START_TX ;
//	p[1] = freq_channel ; 		//	channel 0
//	p[2] = 0x30;
//	p[3] = 0;
//	p[4] = 0;
//	spi_write(5, p);
//}

//void rx_start(void)
//{
//	U8 p[8];
//
//	p[0] = START_RX ;
//	p[1] = freq_channel ; 			//	channel set
//	p[2] = 0x00;
//	p[3] = 0;
//	p[4] = 0;
//	p[5] = 0;
//	p[6] = 0x08;
//	p[7] = 0x08;
//	spi_write(8, p);
//}
//
//void rx_init(void)
//{

//	fifo_cfg(1);
//	fifo_reset();  					// ???? fifo
//	enable_rx_interrupt();
//	clr_interrupt();
//	rx_start();
//}


//void tx_rx_sw(void)
//{
//	if(rf_tx_cnt!=uart_rx_len)
//	{
//		tx_rx_switch=1;
//	}
//}


//void tx_data(void)
//{
//	state_idle=1;
//	rf_pack_bytes=uart_rx_len-rf_tx_cnt;
//	if(rf_pack_bytes<payload_length)
//	{
//		//	delay_us(1);
//		//tx_len=rf_pack_bytes;
//		tx_len=6;
//
//		//rf_tx_cnt=rf_tx_cnt+tx_len;
//		fifo_cfg(0);
//		fifo_reset();
//		spi_write_fifo();
//		enable_tx_interrupt();
//		clr_interrupt();
//		tx_start();
//		//if(uart_rx_len!=rf_tx_cnt)
//		delay_us(15000);
//		__HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);    //??????
//		rx_init();
//		fifo_reset();
//		first_tx=0;
//		rf_tx_cnt=0;
//		uart_rx_len=0;
//		tx_rx_switch=0;
//		state_idle=0;
//		__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);    //??????
//	}
////	else
////	{

////		tx_len=payload_length;
////		rf_tx_cnt=rf_tx_cnt+tx_len;

////		fifo_cfg(1);
////		fifo_reset();
////		spi_write_fifo();
////		enable_tx_interrupt();
////		clr_interrupt();
////		tx_start();
////		//if(uart_rx_len!=rf_tx_cnt)
////		delay_us(15000);
////	}
////	real_tx_len=	tx_len+		real_tx_len;
////	if(uart_rx_len==rf_tx_cnt)
////	{
////		__HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);    //??????
////		rx_init();
////		fifo_reset();
////		first_tx=0;
////		rf_tx_cnt=0;
////		uart_rx_len=0;
////		tx_rx_switch=0;
////		state_idle=0;
////		__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);    //??????
////	}
//}



//unsigned char spi_byte(unsigned char data)
//{
//	unsigned char data2;
//	HAL_SPI_TransmitReceive(&hspi1, &data, &data2, 1, 2);
//	return (data2);
//}

//unsigned char spi_readbyte(void)
//{
//	unsigned char data;
//	  data=0;
//		HAL_SPI_Receive(&hspi1, &data, 1, 10);

//	return (data);
//}


//void spi_write(unsigned char tx_length, unsigned char *p)
//{
//	unsigned char i,j;
//		i = 0;
//		while(i!=0xff)
//		{
//			i = check_cts();
//			if(check_cnt<200)
//				check_cnt =check_cnt + 1;
//		}
//    check_cnt=0;
//		nCS_RESET();
//		delay_us(1);
//		for (i = 0; i < tx_length; i++)
//		{
//			j = *(p+i);
//			spi_byte(j);
//
//		}
//		nCS_SET() ;
//		delay_us(1);
//}

//U8 check_cts(void)
//{
//	U8 i;
//	nCS_SET() ;
//	delay_us(1);
//	nCS_RESET();
//	delay_us(1);
//	spi_byte(0x44);
//	i = spi_byte(0);
//	nCS_SET() ;
//	delay_us(1);
//	return (i);
//}

//void spi_read(U8 data_length, U8 api_command )
//{
//	U8 i;
//
//	U8 p[1];
//	p[0] = api_command;
//	i = 0;
//	while(i!=0xff)
//		{
//			i = check_cts();
//			if(check_cnt<200)
//				check_cnt =check_cnt + 1;
//		}
//    check_cnt=0;

//	spi_write(1, p);

//	i = 0;
//	while(i!=0xff)
//	i = check_cts();

//	nCS_SET() ;
//	delay_us(1);
//	nCS_RESET();
//	delay_us(1);
//	spi_byte(0x44);
//	for (i = 0; i< data_length; i++)
//		spi_read_buf[i] = spi_byte(0xff);
//	//delay_us(1);
//	nCS_SET() ;
//	delay_us(1);
//}

//void spi_write_fifo(void)
//{
//	U8 i;
//
//	i = 0;
//	while(i!=0xff)
//		{
//			i = check_cts();
//			if(check_cnt<200)
//				check_cnt =check_cnt + 1;
//		}
//    check_cnt=0;
//	nCS_SET() ;
//	delay_us(1);
//	nCS_RESET();
//	delay_us(1);
//
//	spi_byte(WRITE_TX_FIFO);
//	// header match tx
//	spi_byte(rf_id[0]);
//	spi_byte(rf_id[1]);
//	spi_byte(rf_id[2]);
//	spi_byte(rf_id[3]);
//	spi_byte(tx_len); //payload length header and length byte not include
//
//	for (i = 0; i< tx_len; i++)
//	{
//		spi_byte(tx_ph_data[rf_tx_index++]);
//		if(rf_tx_index>=buffer_size)
//			rf_tx_index=0;
//	}
//	nCS_SET();
//	delay_us(1);
//}

uint32_t uart_tx_DMA()
{
	if(rf_rx_index!=uart_tx_index&&tmp2!=rf_rx_index)
	{
		tmp2=rf_rx_index;
		tmp1=rf_rx_cnt;
		if((uart_tx_index+tmp1)<buffer_size)
		{
			HAL_UART_Transmit_DMA(&huart1,&rx_ph_data[uart_tx_index],tmp1);
			//uart_real_tx=uart_real_tx+tmp1;
			uart_tx_index=uart_tx_index+tmp1;
			rf_rx_cnt=rf_rx_cnt-tmp1;
		}
		else
		{
			tmp2=buffer_size-uart_tx_index;
			HAL_UART_Transmit_DMA(&huart1,&rx_ph_data[uart_tx_index],tmp2);
			// uart_real_tx=uart_real_tx+tmp2;
			uart_tx_index=0;

			if(tmp1!=rf_rx_cnt)
			{
				rf_rx_cnt =rf_rx_cnt-tmp2;
			}
			else
			{
				rf_rx_cnt =rf_rx_cnt-tmp2;
			}
		}
		uart_dma_state=0;
	}
	return tmp1;
}
uint32_t rf_real_rx;
/*void spi_read_fifo(void)
{
	U8 i;
	i = 0;
	while(i!=0xff)
	{
		i = check_cts();
		if(check_cnt<200)
			check_cnt =check_cnt + 1;
	}
	check_cnt=0;

	nCS_SET() ;
	//delay_us(1);
	nCS_RESET();

	spi_byte(READ_RX_FIFO);

	spi_readbyte();//读s
	spi_readbyte();//读W
	spi_readbyte();//读W
	spi_readbyte();// 读X

	rx_len = 	spi_readbyte();  //读payload长度
	rf_rx_cnt=rf_rx_cnt+rx_len;
	rf_real_rx=rf_real_rx+rx_len;
	rf_rx_base_addr=rf_rx_index;

	for (i = 0; i< rx_len; i++)
	{
		rx_ph_data[rf_rx_index++] = spi_readbyte(); //读payload数据
		if(rf_rx_index>=buffer_size)
			rf_rx_index=0;
	}
	nCS_SET() ;
	rf_Rev(6);
}

void sdn_reset(void)
{
	U8 i;

	RFM95_PORchkout_SET() ;
	delay_us(5000);
	RFM95_PORchkout_RESET();
	delay_us(5000);
	SCK_RESET();
	delay_us(1);
	nCS_SET() ;
	delay_us(1);
	nCS_RESET();	//delay_us(2);

	for (i = 0; i< 7; i++)
		spi_byte(RF_POWER_UP_data[i]);
	delay_us(5);
	nCS_SET() ;
	delay_us(200);
}


void spi_read_info(void)
{
	spi_read(9,PART_INFO);
}

//void spi_read_byte(U8 data_length,U8 *p)
//{
//	U8 i;
//	nCS_SET() ;
//	SCK_RESET();
//	nCS_RESET();
//
//	spi_byte(0x44);
//	for (i = 0; i< data_length; i++)
//		p[i] = spi_byte(0xff);
//	nCS_SET() ;
//
//}

//void SI446X_INT_STATUS(U8 *p )
//{
//    u8 cmd[4];
//    cmd[0] = GET_INT_STATUS;
//    cmd[1] = 0;
//    cmd[2] = 0;
//    cmd[3] = 0;
//    spi_write(4,cmd);
//	  spi_read_byte(9,p);
//    delay_us(10);
//}

/******************************************************************************/
/*void ack_sent()
{
	unsigned short sum_crc=0;
	unsigned char i;
	unsigned char ack_buf[6];
	tx_ph_data[0]=0x5a;
	tx_ph_data[1]=2;//负载长度
	tx_ph_data[2]=0xa6;
	tx_ph_data[3]=00;//0x00:无动作  0x11:上锁   0x22:开锁


	tx_ph_data[4]=MAVLINK_MSG_ID_HIL_DGPS_CRC;
	crc_init(&sum_crc);
	for(i=1; i<5; i++)
	{
		crc_accumulate((ack_buf+i),&sum_crc);//CRC校验
	}
	tx_ph_data[4]=sum_crc&0xff;
	tx_ph_data[5]=sum_crc>>8;
}
*/
/*void rf_Rev(unsigned int len)
{
	unsigned short sum_crc=0;
	unsigned int i,j;
	unsigned char k;
	for(i=0; i<6; i++)
	{
		if(rx_ph_data[0]==0x5a)
		{
			crc_init(&sum_crc);
			k=rx_ph_data[i+1];
			for(j=0; j<3; j++)
			{
				crc_accumulate(&(rx_ph_data[j+1]),&sum_crc);//CRC校验
			}

			k=MAVLINK_MSG_ID_HIL_DGPS_CRC;//crc code
			crc_accumulate(&k,&sum_crc);
			if(((rx_ph_data[4])==(sum_crc&0xff))&&(rx_ph_data[5]==(sum_crc>>8)))
			{
				ack_sent();
				tx_rx_switch=1;
				if(rx_ph_data[3]==0x22)
				{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);	//关锁
				}
				if(rx_ph_data[3]==0x11)
				{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);	//开锁
				}
			}
			for(j=0; j<6; j++)
			{
				rx_ph_data[j]=0;
			}
		}
	}
}*/



