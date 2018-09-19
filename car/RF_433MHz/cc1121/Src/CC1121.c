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

#include "CRC.h"
#include "CC1121_gpio.h"
#include "CC1121.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx.h"
#include "flash.h"
#include "RF_parameter_setting.h"
#include "string.h"
#include "systick_timer.h"
#include "stm32f0xx_hal.h"
#include "hac_crc16l.h"
/* USER CODE BEGIN 0 */
extern UART_HandleTypeDef huart1;
extern IWDG_HandleTypeDef hiwdg;
extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

extern unsigned char uart_dma_state;
CC1121_RF_RX_DATA RF_RECEIVE_DATA;
DATA_PACKET_DEF  Data_Packet;
CC1121_RF_txDATA_DEF Data_Packet_Tx;
static const CC1121_REG_Def RfCC1120_TABLE_less2E[]=
{
	CC112X_IOCFG3,0x06,
	CC112X_IOCFG2,0x06,
	CC112X_IOCFG1,0xB0,
	CC112X_IOCFG0,0x06,
	CC112X_SYNC3,BARKER_CONST1,
	CC112X_SYNC2,BARKER_CONST2,
	CC112X_SYNC1,BARKER_CONST3,
	CC112X_SYNC0,BARKER_CONST4,
	CC112X_SYNC_CFG1,0x08,
	CC112X_SYNC_CFG0,0x17,
	CC112X_DEVIATION_M,0xEB,
	CC112X_MODCFG_DEV_E,0x0C,
	CC112X_DCFILT_CFG,0x16,
	CC112X_PREAMBLE_CFG1,0x2C,
	CC112X_PREAMBLE_CFG0,0x0A,
	CC112X_FREQ_IF_CFG,0x40,
	CC112X_IQIC,0x00,
	CC112X_CHAN_BW,0x03,
	CC112X_MDMCFG1,0x46,
	CC112X_MDMCFG0,0x05,
	CC112X_DRATE2,0x83,
	CC112X_AGC_REF,0x36,
	CC112X_AGC_CS_THR,0xEF,
	CC112X_AGC_CFG1,0xA9,
	CC112X_AGC_CFG0,0xC0,
	CC112X_SETTLING_CFG,0x03,
	CC112X_FS_CFG,0x14,
	CC112X_PKT_CFG2,0x00,
	CC112X_PKT_CFG1,0x00,//40,
	CC112X_PKT_CFG0,0x00,
	CC112X_RFEND_CFG1,0x3F,
	CC112X_RFEND_CFG0,0x00,
	CC112X_PA_CFG2,0x7F,
	CC112X_PA_CFG0,0x04,
	CC112X_PKT_LEN,PKT_LEN
};
static const CC1121_REG_Def RfCC1120_TABLE_more2E[]=
{
	CC112X_IF_MIX_CFG,0x00,//
	CC112X_FREQOFF_CFG,0x20,//
	CC112X_TOC_CFG,0x0A,
	CC112X_FS_DIG1,0x00,
	CC112X_FS_DIG0,0x5F,
	CC112X_FS_CAL1,0x40,
	CC112X_FS_CAL0,0x0E,
	CC112X_FS_DIVTWO,0x03,
	CC112X_FS_DSM0,0x33,
	CC112X_FS_DVC0,0x17,
	CC112X_FS_PFD,0x50,
	CC112X_FS_PRE,0x6E,
	CC112X_FS_REG_DIV_CML,0x14,
	CC112X_FS_SPARE,0xAC,
	CC112X_FS_VCO0,0xB4,
	CC112X_XOSC5,0x0E,
	CC112X_XOSC1,0x03,
	CC112X_ECG_CFG,0x00

//     CC112X_SERIAL_STATUS,0x08//TX// 0x00//Rx//
};

unsigned char PKT_RSSI;

unsigned char spi_byte(unsigned char data)
{
	unsigned char data2;
	HAL_SPI_TransmitReceive(&hspi1, &data, &data2, 1, 2);
	return (data2);
}

unsigned char spi_readbyte(void)
{
	unsigned char data;
	data=0;
	HAL_SPI_Receive(&hspi1, &data, 1, 10);

	return (data);
}


void uRF_rate_SETTING(unsigned char *p)
{
//	rf_air_rate=(((unsigned int)p[0])<<8)+p[1];
}

//set rf id
void uID_SETTING(unsigned char *p)
{
//	rf_id[3]=(unsigned char)p[1];
//	rf_id[2]=(unsigned char)p[0];
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
//	switch(p[0])
//	{
//	case 5:
//		POWER=31;
//		break;
//	case 10:
//		POWER=63;
//		break;
//	case 15:
//		POWER=95;
//		break;
//	case 20:
//		POWER=127;
//		break;
//	default:
//		POWER=80;
//		break;

	//}

}

//channel set 0-255
void uCH_SETTING(unsigned char *p)
{
	//freq_channel=p[0];
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
//	p[7]=(unsigned char)(uBrate>>8);
//	p[8]=(unsigned char)(uBrate);


////air burate
//	p[9]=(unsigned char)(rf_air_rate>>8);
//	p[10]=(unsigned char)(rf_air_rate);

////rf id
//	p[11]=rf_id[2];
//	p[12]=rf_id[3];

//	//rf pa
//	switch(POWER)
//	{
//	case 31:
//		p[13]=5;
//		break;
//	case 63:
//		p[13]=10;
//		break;
//	case 95:
//		p[13]=15;
//		break;
//	case 127:
//		p[13]=20;
//		break;
//	default:
//		p[13]=15;
//		break;

//	}



//	//rf channel
//	p[14]=freq_channel;


////	p[15]=version[1];
////	p[16]=version[3];


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



//rf parameters save in flash
void RF_parameters_save()
{
//	uint32_t addr;
//	addr=2;

//	Lock_Code=	rev_ID_read();
//	flash_init();
//#if down_load_id
//	rev_ID_write(Lock_Code1);
//#else
//	rev_ID_write(Lock_Code);
//#endif


//	flash_write(1,0x5a,1);
//	flash_write(addr++,uBrate,1);	//uart baudrate set

//	flash_write(addr++,freq_channel,1);	//rf channel

//	flash_write(addr++,POWER,1);	//rf tx power

//	flash_write(addr++,rf_air_rate,1);	//rf center freq,3bytes
//	flash_write(addr++,RF_FREQ_CONTROL_INTE_8_data[6],1);
//	flash_write(addr++,RF_FREQ_CONTROL_INTE_8_data[7],1);

//	flash_write(addr++,rf_id[2],1);	//rf id,2bytes
//	flash_write(addr++,rf_id[3],1);
//}
//void default_rf_P(void)
//{
//	uBrate= 9600;
//	freq_channel=0;
//	POWER=127;
//	rf_air_rate=1200;
//	rf_id[0]='s';
//	rf_id[1]='w';
//	rf_id[2]='w';
//	rf_id[3]='x';

}
//rf parameters read from mcu flash
void RF_parameters_read()
{
//	uint32_t addr;
//	uint32_t tmp;
//err:
//	addr=2;
//	Lock_Code=	rev_ID_read();
//	GetLockCode();
//	if(Lock_Code != Lock_Code1)
//	{
//		goto err;
//	} // ??
//	flash_read(addr++,&tmp,1);
//	uBrate = (unsigned int)tmp;

//	flash_read(addr++,&tmp,1);
//	freq_channel=(U8)tmp;

//	flash_read(addr++,&tmp,1);
//	POWER=(U8)tmp;

//	flash_read(addr++,&tmp,1);
//	rf_air_rate=(unsigned int)tmp;

//	flash_read(addr++,&tmp,1);
//	flash_read(addr++,&tmp,1);


//	flash_read(addr++,&tmp,1);
//	rf_id[2]=(U8)tmp;

//	flash_read(addr++,&tmp,1);
//	rf_id[3]=(U8)tmp;

//	if(uBrate<=0||uBrate>19200||POWER<=0||POWER>127||rf_air_rate<1200||rf_air_rate>19200)
//	{
//		default_rf_P();
//	}

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
void uint8_copy(unsigned char *dst,unsigned char *src,unsigned int len)
{
	unsigned int i;
	for(i=0; i<len; i++)
	{
		*(dst++)=*(src++);
	}
}


/*!
*******************************************************************************
@brief  CC1121  Write register
@param  unsigned char  addr   register address
@param  unsigned char  value   register value
@return void
*******************************************************************************
*/
void CC1121_WriteReg(unsigned char value,unsigned char addr)
{

	nCS_RESET();
	delay_us(1);
	spi_byte(addr);
	spi_byte(value);
	nCS_SET() ;
	delay_us(1);
}
void  CC112X_WriteReg(unsigned char addr,unsigned char value)
{
	CC1121_WriteReg(value,addr);
}


/*!
*******************************************************************************
@brief  CC1121  Write Extend_register
@param  unsigned char  addr   register address
@param  unsigned char  value   register value
@return void
*******************************************************************************
*/
void CC1121_WriteExtend_reg(unsigned char value,unsigned char addr)
{

	nCS_RESET();
	delay_us(1);
	spi_byte(CC1121_WriteExtend_Addr);
	spi_byte(addr);
	spi_byte(value);
	nCS_SET() ;
	delay_us(1);
}
/*!
*******************************************************************************
@brief  CC1121  Write cmd
@param  unsigned char  cmd   cmd value
@return void
*******************************************************************************
*/
unsigned char CC1121_WriteCmd(unsigned char cmd )
{
	unsigned char st;
	nCS_RESET();
	delay_us(1);
	st=spi_byte(cmd);
	nCS_SET() ;
	delay_us(1);
	return st;
}


/*!
*******************************************************************************
@brief  CC1121  CC1121_SETTING_TAB_SetUp
@param  CC1121_REG_Def  Table   register list
@param  bool   flase£ºnot extend register true:is  register
@return void
*******************************************************************************
*/

void CC1121_SETTING_TAB_SetUp(const CC1121_REG_Def *Table,unsigned char len,unsigned char extend_falg)
{
	unsigned char       i;
	CC1121_REG_Def register_def;

	for(i=0; i<len; i++)
	{
		register_def=Table[i];
		if(extend_falg==true)
		{
			CC1121_WriteExtend_reg(register_def.reg_value,register_def.reg_addr);
		}
		else
		{
			CC1121_WriteReg(register_def.reg_value,register_def.reg_addr);
		}
	}
}
/*!
*******************************************************************************
@brief  CC1121_ReadReg
@param  unsigned char  addr
@param  bool  extend_falg
@return unsigned char
*******************************************************************************
*/
unsigned char CC1121_ReadReg(unsigned char addr,unsigned char  extend_falg)
{
	unsigned char value;

	nCS_RESET();
	delay_us(1);

	if(extend_falg==true)
	{

		spi_byte(CC1121_ReadExtend_Addr);
		spi_byte(addr);
		value=spi_byte(0xff);
	}
	else
	{
		spi_byte(addr|0x80);
		value=spi_byte(0xff);
	}

	nCS_SET() ;
	delay_us(1);


	return value;
}
void  Reset_CC1121(void)
{
	RFM95_PORchkout_RESET();
	delay_ms(50);
	RFM95_PORchkout_SET() ;
	delay_ms(50);
}
unsigned char value;
void  RF_CC1121_Init(void)
{
	unsigned char len;


	Reset_CC1121();
	RF_Part_Number();
	CC1121_WriteCmd(CC112x_SRES);
	delay_ms(1);
	CC1121_WriteCmd(CC112x_SIDLE);
	delay_ms(1);
	CC1121_WriteCmd(CC112x_SNOP);

	len=(sizeof(RfCC1120_TABLE_less2E)/sizeof(RfCC1120_TABLE_less2E[0]));
	CC1121_SETTING_TAB_SetUp(RfCC1120_TABLE_less2E,len,false);

	len=(sizeof(RfCC1120_TABLE_more2E)/sizeof(RfCC1120_TABLE_more2E[0]));
	CC1121_SETTING_TAB_SetUp(RfCC1120_TABLE_more2E,len,true);

	CC1121_WriteExtend_reg(CC112X_FREQ0_init,CC112X_FREQ0);
	CC1121_WriteExtend_reg(CC112X_FREQ1_init,CC112X_FREQ1);
	CC1121_WriteExtend_reg(CC112X_FREQ2_init,CC112X_FREQ2);

	RF_CC1121_Calibration();
	/*value=0;
	value=CC1121_ReadReg(CC112X_FREQ2,true);
	value=0;
	value=CC1121_ReadReg(CC112X_FS_DIG0,true);
	value=0;
	value=CC1121_ReadReg(CC112X_FS_SPARE,true);
	value=0;
	value=CC1121_ReadReg(CC112X_IOCFG1,false);
	value=0;
	value=CC1121_ReadReg(CC112X_DRATE2,false);
	value=0;
	value=CC1121_ReadReg(CC112X_RFEND_CFG1,false);*/
	RF_CC1121_Set_RX_Module(); //Set to RX module


	RF_Part_Number();
}
void  RF_CC1121_Calibration(void)
{
	unsigned char read_data;
	unsigned char barker1,barker2,barker3,barker4;

	CC1121_WriteCmd(CC112x_SIDLE);
	delay_ms(2);
	CC1121_WriteCmd(CC112x_SFTX);
	CC1121_WriteCmd(CC112x_SFRX);

	read_data=CC1121_ReadReg(CC112X_PARTVERSION,true);
	//asm("nop");
	if(read_data==PARTVERSION)
	{
		CC1121_WriteExtend_reg(0,CC112X_FS_VCO2);
		read_data=CC1121_ReadReg(CC112X_FS_CAL2,true);
		barker4=read_data;
		read_data+=2;
		CC1121_WriteExtend_reg(read_data,CC112X_FS_CAL2);

		CC1121_WriteCmd(CC112x_SCAL);
		delay_ms(1);
		read_data=CC1121_ReadReg(CC112X_FS_VCO2,true);
		barker1=read_data;

		read_data=CC1121_ReadReg(CC112X_FS_VCO4,true);
		barker2=read_data;

		read_data=CC1121_ReadReg(CC112X_FS_CHP,true);
		barker3=read_data;

		CC1121_WriteExtend_reg(0,CC112X_FS_VCO2);

		CC1121_WriteExtend_reg(barker4,CC112X_FS_CAL2);

		CC1121_WriteCmd(CC112x_SCAL);
		delay_ms(1);
		read_data=CC1121_ReadReg(CC112X_FS_VCO2,true);
		if(read_data>=barker1)
		{
			return;
		}
		CC1121_WriteExtend_reg(barker1,CC112X_FS_VCO2);
		CC1121_WriteExtend_reg(barker2,CC112X_FS_VCO4);
		CC1121_WriteExtend_reg(barker3,CC112X_FS_CHP);
	}
	else
	{
		CC1121_WriteCmd(CC112x_SCAL);
		delay_ms(1);
	}
}
unsigned char RF_CC1121_ReadRSSI(void)
{
	unsigned char read_data;

	read_data=CC1121_ReadReg(CC112X_RSSI1,true);
	read_data-=102;
	return read_data;
}



void RF_CC1121_Set_RX_Module(void)
{
	do
	{
		CC1121_WriteCmd(CC112x_SRX);
		delay_ms(1);
	}
	while(!(RF_CC1121_Get_STATUS()&0x10));

}

void RF_CC1121_Set_Idle_Module(void)
{
	do
	{
		CC1121_WriteCmd(CC112x_SIDLE);
		delay_ms(1);
	}
	while((RF_CC1121_Get_STATUS()&0x70));

}

unsigned char  RF_CC1121_Get_STATUS(void)
{
	unsigned char st=0;;
	st= CC1121_WriteCmd(CC112x_SNOP);
	//("nop");
	return st;
}
unsigned char count;
void data_transmit()
{
    unsigned short crc_sum;
	//if(uart_dma_state==1)
	{
		uart_dma_state=0;
		Data_Packet.pkt_def.pkt_crc=MSG_CRC;
		crc_sum=crc_calculate((unsigned char *)&Data_Packet.pkt_def, 13); 
		Data_Packet.pkt_def.pkt_crc=crc_sum;
count++;
		HAL_UART_Transmit(&huart1,(uint8_t *)&Data_Packet.pkt_def,14,10);
	}

}	Module_Sample_Time sample_time_status;

unsigned char rf_start_flag;
void  RF_CC1121_Get_Data(void)
{

	unsigned char read_data,i;
	unsigned int crc16_res;
  uint32_t pkt_id_tmp;
	RF_RECEIVE_DATA.rf_receive_data_len=0;
	if(RF_RECEIVE_DATA.rf_receive_end_flag==true)
	{
		rf_start_flag=1;
		
		
		
		RF_RECEIVE_DATA.rf_receive_end_flag=false;
		memset(RF_RECEIVE_DATA.rf_receive_data_buff,0,sizeof(RF_RECEIVE_DATA.rf_receive_data_buff));
		for(i=0; i<PKT_LEN; i++)
		{
			read_data=CC1121_ReadReg(CC112x_FIFO,false);
			RF_RECEIVE_DATA.rf_receive_data_buff[i]=read_data;
		}
		RF_RECEIVE_DATA.rf_receive_data_len=PKT_LEN;
		crc16_res=crc16l_check(RF_RECEIVE_DATA.rf_receive_data_buff,RF_RECEIVE_DATA.rf_receive_data_len);
		if(crc16_res==0)
		{

				pkt_id_tmp=Data_Packet.pkt_def.pkt_id;
				uint8_copy(Data_Packet.pkt_data_buff,RF_RECEIVE_DATA.rf_receive_data_buff,RF_RECEIVE_DATA.rf_receive_data_len);

			//if(Data_Packet.pkt_def.pkt_id!=pkt_id_tmp||sample_time_status.RF_delay_flag==0x01)
			{
//				if(sample_time_status.RF_delay_flag==0x01)
//				{
//					Sample_Timer_Init();
//					rf_start_flag=0;
//					Reset_Sample_flag(0);				
//				}        
				//pkt_id_tmp=Data_Packet.pkt_def.pkt_id;
				uint8_copy(Data_Packet.pkt_data_buff,RF_RECEIVE_DATA.rf_receive_data_buff,RF_RECEIVE_DATA.rf_receive_data_len);
				if(Data_Packet.pkt_def.pkt_tmp_integer>0x80)   // is negative number
				{
					Data_Packet.pkt_def.pkt_tmp_integer=(Data_Packet.pkt_def.pkt_tmp_integer&0x7f);
					Data_Packet.pkt_def.pkt_tmp_integer=((~Data_Packet.pkt_def.pkt_tmp_integer)+1);
				}
				Data_Packet.pkt_def.pkt_rssi=RF_CC1121_ReadRSSI();
				data_transmit();
				//("nop");			
			}
		}
		else
		{
			RF_RECEIVE_DATA.rf_receive_data_len=0;
		}

	}



}
unsigned char RF_Part_Number()
{
	unsigned char read_data;
	read_data=0;
	read_data=CC1121_ReadReg(CC112X_PARTNUMBER,true);
	return read_data;
}


void rx_init(void)
{

	RF_CC1121_Init();

}



unsigned char txrx_flag;

void  RF_SI443X_Tx_Data(void)
{
	Module_Sample_Time sample_time_flag;


	if(txrx_flag==0)
	{
		if(Data_Packet_Tx.rf_tx_start_flag==1&&RF_RECEIVE_DATA.rf_receive_end_flag==false)
		{
//			txrx_flag=1;
//			tx_len=Data_Packet_Tx.rf_tx_data_len;
//			fifo_cfg(0);
//			fifo_reset();
//			spi_write_fifo();
//			enable_tx_interrupt();
//			clr_interrupt();
//			tx_start();
//			Reset_Sample_flag(1);
//			Data_Packet_Tx.rf_tx_start_flag=0;
//			//delay_us(15000);

		}
	}
	else
	{
		sample_time_flag=Get_Sample_flag_Status();
		if(sample_time_flag.TXRX_sample_flag==1)
		{
			txrx_flag=0;
		}
	}

}




void RF_Process()
{
//  if(rf_start_flag==1)
//	{
//	Sample_Timer_Init();
//	Sample_Timer_Reload();
//	}	
//	sample_time_status=Get_Sample_flag_Status(); 
//	if(sample_time_status.RF_delay_flag==0x01)
//	{
//		rf_start_flag=0;
//		//Sample_Timer_Init();
//		Sample_Timer_Reload();
//		//Reset_Sample_flag(0);				
//	}	
	RF_CC1121_Get_Data();

	
	//RF_SI443X_Tx_Data();
}



void PROC_MANUFACTURE_Do(void)
{


	RF_CC1121_Init();
	RF_CC1121_Set_Idle_Module();

	CC112X_WriteReg(CC112X_SYNC3,0xAA);
	CC112X_WriteReg(CC112X_SYNC2,0xAA);
	CC112X_WriteReg(CC112X_SYNC1,0xAA);
	CC112X_WriteReg(CC112X_SYNC0,0xAA);
	CC112X_WriteReg(CC112X_SYNC_CFG1,0x1F);
	CC112X_WriteReg(CC112X_AGC_CFG1,0x0A);
	CC112X_WriteReg(CC112X_MDMCFG1,0x06);
	CC112X_WriteReg(CC112X_PKT_CFG2,0x05);
	CC112X_WriteReg(CC112X_PKT_LEN,0x3F);

	CC112X_WriteReg(CC112X_IOCFG0,0x09);    //???????


	//RF_CC1121_Calibration();
	RF_CC1121_Set_RX_Module();

	while(1);

}
