#include "xz_cc1121_init.h"
#include "hac_crc16l.h"
#include "systick_timer.h"

CC1121_RF_RX_DATA RF_RECEIVE_DATA;
DATA_PACKET_DEF  Data_Packet;

static const CC1121_REG_Def RfCC1120_TABLE_less2E[]={
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

uint8 PKT_RSSI;

/*!
*******************************************************************************
@brief  CC1121  Write register
@param  uint8  addr   register address
@param  uint8  value   register value
@return void          
*******************************************************************************
*/
void CC1121_WriteReg(uint8 value ,uint8 addr)
{
  CC1121_SPI_CS_Begin();
  
  CC1121_SPI_3W_WriteUint8(addr);   
  CC1121_SPI_3W_WriteUint8(value);  
  
  CC1121_SPI_CS_End();
}

/*!
*******************************************************************************
@brief  CC1121  Write Extend_register
@param  uint8  addr   register address
@param  uint8  value   register value
@return void          
*******************************************************************************
*/
void CC1121_WriteExtend_reg(uint8 value ,uint8 addr)
{
  CC1121_SPI_CS_Begin();
  
  CC1121_SPI_3W_WriteUint8(CC1121_WriteExtend_Addr); 
  CC1121_SPI_3W_WriteUint8(addr);   
  CC1121_SPI_3W_WriteUint8(value);  
  
  CC1121_SPI_CS_End();
}

/*!
*******************************************************************************
@brief  CC1121  Write cmd
@param  uint8  cmd   cmd value
@return void          
*******************************************************************************
*/
uint8 CC1121_WriteCmd(uint8 cmd )
{
  uint8 st;
  CC1121_SPI_CS_Begin();
  
  st=CC1121_SPI_3W_WriteUint8(cmd);  
  
  CC1121_SPI_CS_End();
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
 
void CC1121_SETTING_TAB_SetUp(const CC1121_REG_Def *Table,uint8 len,bool extend_falg)
{ 
  uint8       i;
  CC1121_REG_Def register_def;
  
  for(i=0;i<len;i++)
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
@param  uint8  addr   
@param  bool  extend_falg
@return uint8         
*******************************************************************************
*/
uint8 CC1121_ReadReg(uint8 addr,bool extend_falg)
{
  uint8 value;    
  
  CC1121_SPI_CS_Begin();
  if(extend_falg==true)
  {
    CC1121_SPI_3W_WriteUint8(CC1121_ReadExtend_Addr); 
    CC1121_SPI_3W_WriteUint8(addr);   
    value =CC1121_SPI_3W_ReadUint8(); 
    
  }
  else
  {
    CC1121_SPI_3W_WriteUint8(addr|0x80);
    value =CC1121_SPI_3W_ReadUint8(); 
  } 
  
  CC1121_SPI_CS_End();
  
  return value;
}

void  Reset_CC1121(void)
{
  
  CC1121_RST_OUT_LOW();
  delay_ms(5);
  CC1121_RST_OUT_HIGH();
  delay_ms(5);
}

void  RF_CC1121_Init(void)
{  
  uint8 len;

  Reset_CC1121();
  
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
  
  RF_CC1121_Set_RX_Module(); //Set to RX module
}


void  RF_CC1121_Calibration(void)
{
  uint8 read_data;
  uint8 barker1,barker2,barker3,barker4;

  CC1121_WriteCmd(CC112x_SIDLE);   
  delay_ms(2);
  CC1121_WriteCmd(CC112x_SFTX); 
  CC1121_WriteCmd(CC112x_SFRX); 
  
  read_data=CC1121_ReadReg(CC112X_PARTVERSION,true);
//  asm("nop");
  if(read_data==0x21)
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

uint8 RF_CC1121_ReadRSSI(void)
{
  uint8 read_data;  

  read_data=CC1121_ReadReg(CC112X_RSSI1,true);  
  read_data-=102;
  return read_data;
}



void RF_CC1121_Set_RX_Module(void)
{
  do{
   CC1121_WriteCmd(CC112x_SRX); 
   delay_ms(1);
  }while(!(RF_CC1121_Get_STATUS()&0x10));
  
}

void uint8_copy(uint8 *dst,uint8 *src,uint16 len)
{
  uint16 i;
  for(i=0;i<len;i++)
  {
    *(dst++)=*(src++);
  }
}


uint8  RF_CC1121_Get_STATUS(void)
{
  uint8 st=0;;
  st= CC1121_WriteCmd(CC112x_SNOP);
//  asm("nop"); 
  return st;
}

void  RF_CC1121_Get_Data(void)
{
  uint8 read_data,i;
  uint16 crc16_res;
  
  RF_RECEIVE_DATA.rf_receive_data_len=0;
  if(RF_RECEIVE_DATA.rf_receive_end_flag==true)
  {
    RF_RECEIVE_DATA.rf_receive_end_flag=false;
    memset(RF_RECEIVE_DATA.rf_receive_data_buff,0,sizeof(RF_RECEIVE_DATA.rf_receive_data_buff));
    for(i=0;i<PKT_LEN;i++)
    {
     read_data=CC1121_ReadReg(CC112x_FIFO,false);
     RF_RECEIVE_DATA.rf_receive_data_buff[i]=read_data;    
    } 
    RF_RECEIVE_DATA.rf_receive_data_len=PKT_LEN;   
    crc16_res=crc16l_check(RF_RECEIVE_DATA.rf_receive_data_buff,RF_RECEIVE_DATA.rf_receive_data_len);
    if(crc16_res==0)
    {      
      uint8_copy(Data_Packet.pkt_data_buff,RF_RECEIVE_DATA.rf_receive_data_buff,RF_RECEIVE_DATA.rf_receive_data_len);
      if(Data_Packet.pkt_def.pkt_tmp_integer>0x80)   // is negative number
      {
        Data_Packet.pkt_def.pkt_tmp_integer=(Data_Packet.pkt_def.pkt_tmp_integer&0x7f);
        Data_Packet.pkt_def.pkt_tmp_integer=((~Data_Packet.pkt_def.pkt_tmp_integer)+1);
      }
      Data_Packet.pkt_def.pkt_rssi=RF_CC1121_ReadRSSI();
//      asm("nop");
    }
    else
    {
      RF_RECEIVE_DATA.rf_receive_data_len=0;
    }
   
  }
  
  
  
}







