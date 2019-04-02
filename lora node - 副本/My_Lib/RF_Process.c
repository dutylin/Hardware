#include "stm8l15x.h"
#include "stm8_eval.h"
#include "B_SHT30A.h"
#include "RF_Process.h"
#include "lora.h"
#include "uart1.h"
#include "Delay.h"
#include "CRC.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"
#include "PUMP_GPIO.h"
void payload_pack(void);
void Sensor_Process(void);
void payload_pack_test(void);


USER_DATA_DEF server_cmd,dev_data;
extern uint16_t Buffer[1];
extern u8  USART1_TX_BUF[USART1_MAX_SEND_LEN]; 
extern uint32_t PotVoltage;
uint8 m_TemSymbol = 0;
uint16 m_Tem = 0;
uint16 m_Hum = 0;
uint8_t pbuff[4];
uint16 payload_len;
unsigned short  crc_sum;
unsigned int sum_crc1;
unsigned char crc_sum1;
u32 dev_sn=0x12345678;
void payload_pack_test();

void payload_pack()
{
  uint8_t index_tmp;
  unsigned char tmp;  
  dev_data.len=0;

//head
  dev_data.head=DEV_HEAD;
  dev_data.len=dev_data.len+2;
  //crc len
  dev_data.len=dev_data.len+2;
  //data package len
  dev_data.len=dev_data.len+2;  
//dev type
  dev_data.dev_type=DEV_NODE;
  dev_data.len=dev_data.len+1;     
//sn;
dev_data.sn=dev_sn;
dev_data.len=dev_data.len+4;

//cmd cmd_num
dev_data.cmd_type=CMD_NO;
dev_data.cmd_num=CMD_NO;
dev_data.len=dev_data.len+2;


  //temperatur and humy
  index_tmp=0;
 /* pbuff[0]=0x01;
  pbuff[1]=0x0E;
  pbuff[2]=0x02;
  pbuff[3]=0x2B;*/



  
  memcpy(&dev_data.payload[index_tmp],pbuff,sizeof(pbuff));
  index_tmp=index_tmp+4;
  dev_data.len=dev_data.len+4;

//battery voltage
  //PotVoltage=0x0008;

  PotVoltage=(short unsigned int )PotVoltage;
  dev_data.payload[index_tmp++]=(unsigned char )(PotVoltage/256);
  dev_data.payload[index_tmp++]=(unsigned char )(PotVoltage%256);  
   //memcpy(&USART1_TX_BUF[index_tmp],&PotVoltage,2);
  dev_data.len=dev_data.len+2;

//crc code
  dev_data.payload[index_tmp++]=MSG_CRC;
  dev_data.len=dev_data.len+1;
  
  //payload len
 // dev_data.len=dev_data.len-HEAD_CRC_LEN;
  index_tmp=0;

  memcpy(& USART1_TX_BUF[index_tmp],&dev_data.head,2);
  index_tmp=index_tmp+2;
  memcpy(& USART1_TX_BUF[index_tmp],&dev_data.crc,2);
  index_tmp=index_tmp+2;
  dev_data.len=dev_data.len-HEAD_CRC_LEN-1;
  memcpy(& USART1_TX_BUF[index_tmp],&dev_data.len,2);
    dev_data.len=dev_data.len+HEAD_CRC_LEN+1;
  index_tmp=index_tmp+2;
  memcpy(& USART1_TX_BUF[index_tmp],&dev_data.dev_type,1);
  index_tmp=index_tmp+1;
  memcpy(& USART1_TX_BUF[index_tmp],&dev_data.sn,4);
  index_tmp=index_tmp+4;
  memcpy(& USART1_TX_BUF[index_tmp],&dev_data.cmd_type,1);
  index_tmp=index_tmp+1;
  memcpy(& USART1_TX_BUF[index_tmp],&dev_data.cmd_num,1);
  index_tmp=index_tmp+1;

  memcpy(&USART1_TX_BUF[index_tmp],dev_data.payload,dev_data.len-NOT_PAYLOAD_LEN);

/////////////////////////////////////////////
  crc_sum=crc_calculate(&USART1_TX_BUF[LEN_INDEX], dev_data.len-HEAD_CRC); 
//  crc_sum1=MSG_CRC;
 // crc_accumulate(&crc_sum1, &sum_crc1); 


  dev_data.crc=crc_sum;
  memcpy(& USART1_TX_BUF[CRC_INDEX],&dev_data.crc,2);
  index_tmp=index_tmp+2;
  //dev_data.len=payload_len+HEAD_CRC_LEN;
  uart1_tx(USART1_TX_BUF,dev_data.len-1);
}



void payload_pack_test()
{
//  uint8_t index_tmp;
//  uint8_t tmp;
//  USART1_TX_BUF[0]=SER_HEAD;	//payload header
//  
//  
//  
//  index_tmp=DEV_payload_index;
//  USART1_TX_BUF[index_tmp]=SER_node;//device type
//  payload_len=1;
//  index_tmp=index_tmp+1;
//  
//  //len_tmp=payload_len+1;
//  
//  USART1_TX_BUF[index_tmp++]=0x65;
//  USART1_TX_BUF[index_tmp++]=0x53;
//  USART1_TX_BUF[index_tmp++]=0x45;
//  USART1_TX_BUF[index_tmp++]=0x34;       
//  
//  USART1_TX_BUF[index_tmp++]=1;
//  USART1_TX_BUF[index_tmp++]=0x10;  
//  
//  payload_len=payload_len+6;
//  
//  
//  /*memcpy(&USART1_TX_BUF[index_tmp],pbuff,sizeof(pbuff));
//  index_tmp=index_tmp+sizeof(pbuff);
//  payload_len=payload_len+sizeof(pbuff);*/
//  
//  
//  
//  payload_len=payload_len+2;
//  USART1_TX_BUF[DEV_PAYLOAD_INDEX]=(unsigned char)(payload_len/256);
//  USART1_TX_BUF[DEV_len_index+1]=(unsigned char)(payload_len%256);
//  USART1_TX_BUF[payload_len+1]=MSG_CRC;
//  crc_sum=crc_calculate(&USART1_TX_BUF[DEV_len_index], payload_len+1);
//  
//  //payload_len=payload_len+2;
//  
//  
//  USART1_TX_BUF[payload_len+1]=(unsigned char)(crc_sum/256);
//  USART1_TX_BUF[payload_len+2]=(unsigned char)(crc_sum%256);
//  
//  uart1_tx(USART1_TX_BUF,payload_len+3);
}

void Sensor_Process()
{
  u8 i;
  i=0;
  SHT30_Get_TH(&m_TemSymbol, &m_Tem, &m_Hum);
  if(m_TemSymbol==0)
    m_Tem=m_Tem||0x8000;
  pbuff[i++]=(unsigned char)(m_Tem>>8);
  pbuff[i++]=(unsigned char)(m_Tem%256);
  pbuff[i++]=(unsigned char)(m_Hum>>8);
  pbuff[i++]=(unsigned char)(m_Hum); 
}

void RF_Process()
{
  
  //lora_init();
  
  Sensor_Process();     
  payload_pack();
  //payload_pack_test();
}
void server_command_process()
{
    unsigned char index;
	index=uart1_Process();
	if(index!=255)
	 {

          server_cmd.cmd_type=USART1_RX_BUF[index+6];
	  index++;
	  //copy sn
	  server_cmd.sn=USART1_RX_BUF[index+6];
	  server_cmd.sn=server_cmd.sn<<8;
	  index++;
	  server_cmd.sn=server_cmd.sn+USART1_RX_BUF[index+6];
	  server_cmd.sn=server_cmd.sn<<8;
	  index++;
	  server_cmd.sn=server_cmd.sn+USART1_RX_BUF[index+6];
	  server_cmd.sn=server_cmd.sn<<8;
	  index++;
	  server_cmd.sn=server_cmd.sn+USART1_RX_BUF[index+6];
	  //server_cmd.sn=server_cmd.sn<<8;
	  index++;


	 server_cmd.cmd_type=USART1_RX_BUF[index+6];
	  index++;	
	 server_cmd.cmd_num=USART1_RX_BUF[index+6];
	 index++;
	 
	 if(dev_sn==server_cmd.sn||server_cmd.sn==0xffff)
	 {
	   if( server_cmd.cmd_type==CMD_SET)
	   {
		 if( server_cmd.cmd_num==PUMP_ON||((m_Tem>=TEMPERATURE_MAX)&&(m_Hum<=HUM_MIN)))
		 {GPIO_ToggleBits(GPIOD,GPIO_Pin_7);
		   PUMP_CTRL(1);	  
		 }
		 if( server_cmd.cmd_num==PUMP_OFF||((m_Tem<=TEMPERATURE_MIN)||(m_Hum>=HUM_MAX)))
		 {
			GPIO_ToggleBits(GPIOD,GPIO_Pin_7);	   PUMP_CTRL(2);
		   //PUMP_CTRL(0);
		 }
	   }
	 }
	}

}

void PUMP_test()
{
PUMP_CTRL(1);		
PUMP_CTRL(0);
}

