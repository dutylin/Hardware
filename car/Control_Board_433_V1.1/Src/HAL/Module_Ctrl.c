#include "Module_ctrl.h"
#include "main.h"
#include "BSP.h"
#include "LTE_BSP.h"
#include "stm32f4xx_hal.h"
#include "systick_timer.h"
#include <string.h>
#include "SIM7600SA.h"
#include "CRC.h"
#include "LTE_analysis.h"
#include "UART_Process.h"
#include "GPS_analysis.h"
#include "m25p64.h"
#include "LED_BSP.h"
#include "UART_Process.h"

GPS_INFO GPS1;
UART_INFO_DEF   Uart_Info;
device_com_up  device_com_up1;
ADC_Types  adc_value;

Module_Sample_Time sample_time_status;
device_com_down  dev_info;
unsigned char  Payload_status;
unsigned char   Payload_status_index;
unsigned char server_command_flag=0;


unsigned char LTE_TX_Buff[buffer_size];
uint32_t Wr_address=0;

extern Uart_Types_2 UsartType2;
extern Uart_Types_2 UsartType3;
extern Uart_Types UsartType5;

unsigned char Net_status_flag;
unsigned int tx_buff_index;
unsigned int tx_buff_length;
unsigned int Raddress;
unsigned int len_tmp;
unsigned char pwr_status_flag;
/**************************************************************************
数据包状态信息
**************************************************************************/
void LTE_Payload_Pack_status(unsigned char logic,unsigned char index)
{
	if(logic==0)
	{
		switch(index)
		{

		case GPS_data_index:
			Payload_status=Payload_status|GPS_data_update;
			break;
		case Temper_data_index:
			Payload_status=Payload_status|Temper_data_update;
			break;
		case UART_data_index:
			Payload_status=Payload_status|UART_data_update;
			break;
		case SPI_Flash_data_index:
			Payload_status=Payload_status|SPI_Flash_data_update;
			break;
		case Heart_data__index:
			Payload_status=Payload_status|Heart_data_update;
			break;
		case TX_data_index:
			Payload_status=Payload_status|TX_data_update;
			break;

		}

	}
	else
	{
		switch(index)
		{
		case GPS_data_index:
			Payload_status=Payload_status&GPS_data_update_mask;
			break;
		case SPI_Flash_data_mask__index:
			Payload_status=Payload_status&SPI_Flash_data_update_mask;
			break;
		case TX_data_update_mask_index:
			Payload_status=Payload_status&TX_data_update_mask;
			break;
		case Heart_data_mask_index:
			Payload_status=Payload_status&Heart_data_update_mask;
			break;

		}

	}

}
unsigned char Get_Payload_Status()
{
	return Payload_status;
}
/*void LTE_Payload_Pack()
{
	if(Net_status_flag==SIMCOM_NET_OK)
	{
		LTE_Payload_Pack_status(0,TX_data__index);
	}
	else
	{
	  // Store_DeviceInfo_Into_flash();
	   LTE_Payload_Pack_status(0,SPI_Flash_data_index);

	}
}*/

/*************************************************************************
周期性获取gps信息
*************************************************************************/
unsigned char GetGps_Info_Poll()
{
//	unsigned char pbuff[buffer_size];
	if(sample_time_status.GPS_sample_flag==1)//采样时间到
	{
		Reset_Sample_flag(GPS_INTERVAL_INDEX);//sample timer reset
		if(Net_status_flag==SIMCOM_NET_OK)//networ ok
		{
			if(SIMCOM_Get_QuitTransparent_Staus(2000)==1) //enter into command mode
			{
				delay_ms(5000);
				//memcpy(pbuff,UsartType5.usartDMA_rxBuf[UsartType5.real_index],UsartType5.)
				SIMCOM_GetGPS_Info(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],&GPS1,1,10);//get gps position
				LTE_Payload_Pack_status(0,GPS_data_index);//update data flag
				return 1;
			}
		}
		else
		{
			if(Get_Network_Thres()==1)//无信号采集gps信息
			{
				tx_buff_index=0;
				if(SIMCOM_GetGPS_Info(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],&GPS1,1,10))
				{
					len_tmp=sizeof(GPS1);
					memcpy(&LTE_TX_Buff[tx_buff_index],&GPS1,len_tmp);
					tx_buff_index=tx_buff_index+len_tmp;
					//if(Get_Payload_Status()&Temper_data_update)
					{
						memcpy(&LTE_TX_Buff[tx_buff_index],&Uart_Info.dev_sn,Uart_Info.len);
						tx_buff_index=tx_buff_index+2;
						len_tmp=len_tmp+2;
						memcpy(&LTE_TX_Buff[tx_buff_index],&Uart_Info.uart_pbuff,Uart_Info.len);
						tx_buff_index=tx_buff_index+len_tmp;
						len_tmp=len_tmp+Uart_Info.len;
						memcpy(&LTE_TX_Buff[tx_buff_index],&adc_value,8);
						tx_buff_index=tx_buff_index+len_tmp;
						len_tmp=len_tmp+8;

					}
					Device_data_store(LTE_TX_Buff,Wr_address,len_tmp);//数据存储到spi flash中
					Wr_address=Wr_address+len_tmp;
					if(Wr_address>=65535)
						Wr_address=0;
					LTE_Payload_Pack_status(0,SPI_Flash_data_index);
					return 1;

				}


			}

		}

	}
	return 0;

}

unsigned char GetServer_Info_Poll()//服务器下发指令
{

	if(Net_status_flag==SIMCOM_NET_OK)
	{
		if(LTE_Parse(&UsartType5.usartDMA_rxBuf[UsartType5.real_index], dev_info))
		{
			UsartType5.receive_flag=0;
			dev_info.function_status=UsartType5.usartDMA_rxBuf[UsartType5.real_index+HEAD_INDEX_LENGTH+PAYLOAD_LENGTH+CPU_ID_LENGTH];
			if(dev_info.function_status==0x01)//setting
			{
				memcpy(&dev_info.dev_sn,&UsartType5.usartDMA_rxBuf[UsartType5.real_index+HEAD_INDEX_LENGTH+PAYLOAD_LENGTH],CPU_ID_LENGTH);
				memcpy(&dev_info.server_ip,&UsartType5.usartDMA_rxBuf[UsartType5.real_index+HEAD_INDEX_LENGTH+PAYLOAD_LENGTH+CPU_ID_LENGTH+2],dev_info.len-HEAD_INDEX_LENGTH-PAYLOAD_LENGTH-CPU_ID_LENGTH-6);

				dev_info.gps_time_interval=UsartType5.usartDMA_rxBuf[UsartType5.real_index+HEAD_INDEX_LENGTH+PAYLOAD_LENGTH+CPU_ID_LENGTH+1];
				//memcpy(&dev_info.rf_dev_sn,&UsartType5.usartDMA_rxBuf[UsartType5.real_index+HEAD_INDEX_LENGTH+PAYLOAD_LENGTH],CPU_ID_LENGTH);
				//dev_info.rf_time_interval=UsartType5.usartDMA_rxBuf[UsartType5.real_index++HEAD_INDEX_LENGTH+PAYLOAD_LENGTH+CPU_ID_LENGTH+1];

			}
			else  //query
			{
				server_command_flag=2;
			}




		}
	}
	return 0;

}
unsigned char Re_Setting_Parameters()
{
	unsigned int i;
	unsigned char *pbuff="AT+CIPOPEN=0,\"TCP\",\"";
	if(server_command_flag==1)
	{
		for(i=0; i<dev_info.len-25; i++)
		{
			if(dev_info.dev_sn[i]==':')
			{
				pbuff[24+i]=0x5c;
				i=i+1;
				pbuff[24+i]=0x22;
				i=i+1;
				pbuff[24+i]=',';
				i=i+1;

				pbuff[24+i]=dev_info.dev_sn[i];
				//memcpy(&Establish_TCP_Connection[27+i],&dev_info.dev_sn[i],dev_info.len-25-i);
				//i=i+1;
			}
			else
				pbuff[24+i]=dev_info.dev_sn[i];

		}
		pbuff[26+i]='\r';
		pbuff[27+i]=0x22;
		Server_IP_Config(pbuff);
		server_command_flag=0;
		return 1;

	}
	else if(server_command_flag==2)
	{
		if(Get_Payload_Status()&GPS_data_update==0)
		{
			LTE_Payload_Pack_status(0,GPS_data_update);
			server_command_flag=0;
			return 2;
		}
	}
	else
		return 0;
}
unsigned char Get_Server_Heart_Status()
{
	return dev_info.function_status;
}
unsigned char GPS_Process()
{
	unsigned char i;
//	GetGps_Info_Poll();                      //获取gps信息完成，返回透传状态成功
//	unsigned char pbuff[buffer_size];
	if(sample_time_status.GPS_sample_flag==1)//采样时间到
	{
		Reset_Sample_flag(GPS_INTERVAL_INDEX);//sample timer reset
		if(Net_status_flag==SIMCOM_NET_OK)//networ ok
		{
			if(SIMCOM_Get_QuitTransparent_Staus(2000)==1) //enter into command mode
			{
				delay_ms(1000);
				//memcpy(pbuff,UsartType5.usartDMA_rxBuf[UsartType5.real_index],UsartType5.)
				if(SIMCOM_GetGPS_Info(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],&GPS1,1,10))//get gps position
				{
					LTE_Payload_Pack_status(0,GPS_data_index);//update data flag
					return 1;
				}
			}
		}
		else
		{
			if(Get_Network_Thres()==1)//无信号采集gps信息
			{
				
				if(SIMCOM_GetGPS_Info(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],&GPS1,1,10))
				{
				 
				    tx_buff_index=HEAD_INDEX_LENGTH+PAYLOAD_LENGTH;
					len_tmp=GPS_INFO_LENGTH;
					//gps save into spi flash
					memcpy(&LTE_TX_Buff[tx_buff_index],&GPS1,len_tmp);
					tx_buff_index=tx_buff_index+len_tmp;


                    //temperature save into spi flash
					for(i=0;i<TEMPERATURE_GROUPS_LENGTH;i++)
					{
						memcpy(&LTE_TX_Buff[tx_buff_index],&Uart_Info.dev_sn[i],TEMPERATURE_SN_LENGTH);
						tx_buff_index=tx_buff_index+TEMPERATURE_SN_LENGTH;
						len_tmp=len_tmp+TEMPERATURE_SN_LENGTH;
						memcpy(&LTE_TX_Buff[tx_buff_index],&Uart_Info.uart_pbuff[i*TEMPERATURE_PAYLOAD_LENGTH],TEMPERATURE_PAYLOAD_LENGTH);
						tx_buff_index=tx_buff_index+TEMPERATURE_PAYLOAD_LENGTH;
						len_tmp=len_tmp+TEMPERATURE_PAYLOAD_LENGTH;
					}

					//adc save into spi flash
					memcpy(&LTE_TX_Buff[tx_buff_index],&adc_value,ADC_PAYLOAD_LENGTH);		//copy adc data to buffer
					len_tmp=len_tmp+ADC_PAYLOAD_LENGTH;


					Device_data_store(&LTE_TX_Buff[HEAD_INDEX_LENGTH+PAYLOAD_LENGTH],Wr_address,len_tmp);//数据存储到spi flash中
					Wr_address=Wr_address+len_tmp;
					if(Wr_address>=65535)
						Wr_address=0;
					LTE_Payload_Pack_status(0,SPI_Flash_data_index);
					return 1;

				}


			}

		}

	}
	return 0;

	//  SIMCOM_Reconnect_status();                  //重新链接服务器，如果退出透传时间短，可能不需要该操作
	//SIMCOM_Register_Network();                      //实时监控网络，及时重连

}

DATA_PACKET_DEF  pkt_def_usr;

unsigned char Uart_Param(unsigned char *pbuff,unsigned char len)
{

	unsigned char res,i;
	unsigned int len_tmp;
	res=0;
	if( CRC_decode(pbuff,len))
	{
		memcpy(pkt_def_usr.pkt_data_buff,pbuff,len);
		for(i=0; i<3; i++)
		{
			if((pkt_def_usr.pkt_def.pkt_id!=Uart_Info.dev_sn[i]&&Uart_Info.dev_sn[i]==0)||pkt_def_usr.pkt_def.pkt_id==Uart_Info.dev_sn[i])
			{
					len_tmp=Uart_Info.len;
				memcpy(&Uart_Info.dev_sn[i],&pkt_def_usr.pkt_def.pkt_id,4);
				memcpy(&Uart_Info.uart_pbuff[i*8],&pkt_def_usr.pkt_data_buff[4],8);				
				Uart_Info.len=len_tmp+12;
				res=1;
				break;

			}

		}

	}

	else
		res=0;
	return res;
}

void Temperature_Process()
{
	unsigned char i;
	for(i=0; i<(UsartType3.rx_len/14); i++)
	{
		if(UsartType3.receive_flag)
		{
			if(Uart_Param(&UsartType3.usartDMA_rxBuf[UsartType3.real_index+i*14],14))
			{
				LTE_Payload_Pack_status(0,UART_data_index);
			}		
		}


	}
	if(UsartType3.rx_len)
		uart_dma_resume(3);
  UsartType3.receive_flag=0;
}
uint32_t CpuID[3];
void GetCPU_ID(void)
{
	//获取CPU唯一ID
	CpuID[0]=*(uint32_t*)(0x1fff7a10);
	CpuID[1]=*(uint32_t*)(0x1fff7a14);
	CpuID[2]=*(uint32_t*)(0x1fff7a18);
}
void SpiFlash_payload_read(uint32_t read_len)
{
	Device_data_read(&LTE_TX_Buff[tx_buff_index],Raddress,read_len);
	//return read_len;	
}
void Payload_internal_pack(unsigned char flag,uint32_t flash_read_len)
{
	unsigned int sum_crc=0;
	unsigned int i;

	//payload struct:dev_head(2)----PAYLOAD_LENGTH(4)---CPU_ID(12)---GPS(27)---TEMPERATURE(36)---GAS(8)----CRC(2)
	//payload header,len=2bytes
       //71*7+20
	LTE_TX_Buff[0]=Dev_Head>>8;//data head to server
	LTE_TX_Buff[1]=Dev_Head;//data head to server


	tx_buff_index=tx_buff_index+HEAD_INDEX_LENGTH+PAYLOAD_LENGTH;
	memcpy(&LTE_TX_Buff[tx_buff_index],CpuID,CPU_ID_LENGTH);//copy cpu id to buffer
	tx_buff_index=tx_buff_index+CPU_ID_LENGTH;
	len_tmp=len_tmp+CPU_ID_LENGTH;


	if(flag==0)
	{
		memcpy(&LTE_TX_Buff[tx_buff_index],&GPS1,GPS_INFO_LENGTH);//copy gps data to buffer
		tx_buff_index=tx_buff_index+GPS_INFO_LENGTH;
		len_tmp=len_tmp+GPS_INFO_LENGTH;

		//copy temperture data to buffer
		for(i=0;i<TEMPERATURE_GROUPS_LENGTH;i++)
		{
			memcpy(&LTE_TX_Buff[tx_buff_index],&Uart_Info.dev_sn[i],TEMPERATURE_SN_LENGTH);
			tx_buff_index=tx_buff_index+TEMPERATURE_SN_LENGTH;
			len_tmp=len_tmp+TEMPERATURE_SN_LENGTH;
			memcpy(&LTE_TX_Buff[tx_buff_index],&Uart_Info.uart_pbuff[i*TEMPERATURE_PAYLOAD_LENGTH],TEMPERATURE_PAYLOAD_LENGTH);
			tx_buff_index=tx_buff_index+TEMPERATURE_PAYLOAD_LENGTH;
			len_tmp=len_tmp+TEMPERATURE_PAYLOAD_LENGTH;
		}

		memcpy(&LTE_TX_Buff[tx_buff_index],&adc_value,ADC_PAYLOAD_LENGTH);		//copy adc data to buffer
		len_tmp=len_tmp+ADC_PAYLOAD_LENGTH;

	}
	else
	{
		//len_tmp=len_tmp+spi_flash_payload_pack();
		len_tmp=flash_read_len+len_tmp;

	}

	LTE_TX_Buff[PAYLOAD_LENGTH_INDEX]=len_tmp>>24;
	LTE_TX_Buff[PAYLOAD_LENGTH_INDEX+1]=len_tmp>>16;
	LTE_TX_Buff[PAYLOAD_LENGTH_INDEX+2]=len_tmp>>8;
	LTE_TX_Buff[PAYLOAD_LENGTH_INDEX+3]=len_tmp;
	len_tmp=len_tmp+PAYLOAD_LENGTH;
	tx_buff_index=0;
	LTE_TX_Buff[len_tmp+1]=MSG_CRC;
	len_tmp=len_tmp+1;

	//CRC calculate
	crc_init(&sum_crc);
	for(i=2; i<(len_tmp); i++)
	{
		crc_accumulate(&LTE_TX_Buff[i],&sum_crc);//CRC校验
	}
	LTE_TX_Buff[len_tmp+1]=sum_crc&0xff;
	LTE_TX_Buff[len_tmp+2]=sum_crc>>8;

}

//read data from spi flash and pack
void SpiFlash_payload_body_process()
{
	if(tx_buff_length<=(buffer_size-20))
	{	
		SpiFlash_payload_read(tx_buff_length);
		Payload_internal_pack(1,tx_buff_length);
		M25PXX_Erase_Head();
	        LTE_Payload_Pack_status(1,SPI_Flash_data_update_mask);//清spi flash数据标志位
		Raddress=0;
							
	}
	else
	{
	
		SpiFlash_payload_read(buffer_size-20);
		Raddress=Raddress+buffer_size-20;
		Payload_internal_pack(1,buffer_size-20);				
		tx_buff_length=tx_buff_length+20-buffer_size;
		tx_buff_index=0;
	
	}


}

//data pack,include data in spi flash and real time 
void Payload_Pack(unsigned char flag)
{

	if(flag==0)//pack data real time
	{
		Payload_internal_pack(0,0);
		LTE_Payload_Pack_status(1,GPS_data_update_mask);
	}
	else  //package data from spi flash 
	{
		//payload struct:dev_head(2)----PAYLOAD_LENGTH(4)---CPU_ID(12)---GPS(27)---TEMPERATURE(36)---GAS(8)----CRC(2)
		//payload in flash struct            						  ---GPS(27)---TEMPERATURE(36)---GAS(8)

		if(tx_buff_length==0)//first pack data in spi flash 
		{

			if(M25PXX_Read_Length(&tx_buff_length)==1)
			{
				SpiFlash_payload_body_process();
			}
		}
		else
		{
			SpiFlash_payload_body_process();
		}

	}
	LTE_Payload_Pack_status(0,TX_data_index);
}
void LTE_Transmit()
{
	uint32_t len2;
	if(Get_Payload_Status()&TX_data_update)
	{
		len2=(uint32_t)LTE_TX_Buff[PAYLOAD_LENGTH_INDEX]*256*256*256+(uint32_t)LTE_TX_Buff[PAYLOAD_LENGTH_INDEX+1]*256*256
			+(uint32_t)LTE_TX_Buff[PAYLOAD_LENGTH_INDEX+2]*256+(uint32_t)(LTE_TX_Buff[PAYLOAD_LENGTH_INDEX+3]);
		Send_Comm((unsigned char*)LTE_TX_Buff,len2+HEAD_INDEX_LENGTH+PAYLOAD_LENGTH);
	}

}

void Payload_Transmit()

{
	if(Net_status_flag==SIMCOM_NET_OK)
	{
		if(Get_Payload_Status()&SPI_Flash_data_update)
		{
			Payload_Pack(1);//打包spi flash data
			LTE_Transmit();
			LTE_Payload_Pack_status(1,TX_data_index);
		}
		else if(Get_Payload_Status()&GPS_data_update)
		{
			Payload_Pack(0);//打包实时收到的数据
			LTE_Transmit();
			LTE_Payload_Pack_status(1,GPS_data_index);
			LTE_Payload_Pack_status(1,TX_data_index);
		}
	}
}



void LTE_TX_Heart()
{
	unsigned char heart[]="lte_heart";
	unsigned char pbuff[21];
	memcpy(pbuff,device_com_up1.dev_sn,12);
	memcpy(&pbuff[12],heart,sizeof(heart));
	Send_Comm(pbuff,20);

}

unsigned char LTE_Heart()
{

	if(sample_time_status.LTE_heart_flag==1)
	{
		Reset_Sample_flag(LTE_Heart_INDEX);//
		/*if(dev_info.function_status==0)
		{
			Set_Network_status();
		}*/
		if(sample_time_status.GPS_sample_flag==0&&Net_status_flag== SIMCOM_NET_OK)
		{
			LTE_TX_Heart();
		}
	}
}

void PWR_On_Off()
{
	if(PWR_KEY_Dectection()==1&&pwr_status_flag==0)
	{
		if(pwr_status_flag==0)
		{
			pwr_status_flag=1;
			LTE_power_on();
			LET_gps_power_ctrl(OFF);

		}
		else
		{
			/* USER CODE BEGIN 2 */
			HW_init();
			pwr_status_flag=0;
			LED_all_ctrl(1);
			LED_ctrl(SEL_PWR_LED,PWR_LED_OFF);
			uart_init();
			LTE_M_init();
			Sample_Timer_Init();


		}

	}
}
//system run
void Modue_Run_Process()
{

	PWR_On_Off();

	Sample_Timer_Reload();

	Net_status_flag=Get_Network_status();           //实时获取网络状态
	sample_time_status=Get_Sample_flag_Status();      //获取各个参数采样时间是否到达

	Temperature_Process();

	if(GPS_Process()==0)
		memcpy(LTE_TX_Buff,&UsartType5.usartDMA_rxBuf[UsartType5.real_index],UsartType5.rx_len_var) ;
	Payload_Transmit();
	//LTE_Heart();

	SIMCOM_Register_Network();
	if(sample_time_status.LTE_Battery_flag==1)
	{
		Get_Battery_Gas_adc(10);
		Reset_Sample_flag(LTE_Battery_INDEX);//sample timer reset
	}
}





