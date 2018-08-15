//#include "stm32f0xx_hal.h"
//#include "stm32f0xx.h"
//#include "RF_parameter_setting.h"
//#include "Si4463.h"

//extern uint32_t uBrate;
//extern uint32_t uart_rx_len;
//extern uint32_t rx_len_tmp;
//extern unsigned int len_var;

//extern UART_HandleTypeDef huart1;
//extern unsigned int rf_tx_index;
//extern unsigned int rf_rx_index;
//extern unsigned char tx_ph_data[buffer_size] ;  // test signal,The 10th data is a former nine checksum
//extern unsigned char rx_ph_data[buffer_size];

//unsigned char RF_mode_str[]		="woozoom rf set";
//unsigned char RF_PA[]     		="woozoom rf w pa";



////uint16_t rf_parameters_setting[20],sscom_setting_index,first_setting;
//unsigned char dest[30];
//unsigned char setting_mode=0,state;

//void my_strncpy(unsigned char *dest, unsigned char *src, unsigned char count)
//{
//	unsigned char *tmp = dest;
//	while(count)
//	{
//		*tmp = *src; 
//		src++;
//		tmp++;
//		count--;
//	}
//}



//unsigned char my_strncmp(unsigned char *string1,unsigned char *string2,size_t count)
//{
//	unsigned char res,k=0;
//	for(k=0;k<count;k++)
//	{
//		//k++;
//		if(*string1==*string2)
//		{
//			string1++;
//			string2++;
//			res=0;
//		}
//		else 
//		{
//			res=*string1-*string2;	
//			break;			
//		}
//			
//	}
//	
//	//k=0;
//	return res;
//}
//unsigned int sscom_tx_index;
//unsigned char parameters_compare(unsigned char *expected_str,unsigned char len)
//{

//	my_strncpy(&dest[0], &tx_ph_data[rf_tx_index], len_var);
//	
//	state=my_strncmp(dest,&expected_str[0],len);
//	if(state==0)
//	{
//		uart_rx_len=uart_rx_len- len_var;
//		rf_tx_index = rf_tx_index + len_var;
//		if(rf_tx_index>=buffer_size)
//			rf_tx_index = rf_tx_index - buffer_size;		
//	}
//	return state;
//}
////set uart burate
//uint32_t uSSCOM_Burate(unsigned char *p)
//{
//	uBrate=(dest[15]<<8)+dest[16];
//	return uBrate;
//}

////set rf module parameters
//unsigned char setting_rf_bySSCOM()
//{

//	unsigned char p[20];
//	if(setting_mode==1)//entry setting mode
//	{
//		switch(len_var)
//		{


////		case 14://entry set mode
////		{
////			state=parameters_compare(RF_mode_str,(sizeof(RF_mode_str)-1));
////			if(state==0)//set mode
////			{
////				setting_mode=1;
////				RF_info(p);
////				HAL_UART_Transmit(&huart1,p,15,10);
////			}
////			else
////			{
////				HAL_UART_Transmit(&huart1,p,15,10);
////				setting_mode=0;
////			}
////		}
////		break;

//		case 23://uart burate
//		{
//			state=parameters_compare(RF_PA,15);
//			if(state==0)//set sscom burate
//			{
//				
//		
//				uSSCOM_Burate(&dest[15]);

//				uRF_rate_SETTING(&dest[17]);
//				uID_SETTING(&dest[19]);
//				uPA_SETTING(&dest[21]);			
//				uCH_SETTING(&dest[22]);
//				//setting_mode=0;
//				RF_parameters_save();
//				RE_Configuration();
//				//setting_mode=0;					
//			}
//		}
//		break;
//		default:
//	//		setting_mode=0;
//			break;
//		}
//	}

//	if(len_var==14)//entry setting mode
//	{
//		state=parameters_compare(RF_mode_str,14);
//		if(state==0)
//		{
//			setting_mode=1;
//			RF_info(p);
//			HAL_UART_Transmit(&huart1,p,15,10);		
//		}
//	}
//	

//	return setting_mode;
//}
