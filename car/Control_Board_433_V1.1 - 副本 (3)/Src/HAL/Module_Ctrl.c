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

	unsigned int tmp;
	if(Net_status_flag==SIMCOM_NET_OK)
	{
		if(LTE_Parse(&UsartType5.usartDMA_rxBuf[UsartType5.real_index], dev_info))
		{
			UsartType5.receive_flag=0;
			dev_info.function_status=UsartType5.usartDMA_rxBuf[UsartType5.real_index+6];
			if(dev_info.function_status==0x01)//setting
			{
				memcpy(&dev_info.dev_sn,&UsartType5.usartDMA_rxBuf[UsartType5.real_index+7],12);
				memcpy(&dev_info.server_IP,&UsartType5.usartDMA_rxBuf[UsartType5.real_index+19],dev_info.len-25);
				//dev_info.server_port=UsartType5.usartDMA_rxBuf[UsartType5.real_index+dev_info.len-8];
				dev_info.gps_time_interval=UsartType5.usartDMA_rxBuf[UsartType5.real_index+dev_info.len-6];
				memcpy(&dev_info.rf_dev_sn,&UsartType5.usartDMA_rxBuf[UsartType5.real_index+dev_info.len-5],4);
				dev_info.rf_time_interval=UsartType5.usartDMA_rxBuf[UsartType5.real_index+dev_info.len-1];

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
	unsigned int sum_crc,i;
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
				tx_buff_index=6;
				if(SIMCOM_GetGPS_Info(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],&GPS1,1,10))
				{
					len_tmp=sizeof(GPS1);
					memcpy(&LTE_TX_Buff[tx_buff_index],&GPS1,len_tmp);
					tx_buff_index=tx_buff_index+len_tmp;

					memcpy(&LTE_TX_Buff[tx_buff_index],&Uart_Info.dev_sn,Uart_Info.len);
					tx_buff_index=tx_buff_index+2;
					len_tmp=len_tmp+2;
					memcpy(&LTE_TX_Buff[tx_buff_index],&Uart_Info.uart_pbuff,Uart_Info.len);
					tx_buff_index=tx_buff_index+len_tmp;
					len_tmp=len_tmp+Uart_Info.len;
					memcpy(&LTE_TX_Buff[tx_buff_index],&adc_value,8);
					tx_buff_index=tx_buff_index+len_tmp;
					len_tmp=len_tmp+8;

					//CRC calculate
					crc_init(&sum_crc);
					for(i=1; i<(len_tmp+3); i++)
					{
						crc_accumulate(&LTE_TX_Buff[i],&sum_crc);//CRC校验
					}
					LTE_TX_Buff[len_tmp+4]=sum_crc&0xff;
					LTE_TX_Buff[len_tmp+5]=sum_crc>>8;


					Device_data_store(LTE_TX_Buff,Wr_address,len_tmp+2);//数据存储到spi flash中
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
unsigned char Temperature_Process()
{
	if(Uart_Param(&UsartType2.usartDMA_rxBuf[UsartType2.real_index],Uart_Info))
	{
		UsartType2.receive_flag=0;
	}

}

void Payload_Pack(unsigned char flag)
{
	unsigned int sum_crc=0;
	unsigned int i;


	if(flag==0)
	{
		len_tmp=27;
		tx_buff_index=6;
		LTE_TX_Buff[0]=Dev_Head>>8;//data head to server
		LTE_TX_Buff[1]=Dev_Head;//data head to server
		memcpy(&LTE_TX_Buff[tx_buff_index],&GPS1,len_tmp);//copy gps data to buffer
		tx_buff_index=tx_buff_index+len_tmp;

		//copy temperture data to buffer
		memcpy(&LTE_TX_Buff[tx_buff_index],&Uart_Info.dev_sn,Uart_Info.len);
		tx_buff_index=tx_buff_index+2;
		len_tmp=len_tmp+2;
		memcpy(&LTE_TX_Buff[tx_buff_index],&Uart_Info.uart_pbuff,Uart_Info.len);
		tx_buff_index=tx_buff_index+len_tmp;

		memcpy(&LTE_TX_Buff[tx_buff_index],&adc_value,8);		//copy adc data to buffer
		len_tmp=len_tmp+8;

		LTE_TX_Buff[2]=len_tmp>>24;
		LTE_TX_Buff[3]=len_tmp>>16;
		LTE_TX_Buff[4]=len_tmp>>8;
		LTE_TX_Buff[5]=len_tmp;
		tx_buff_index=0;

		//CRC calculate
		crc_init(&sum_crc);
		for(i=1; i<(len_tmp+3); i++)
		{
			crc_accumulate(&LTE_TX_Buff[i],&sum_crc);//CRC校验
		}
		LTE_TX_Buff[len_tmp+4]=sum_crc&0xff;
		LTE_TX_Buff[len_tmp+5]=sum_crc>>8;


		LTE_Payload_Pack_status(1,GPS_data_update_mask);
	}
	else
	{
		if(M25PXX_Read_Length(&tx_buff_length)==1&&tx_buff_length==0)
		{
			if(tx_buff_length<=buffer_size)
			{
				tx_buff_index=4;
				Device_data_read(&LTE_TX_Buff[tx_buff_index],Raddress,tx_buff_length);
				LTE_Payload_Pack_status(1,SPI_Flash_data_update_mask);
				Raddress=0;
				tx_buff_index=0;
			}
			else
			{
				if(tx_buff_index==0)
					tx_buff_index=4;
				Device_data_read(&LTE_TX_Buff[tx_buff_index],Raddress,buffer_size);
				Raddress=Raddress+buffer_size;
				//tx_buff_index=tx_buff_index+tx_buff_length;
				tx_buff_length=tx_buff_length-buffer_size;
				tx_buff_index=0;
			}
		}
		else if(tx_buff_length)
		{
			if(tx_buff_length<=buffer_size)
			{
				tx_buff_index=4;
				Device_data_read(&LTE_TX_Buff[tx_buff_index],Raddress,tx_buff_length);
				tx_buff_length=0;
				tx_buff_index=0;
				Raddress=0;
				LTE_Payload_Pack_status(1,SPI_Flash_data_update_mask);
				tx_buff_index=0;
			}
			else
			{
				if(tx_buff_index==0)
					tx_buff_index=4;
				Device_data_read(&LTE_TX_Buff[tx_buff_index],Raddress,buffer_size);
				//tx_buff_index=tx_buff_index+tx_buff_length;
				Raddress=Raddress+buffer_size;
				tx_buff_length=tx_buff_length-buffer_size;
				tx_buff_index=0;

			}

		}
	}
	LTE_Payload_Pack_status(0,TX_data_index);
}
void LTE_Transmit()
{
	 uint32_t len2;
	if(Get_Payload_Status()&TX_data_update)
	{
		len2=(uint32_t)LTE_TX_Buff[2]*256*256*256+(uint32_t)LTE_TX_Buff[3]*256*256+(uint32_t)LTE_TX_Buff[4]*256+(uint32_t)(LTE_TX_Buff[5]);
		Send_Comm((unsigned char*)LTE_TX_Buff,len2+6);
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
//	if(sample_time_status.LTE_Battery_flag)
//	{
//		adc_value=Get_Battery_Gas_adc(50);
//		Reset_Sample_flag(LTE_Battery_INDEX);

//	}


	//if(GetServer_Info_Poll()==1)//服务器下发指令
		//;
	 if(GPS_Process()==0)
		memcpy(LTE_TX_Buff,&UsartType5.usartDMA_rxBuf[UsartType5.real_index],UsartType5.rx_len_var) ;
	Payload_Transmit();
	//LTE_Heart();

	SIMCOM_Register_Network();
	//LTE_TX_Buff[0]=1;
	//LTE_TX_Buff[1]=2;
	//LTE_TX_Buff[2]=3;
	//LTE_TX_Buff[3]=4;

 //Device_data_store(LTE_TX_Buff,0,4);
	//Device_data_read(&LTE_TX_Buff[4],0,4);
	Get_Battery_Gas_adc(10);

}





