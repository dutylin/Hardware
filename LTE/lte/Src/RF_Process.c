#include "main.h"
#include "stm32f1xx_hal.h"
#include "RF_Process.h"
#include "systick_timer.h"
#include "CRC.h"
#include "stdarg.h"	 	 
#include "bsp.h"
#include "lte_hal.h"

#define CRC_ENABLE      0

USER_DATA_DEF server_cmd,dev_data;

extern Uart_Types UsartType3,UsartType2,UsartType1;
unsigned char m_TemSymbol = 0;
unsigned int m_Tem = 0;
unsigned int m_Hum = 0;
uint8_t pbuff[4];
unsigned int payload_len;
unsigned short  crc_sum;
uint32_t dev_sn=0x12345678;


//void payload_pack()
//{

// uint8_t index_tmp;
//  unsigned char tmp;  
//  dev_data.len=0;

////head
//  dev_data.head=DEV_HEAD;
//  dev_data.len=dev_data.len+2;
//  //crc len
//  dev_data.len=dev_data.len+2;
//  //data package len
//  dev_data.len=dev_data.len+2;  
////dev type
//  dev_data.dev_type=DEV_NODE;
//  dev_data.len=dev_data.len+1;     
////sn;
//dev_data.sn=dev_sn;
//dev_data.len=dev_data.len+4;

////cmd cmd_num
//dev_data.cmd_type=CMD_NO;
//dev_data.cmd_num=CMD_NO;
//dev_data.len=dev_data.len+2;


//  //temperatur and humy
//  index_tmp=0;
// /* pbuff[0]=0x01;
//  pbuff[1]=0x0E;
//  pbuff[2]=0x02;
//  pbuff[3]=0x2B;*/



//  
//  memcpy(&dev_data.payload[index_tmp],pbuff,sizeof(pbuff));
//  index_tmp=index_tmp+4;
//  dev_data.len=dev_data.len+4;

//battery voltage
//  PotVoltage=(short unsigned int )PotVoltage;
//  dev_data.payload[index_tmp++]=(unsigned char )(PotVoltage/256);
//  dev_data.payload[index_tmp++]=(unsigned char )(PotVoltage%256);  
//   //memcpy(&USART1_TX_BUF[index_tmp],&PotVoltage,2);
//  dev_data.len=dev_data.len+2;

////crc code
//  dev_data.payload[index_tmp++]=MSG_CRC;
//  dev_data.len=dev_data.len+1;
//  
//  //payload len
// // dev_data.len=dev_data.len-HEAD_CRC_LEN;
//  index_tmp=0;

//  memcpy(& USART1_TX_BUF[index_tmp],&dev_data.head,2);
//  index_tmp=index_tmp+2;
//  memcpy(& USART1_TX_BUF[index_tmp],&dev_data.crc,2);
//  index_tmp=index_tmp+2;
//  dev_data.len=dev_data.len-HEAD_CRC_LEN-1;
//  memcpy(& USART1_TX_BUF[index_tmp],&dev_data.len,2);
//    dev_data.len=dev_data.len+HEAD_CRC_LEN+1;
//  index_tmp=index_tmp+2;
//  memcpy(& USART1_TX_BUF[index_tmp],&dev_data.dev_type,1);
//  index_tmp=index_tmp+1;
//  memcpy(& USART1_TX_BUF[index_tmp],&dev_data.sn,4);
//  index_tmp=index_tmp+4;
//  memcpy(& USART1_TX_BUF[index_tmp],&dev_data.cmd_type,1);
//  index_tmp=index_tmp+1;
//  memcpy(& USART1_TX_BUF[index_tmp],&dev_data.cmd_num,1);
//  index_tmp=index_tmp+1;

//  memcpy(&UsartType2.usartDMA_txBuf[index_tmp],dev_data.payload,dev_data.len-NOT_PAYLOAD_LEN);

///////////////////////////////////////////////
//  crc_sum=crc_calculate(&UsartType2.usartDMA_txBuf[LEN_INDEX], dev_data.len-HEAD_CRC); 
////  crc_sum1=MSG_CRC;
// // crc_accumulate(&crc_sum1, &sum_crc1); 


//  dev_data.crc=crc_sum;
//  memcpy(& UsartType2.usartDMA_txBuf[CRC_INDEX],&dev_data.crc,2);
//  index_tmp=index_tmp+2;
//  //dev_data.len=payload_len+HEAD_CRC_LEN;


//uart_tx(2,UsartType2.usartDMA_txBuf,payload_len);
//}



void payload_pack_test()
{
//  uint8_t index_tmp;

//  UsartType2.usartDMA_txBuf[0]=SER_HEAD;	//payload header
//  
//  
//  
//  index_tmp=DEV_payload_index;
//  UsartType2.usartDMA_txBuf[index_tmp]=SER_node;//device type
//  payload_len=1;
//  index_tmp=index_tmp+1;
//  
//  //len_tmp=payload_len+1;
//  
//  UsartType2.usartDMA_txBuf[index_tmp++]=0x65;
//  UsartType2.usartDMA_txBuf[index_tmp++]=0x53;
//  UsartType2.usartDMA_txBuf[index_tmp++]=0x45;
//  UsartType2.usartDMA_txBuf[index_tmp++]=0x34;       
//  
//  UsartType2.usartDMA_txBuf[index_tmp++]=1;
//  UsartType2.usartDMA_txBuf[index_tmp++]=0x10;  
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
//  UsartType2.usartDMA_txBuf[DEV_len_index]=(unsigned char)(payload_len/256);
//  UsartType2.usartDMA_txBuf[DEV_len_index+1]=(unsigned char)(payload_len%256);
//  UsartType2.usartDMA_txBuf[payload_len+1]=MSG_CRC;
//  crc_sum=crc_calculate(&UsartType2.usartDMA_txBuf[DEV_len_index], payload_len+1);
//  
//  //payload_len=payload_len+2;
//  
//  
//  UsartType2.usartDMA_txBuf[payload_len+1]=(unsigned char)(crc_sum/256);
//  UsartType2.usartDMA_txBuf[payload_len+2]=(unsigned char)(crc_sum%256);
//  
//  uart_tx(2,UsartType2.usartDMA_txBuf,payload_len+3);
}


void RF_Process()
{
  
  //Lora_Node_process();

}
unsigned char Server_PayloadProecss()
{
	unsigned char res=0;
	unsigned int crc_sum;
	 if(UsartType2.receive_flag==1)
	  {
		if((UsartType2.usartDMA_rxBuf[UsartType2.real_index]*256+UsartType2.usartDMA_rxBuf[UsartType2.real_index+1])==SER_HEAD)
		{
			  if( CRC_ENABLE == 1)
				{
					crc_sum=CRC_decode(&(UsartType2.usartDMA_rxBuf[UsartType2.real_index+LEN_INDEX]),UsartType2.rx_len_var-HEAD_CRC);
					if(UsartType2.usartDMA_rxBuf[UsartType2.real_index+CRC_INDEX]==(unsigned char)(crc_sum/256)&&UsartType2.usartDMA_rxBuf[UsartType2.real_index+CRC_INDEX+1]==(unsigned char)(crc_sum%256))

					//  if(CRC_decode(&(UsartType2.usartDMA_rxBuf[UsartType2.real_index+1]),UsartType2.rx_len_var-1))
						res=1;
					else
						res=0;			
				}
				else 
					res = 1;

		  
		} 
		UsartType2.receive_flag=0;
	  }
	
	return res;

}
/*deal the lora node payload */
unsigned char Lora_Node_PayloadProecss()
{
	unsigned char res=0;

	 if(UsartType3.receive_flag==1)
	  {
		
		if((UsartType3.usartDMA_rxBuf[UsartType3.real_index]*256+UsartType3.usartDMA_rxBuf[UsartType3.real_index+1])==DEV_HEAD)
		{
			 if( CRC_ENABLE == 1)
				{
			    crc_sum=CRC_decode(&(UsartType3.usartDMA_rxBuf[UsartType3.real_index+LEN_INDEX]),UsartType3.rx_len_var-HEAD_CRC);
					if(UsartType3.usartDMA_rxBuf[UsartType3.real_index+CRC_INDEX]==(unsigned char)(crc_sum/256)&&UsartType3.usartDMA_rxBuf[UsartType3.real_index+CRC_INDEX+1]==(unsigned char)(crc_sum%256))
						res=1;
					else
						res=0;				
					
				}
				else 
					res = 1;

		  
		} 
		UsartType3.receive_flag=0;
	  }
	
	return res;

}


extern UART_HandleTypeDef huart2;
void Server_Process()
{
	          //if( Get_Network_status()==SIMCOM_NET_OK)
							//HAL_UART_Transmit(&huart2,comm,len,100);
//           uart_tx(2,&UsartType2.usartDMA_txBuf[0], 50) ; 
	if(Lora_Node_PayloadProecss())
	 {
	    memcpy(&UsartType2.usartDMA_txBuf[0],&UsartType3.usartDMA_rxBuf[UsartType3.real_index],UsartType3.rx_len_var);
           UsartType2.tx_len= UsartType2.tx_len+UsartType3.rx_len_var;
          if( Get_Network_status()==SIMCOM_NET_OK)
           uart_tx(2,&UsartType2.usartDMA_txBuf[0], UsartType2.tx_len) ; 
		   UsartType2.tx_len=0;
					
					HAL_GPIO_TogglePin(LED_Ctrl_GPIO_Port, LED_Ctrl_Pin);
	   /*server_cmd.dev_type=USART1_RX_BUF[3];
	   
	   server_cmd.dev_dest_sn=(USART1_RX_BUF[4]);
	   server_cmd.dev_dest_sn=server_cmd.dev_dest_sn<<8;
	   
	   server_cmd.dev_dest_sn=server_cmd.dev_dest_sn+(USART1_RX_BUF[5]);
	   server_cmd.dev_dest_sn=server_cmd.dev_dest_sn<<8;  
	   
	   server_cmd.dev_dest_sn=server_cmd.dev_dest_sn+(USART1_RX_BUF[6]);
	   server_cmd.dev_dest_sn=server_cmd.dev_dest_sn<<8; 
	   
	   server_cmd.dev_dest_sn=server_cmd.dev_dest_sn+(USART1_RX_BUF[7]);		
	   server_cmd.cmd_type=USART1_RX_BUF[8];
	   
	   server_cmd.cmd=USART1_RX_BUF[9];
	   if(dev_sn==server_cmd.dev_dest_sn||server_cmd.dev_dest_sn==0xffff)
	   {
		 if( server_cmd.cmd_type==CMD_SET)
		 {
		   if(server_cmd.cmd==PUMP_ON||((m_Tem>=TEMPERATURE_MAX)&&(m_Hum<=HUM_MIN)))
		   {
			 PUMP_CTRL(1);		
		   }
		   if(server_cmd.cmd==PUMP_OFF||((m_Tem<=TEMPERATURE_MIN)||(m_Hum>=HUM_MAX)))
		   {
			 PUMP_CTRL(0);
		   }
		 }
	   }*/
         }
         if(Server_PayloadProecss()==1)
         	{
							HAL_GPIO_TogglePin(LED_Ctrl_GPIO_Port, LED_Ctrl_Pin);
			 memcpy(&UsartType3.usartDMA_txBuf[0],&UsartType2.usartDMA_rxBuf[UsartType3.real_index],UsartType2.rx_len_var);
				UsartType3.tx_len=UsartType2.rx_len_var+UsartType3.tx_len;
			//   if( Get_Network_status()==SIMCOM_NET_OK)
				uart_tx(3,&UsartType3.usartDMA_txBuf[0], UsartType3.tx_len) ; 
				UsartType3.tx_len=0;

		 }
}

