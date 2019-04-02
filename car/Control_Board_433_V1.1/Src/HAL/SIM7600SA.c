#include "main.h"
#include "BSP.h"
#include "LTE_BSP.h"
#include "stm32f4xx_hal.h"
#include "systick_timer.h"
#include "LED_BSP.h"
#include <string.h>
#include "AT_CMD.h"
#include "SIM7600SA.h"
#include "m25p64.h"
#include "GPS_analysis.h"


#define Network_Thres    30


extern Uart_Types_2 UsartType2;
extern Uart_Types_2 UsartType3;
extern Uart_Types UsartType5;
unsigned char Establish_TCP_Connection[100]="AT+CIPOPEN=0,\"TCP\",\"zhangmeng5337.iask.in\",15969\r";
//unsigned char Establish_TCP_Connection[100]="AT+CIPOPEN=0,\"TCP\",\"www. baidu.com.cn\",80\r";



unsigned char	NET_STAUS=SIMCOM_NET_NOT;//Õ¯¬Á¡¥Ω”◊¥Ã¨–≈œ¢±Í÷æ
uint32_t  SIMCOM_TimeOut_Count;
unsigned char Network_Intens;
unsigned char* server_ip_tmp=Establish_TCP_Connection;

void reconfig_IP(unsigned char *pbuff,unsigned int len)
{
	unsigned int i;
	for(i=0;i<len;len++)
	{
	 Establish_TCP_Connection[i]=pbuff[i];
	}
}
/************************************************************
Function:  init LTE Module
Name:      LTE_M_init()
Parameter: no
************************************************************/
void LTE_M_init()
{

	

	LTE_hw_init();
	LTE_power_on();
	LET_gps_power_ctrl(ON);
	LET_power_ctrl();
	LET_reset();
}
//’“≥ˆtemp‘⁄targetµƒŒª÷√
int FindIndex(char *target,char temp)
{
	int i= 0;
	if(target ==NULL)
	{
		//  printf("À—À˜¥ Œ™ø’...");
		return 0;
	}
	for(i = strlen(target) -1; i>=0; i--)
	{
		if(target[i] == temp)
		{
			return i;
		}
	}
	return -1;  //Œ¥’“µΩ◊÷∑˚∆•≈‰Œª÷√

}
/************************************************************
Function:  string match or not that return froom LTE module
Name:      AT_cmd_ack(unsigned char *src,unsigned char *dest)
Parameter: 1:string match, 0: string not match
************************************************************/
unsigned char AT_cmd_ack(unsigned char *src,unsigned char *ack)
{
	int i= 0,j = 0;
	int srclen = strlen((const char *)src);
	int tarlen=strlen((const char *)ack);
	int temp  =0;
	int index = -1;
	if(src ==NULL || ack ==NULL)
	{
		return 0;
	}

	while(i<srclen)
	{
		//—≠ª∑Ãıº˛
		if(src[i] == ack[j])
		{
			if(j==tarlen-1)
			{
				// printf("∆•≈‰≥…π¶...");
				return 1;
			}
			i++;
			j++;
		}
		else
		{
			//∑¢œ÷≤ªœ‡µ»Œª÷√
			temp = tarlen - j + i;  //◊÷∑˚¥Æ∫Û√Êµƒµ⁄“ª∏ˆ◊÷∑˚Œª÷√
			index = FindIndex((char *)ack,src[temp]);
			if(index==-1)
			{
				//Œ¥’“µΩŒª÷√£¨∫Û“∆
				i = temp+1;
				j = 0;
			}
			else
			{
				//’“µΩŒª÷√
				i = temp-index;
				j = 0;
			}
		}
	}
//  printf("∆•≈‰ ß∞‹..");
	return 0;
}


unsigned char at_cmd_ok(unsigned char *src)
{
	if(AT_cmd_ack(src,(unsigned char *)Respond_OK)==1)//reset
	{
		return 1;
	}
	return 0;

}

unsigned char  SIMCOM_Get_InitReady_Staus(unsigned int waittime)
{
	unsigned char res=0;
	//Send_Comm((const char*)Normal_Mode,strlen((const char*)Normal_Mode));

	if(waittime)		//–Ë“™µ»¥˝”¶¥
	{
		while(--waittime)	//µ»¥˝µπº∆ ±
		{
			delay_ms(10);
			if(UsartType5.receive_flag==1)//Ω” ’µΩ∆⁄¥˝µƒ”¶¥Ω·π˚
			{
				if(AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)Respond_Start))
				{
					UsartType5.receive_flag=0;
					res=1;
					break;//µ√µΩ”––ß ˝æ›
				}


			}
		}
		if(waittime==0)
			res=0;
	}
	return res;
}

unsigned char  SIMCOM_Get_Echo_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)Echo_Dis,strlen((const char*)Echo_Dis));

	if(waittime)		//–Ë“™µ»¥˝”¶¥
	{
		while(--waittime)	//µ»¥˝µπº∆ ±
		{
			delay_ms(10);
			if(UsartType5.receive_flag==1)//Ω” ’µΩ∆⁄¥˝µƒ”¶¥Ω·π˚
			{
				if(AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)Respond_OK))
				{
					UsartType5.receive_flag=0;
					res=1;
					break;//µ√µΩ”––ß ˝æ›
				}

			}
		}
		if(waittime==0)res=0;
	}
	return res;
}
signed char  SIMCOM_Get_NormalMode_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)Normal_Mode,strlen((const char*)Normal_Mode));

	if(waittime)		//–Ë“™µ»¥˝”¶¥
	{
		while(--waittime)	//µ»¥˝µπº∆ ±
		{
			delay_ms(10);
			if(UsartType5.receive_flag==1)//Ω” ’µΩ∆⁄¥˝µƒ”¶¥Ω·π˚
			{
				if(AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)Respond_OK))
				{
					UsartType5.receive_flag=0;
					res=1;
					break;//µ√µΩ”––ß ˝æ›
				}

			}
		}
		if(waittime==0)res=0;
	}
	return res;
}
unsigned int tmpp;
signed char  SIMCOM_Get_SimCard_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)Check_SIM,strlen((const char*)Check_SIM));

	if(waittime)		//–Ë“™µ»¥˝”¶¥
	{
		tmpp=waittime;
		while(--waittime)	//µ»¥˝µπº∆ ±
		{
			delay_ms(10);
			if(UsartType5.receive_flag==1)//Ω” ’µΩ∆⁄¥˝µƒ”¶¥Ω·π˚
			{
				if(AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)Respond_CPIN))
				{
					UsartType5.receive_flag=0;
					res=1;
					break;//µ√µΩ”––ß ˝æ›
				}

			}
		}
		if(waittime==0)res=0;
	}
	return res;
}
signed char  SIMCOM_Get_Network_Intensity_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)Network_Intensity,strlen((const char*)Network_Intensity));

	if(waittime)		//–Ë“™µ»¥˝”¶¥
	{
		while(--waittime)	//µ»¥˝µπº∆ ±
		{
			delay_ms(10);
			if(UsartType5.receive_flag==1)//Ω” ’µΩ∆⁄¥˝µƒ”¶¥Ω·π˚
			{
				if(AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)Respond_Network_Intensity))
				{
					UsartType5.receive_flag=0;
					res=0;
					break;//µ√µΩ”––ß ˝æ›
				}
				else if(AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)Respond_Network_Intensity_eff))
				{
					res=1;
					UsartType5.receive_flag=0;

				}
				break;
				//res=0;

			}
		}
		if(waittime==0)res=0;
	}
	return res;
}
signed char  SIMCOM_Get_GPRS_Register_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)GPRS_Register,strlen((const char*)GPRS_Register));

	if(waittime)		//–Ë“™µ»¥˝”¶¥
	{
		while(--waittime)	//µ»¥˝µπº∆ ±
		{
			delay_ms(10);
			if(UsartType5.receive_flag==1)//Ω” ’µΩ∆⁄¥˝µƒ”¶¥Ω·π˚
			{
				if(AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)Respond_OK))
				{
					res=1;
					UsartType5.receive_flag=0;
					break;//µ√µΩ”––ß ˝æ›

				}

				//res=0;
			}
		}
		if(waittime==0)res=0;
	}
	return res;
}

signed char  SIMCOM_Get_StartGPS_Register_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)Start_GPS,strlen((const char*)Start_GPS));

	if(waittime)		//–Ë“™µ»¥˝”¶¥
	{
		while(--waittime)	//µ»¥˝µπº∆ ±
		{
			delay_ms(10);
			if(UsartType5.receive_flag==1)//Ω” ’µΩ∆⁄¥˝µƒ”¶¥Ω·π˚
			{
				if(AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)Respond_OK))
				{
					res=1;
					UsartType5.receive_flag=0;
					break;//µ√µΩ”––ß ˝æ›

				}

				//res=0;
			}
		}
		if(waittime==0)res=0;
	}
	return res;
}
unsigned char  SIMCOM_GetGPS_Info(unsigned char * buff,GPS_INFO *gps,unsigned char gps_format,unsigned int waittime)
{
	unsigned char res=0;
	unsigned char i;
	Send_Comm((unsigned char*)AT_GPS_info,strlen((const char*)AT_GPS_info));

	if(waittime)		//–Ë“™µ»¥˝”¶¥
	{
		while(--waittime)	//µ»¥˝µπº∆ ±
		{
			delay_ms(10);
			if(UsartType5.receive_flag==1)//Ω” ’µΩ∆⁄¥˝µƒ”¶¥Ω·π˚
			{
				for(i=0; i<UsartType5.rx_len_var; i++)
				{
					if(buff[i]=='+')
						break;

				}


				if(GPS_RMC_Parse(&buff[i], gps,1))
				{
					res=1;
					UsartType5.receive_flag=0;
					if(SIMCOM_Get_TO_DataMode_Staus(150)==1)
						return 1;
					break;//µ√µΩ”––ß ˝æ›
				}
			}
		}
		if(waittime==0)res=0;
	}
	return res;
}

signed char  SIMCOM_Get_Transparent_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)Pass_Through,strlen((const char*)Pass_Through));

	if(waittime)		//–Ë“™µ»¥˝”¶¥
	{
		while(--waittime)	//µ»¥˝µπº∆ ±
		{
			delay_ms(10);
			if(UsartType5.receive_flag==1)//Ω” ’µΩ∆⁄¥˝µƒ”¶¥Ω·π˚
			{
				if(AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)Respond_OK))
				{
					res=1;
					UsartType5.receive_flag=0;
					break;//µ√µΩ”––ß ˝æ›

				}

				//res=0;
			}
		}
		if(waittime==0)res=0;
	}
	return res;
}
signed char  SIMCOM_Get_NetOpen_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)Net_Open,strlen((const char*)Net_Open));

	if(waittime)		//–Ë“™µ»¥˝”¶¥
	{
		while(--waittime)	//µ»¥˝µπº∆ ±
		{
			delay_ms(10);
			if(UsartType5.receive_flag==1)//Ω” ’µΩ∆⁄¥˝µƒ”¶¥Ω·π˚
			{
				if(AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)Respond_Network_Open2)||
				        AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)Respond_Network_Open))
				{
					res=1;
					UsartType5.receive_flag=0;
					break;//µ√µΩ”––ß ˝æ›

				}

			}
			if(waittime==0)res=0;
		}

	}
			return res;
}
signed char  SIMCOM_Get_NetClose_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)Net_Close,strlen((const char*)Net_Close));

	if(waittime)		//–Ë“™µ»¥˝”¶¥
	{
		while(--waittime)	//µ»¥˝µπº∆ ±
		{
			delay_ms(10);
			if(UsartType5.receive_flag==1)//Ω” ’µΩ∆⁄¥˝µƒ”¶¥Ω·π˚
			{
				if(AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)Respond_OK))
				{
					res=1;
					UsartType5.receive_flag=0;
					break;//µ√µΩ”––ß ˝æ›

				}

			}
			if(waittime==0)res=0;
		}

	}
			return res;
}
signed char  SIMCOM_Get_CIPClose_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)CIP_Close,strlen((const char*)CIP_Close));

	if(waittime)		//–Ë“™µ»¥˝”¶¥
	{
		while(--waittime)	//µ»¥˝µπº∆ ±
		{
			delay_ms(10);
			if(UsartType5.receive_flag==1)//Ω” ’µΩ∆⁄¥˝µƒ”¶¥Ω·π˚
			{
				if(AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)Respond_OK))
				{
					res=1;
					UsartType5.receive_flag=0;
					break;//µ√µΩ”––ß ˝æ›

				}

			}
			if(waittime==0)res=0;
		}

	}
			return res;
}

signed char  SIMCOM_Get_EstablishTCP_Staus(unsigned int waittime,unsigned char *server_ip)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)server_ip,strlen((const char*)server_ip));

	if(waittime)		//–Ë“™µ»¥˝”¶¥
	{
		while(--waittime)	//µ»¥˝µπº∆ ±
		{
			delay_ms(10);
			if(UsartType5.receive_flag==1)//Ω” ’µΩ∆⁄¥˝µƒ”¶¥Ω·π˚
			{
				if(AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)Respond_TCP_Connect))
				{
					res=1;
					UsartType5.receive_flag=0;
					break;//µ√µΩ”––ß ˝æ›
				}


				//res=0;
			}
		}
		if(waittime==0)res=0;
	}
	return res;
}
signed char  SIMCOM_Get_TCP_Staus(unsigned int waittime)
{
	unsigned char res=0;
	//Send_Comm((const char*)Establish_TCP_Connection,strlen((const char*)Establish_TCP_Connection));

	if(waittime)		//–Ë“™µ»¥˝”¶¥
	{
		while(--waittime)	//µ»¥˝µπº∆ ±
		{
			delay_ms(5);
			if(UsartType5.receive_flag==1)//Ω” ’µΩ∆⁄¥˝µƒ”¶¥Ω·π˚
			{
				if(AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)TCP_Closed)
					||AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)Respond_No_Carrier)
					||AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)TCP_ERROR))
				{
					res=1;
					UsartType5.receive_flag=0;
					break;//µ√µΩ”––ß ˝æ›

				}

				//res=0;TIMEOUT
			}
		}
		if(waittime==0)res=0;
	}
	return res;
}
signed char  SIMCOM_Get_TIMEOUT_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)TIMEOUT,strlen((const char*)TIMEOUT));

	if(waittime)		//–Ë“™µ»¥˝”¶¥
	{
		while(--waittime)	//µ»¥˝µπº∆ ±
		{
			delay_ms(5);
			if(UsartType5.receive_flag==1)//Ω” ’µΩ∆⁄¥˝µƒ”¶¥Ω·π˚
			{
				if(AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)TCP_Closed))
					//||AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)TCP_ERROR))
				{
					res=1;
					UsartType5.receive_flag=0;
					break;//µ√µΩ”––ß ˝æ›

				}

				//res=0;
			}
		}
		if(waittime==0)res=0;
	}
	return res;
}

signed char  SIMCOM_Get_QuitTransparent_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)Quit_transparent,strlen((const char*)Quit_transparent));
  
	if(waittime)		//–Ë“™µ»¥˝”¶¥
	{
		while(--waittime)	//µ»¥˝µπº∆ ±
		{
			delay_ms(1);
			if(UsartType5.receive_flag==1)//Ω” ’µΩ∆⁄¥˝µƒ”¶¥Ω·π˚
			{
				if(AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)Respond_OK))
				{
					res=1;
					delay_ms(200);
					UsartType5.receive_flag=0;
					break;//µ√µΩ”––ß ˝æ›
				}


				//res=0;
			}
		}
		if(waittime==0)
			res=0;
	}
	return res;
}
signed char  SIMCOM_Get_TO_DataMode_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)TO_DATA_MODE,strlen((const char*)TO_DATA_MODE));

	if(waittime)		//–Ë“™µ»¥˝”¶¥
	{
		while(--waittime)	//µ»¥˝µπº∆ ±
		{
			delay_ms(10);
			if(UsartType5.receive_flag==1)//Ω” ’µΩ∆⁄¥˝µƒ”¶¥Ω·π˚
			{
				if(AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)Respond_TCP_Connect))
				{
					res=1;
					UsartType5.receive_flag=0;
					break;//µ√µΩ”––ß ˝æ›

				}


				//res=0;
			}
		}
		if(waittime==0)res=0;
	}
	return res;
}
unsigned char  SIMCOM_Get_Trans_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)Pass_Through,strlen((const char*)Pass_Through));

	if(waittime)		//–Ë“™µ»¥˝”¶¥
	{
		while(--waittime)	//µ»¥˝µπº∆ ±
		{
			delay_ms(10);
			if(UsartType5.receive_flag==1)//Ω” ’µΩ∆⁄¥˝µƒ”¶¥Ω·π˚
			{
				if(AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],(unsigned char*)Respond_OK))
				{
					res=1;
					UsartType5.receive_flag=0;
					break;//µ√µΩ”––ß ˝æ›

				}


				//res=0;
			}
		}
		if(waittime==0)res=0;
	}
	return res;
}


//œÚsim900a∑¢ÀÕ√¸¡Ó
//cmd:∑¢ÀÕµƒ√¸¡Ó◊÷∑˚¥Æ(≤ª–Ë“™ÃÌº”ªÿ≥µ¡À),µ±cmd<0XFFµƒ ±∫Ú,∑¢ÀÕ ˝◊÷(±»»Á∑¢ÀÕ0X1A),¥Û”⁄µƒ ±∫Ú∑¢ÀÕ◊÷∑˚¥Æ.
//ack:∆⁄¥˝µƒ”¶¥Ω·π˚,»Áπ˚Œ™ø’,‘Ú±Ì æ≤ª–Ë“™µ»¥˝”¶¥
//waittime:µ»¥˝ ±º‰(µ•Œª:10ms)
//∑µªÿ÷µ:0,∑¢ÀÕ≥…π¶(µ√µΩ¡À∆⁄¥˝µƒ”¶¥Ω·π˚)
//       1,∑¢ÀÕ ß∞‹
unsigned char  SIMCOM_SendAT(unsigned char *cmd,unsigned char *ack,unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)cmd,strlen((const char*)cmd));

	if(waittime)		//–Ë“™µ»¥˝”¶¥
	{
		while(--waittime)	//µ»¥˝µπº∆ ±
		{
			delay_ms(10);
			if(UsartType5.receive_flag==1)//Ω” ’µΩ∆⁄¥˝µƒ”¶¥Ω·π˚
			{
				if(AT_cmd_ack(&UsartType5.usartDMA_rxBuf[UsartType5.real_index],ack))
				{
					res=1;
					UsartType5.receive_flag=0;
					break;//µ√µΩ”––ß ˝æ›

				}


			}
		}
		if(waittime==0)res=0;
	}
	return res;
}


void SIMCOM_ReConnect()
{
	if(NET_STAUS!=SIMCOM_NET_NOT&&NET_STAUS!=SIMCOM_NET_OK) //øÿ÷∆œµÕ≥≥¨ ±ª˙÷∆£¨±£÷§œµÕ≥ƒ‹πª‘⁄≥¨ ±∫Û÷ÿ–¬∆Ù∂Ø¡¥Ω”
		SIMCOM_TimeOut_Count++;
	else if(NET_STAUS==SIMCOM_NET_OK)
		SIMCOM_TimeOut_Count=0;
	if(SIMCOM_TimeOut_Count>=SIMCOM_TIME_OUT)
	{
	       SIMCOM_TimeOut_Count=0;
		NET_STAUS=SIMCOM_NET_ERROR;
	}

}
void Net_Status_Change(unsigned status_tmp,unsigned char update_ip_flag,unsigned char *server_ip)
{
	switch(status_tmp)
		{
		case SIMCOM_NET_NOT:
		{
			//if(SIMCOM_Get_InitReady_Staus(1000)==1)
				NET_STAUS=SIMCOM_NET_NOT;
		}
		break;
		case SIMCOM_READY_YES:
		{
			//if( SIMCOM_Get_Echo_Staus(500)==1)
				NET_STAUS=SIMCOM_READY_YES;
		}
		break;
		case SIMCOM_Echo_OFF:
		{
			//if(SIMCOM_Get_NormalMode_Staus(500)==1)
				NET_STAUS=SIMCOM_Echo_OFF;
		}
		break;
		case SIMCOM_NORMAL_MODE:
		{
			//if(SIMCOM_Get_SimCard_Staus(500)==1)
				NET_STAUS=SIMCOM_NORMAL_MODE;
		}
		break;
				
		case SIMCOM_CIPClose_MODE:
		{
			//if(SIMCOM_Get_Network_Intensity_Staus(500)==1)
				NET_STAUS=SIMCOM_CIPClose_MODE;
			if(update_ip_flag==1)
			server_ip_tmp=server_ip;
		}
		break;	
		case SIMCOM_SIMCARD_READY:
		{
			//if(SIMCOM_Get_Network_Intensity_Staus(500)==1)
				NET_STAUS=SIMCOM_SIMCARD_READY;


		}
		break;
		case SIMCOM_Network_Intensity_READY:
		{
			//if(SIMCOM_Get_GPRS_Register_Staus(200)==1)
				NET_STAUS=SIMCOM_Network_Intensity_READY;
		}
		break;
		case SIMCOM_GPRS_READY:
		{
			//if(SIMCOM_Get_StartGPS_Register_Staus(200)==1)
				NET_STAUS=SIMCOM_GPRS_READY;
		}
		break;
		case SIMCOM_START_GPS:
		{
			//if(SIMCOM_Get_Transparent_Staus(200)==1)
				NET_STAUS=SIMCOM_START_GPS;
		}
		break;
		case SIMCOM_TRANSPERENT_MODE:
		{
			//if(SIMCOM_Get_NetOpen_Staus(200)==1)
				NET_STAUS=SIMCOM_TRANSPERENT_MODE;
		}
		break;
		case SIMCOM_NetOpen_READY:
		{
			//if(SIMCOM_Get_EstablishTCP_Staus(500)==1)
				NET_STAUS=SIMCOM_NetOpen_READY;
			
		}
		break;
		case SIMCOM_NET_OK:
		{
			//if(SIMCOM_Get_TCP_Staus(40)==1)
				NET_STAUS=SIMCOM_NET_OK;
		}
		break;
		case SIMCOM_NET_ERROR:
		{
			LET_reset();					//∏¥Œª÷ÿ∆
			NET_STAUS=SIMCOM_NET_NOT;
			//NET_STAUS=SIMCOM_NET_ERROR;		//◊¥Ã¨ª˙∏¥Œª
		}
		break;
		}



	

}
void SIMCOM_Register_Network()
{
	SIMCOM_ReConnect();


	switch(NET_STAUS)
	{
	case SIMCOM_NET_NOT:
	{
		if(SIMCOM_Get_InitReady_Staus(200)==1)
			NET_STAUS=SIMCOM_READY_YES;
		   
	}
	break;
	case SIMCOM_READY_YES:
	{
		if( SIMCOM_Get_Echo_Staus(300)==1)
			NET_STAUS=SIMCOM_Echo_OFF;
	}
	break;
	case SIMCOM_Echo_OFF:
	{
		if(SIMCOM_Get_StartGPS_Register_Staus(300)==1)
			NET_STAUS=SIMCOM_START_GPS;
	}
	break;
	case SIMCOM_START_GPS:
	{	
		if(SIMCOM_Get_NormalMode_Staus(300)==1)
			NET_STAUS=SIMCOM_NORMAL_MODE;
	}
	break;
	case SIMCOM_NORMAL_MODE:
	{
		if(SIMCOM_Get_SimCard_Staus(300)==1)
			NET_STAUS=SIMCOM_SIMCARD_READY;
		//server_ip_tmp=Establish_TCP_Connection;
	}
	break;

	case SIMCOM_SIMCARD_READY:
	{
		if(SIMCOM_Get_Network_Intensity_Staus(300)==1)
			NET_STAUS=SIMCOM_Network_Intensity_READY;
	}
	break;
	case SIMCOM_Network_Intensity_READY:
	{
		if(SIMCOM_Get_GPRS_Register_Staus(300)==1)
			NET_STAUS=SIMCOM_GPRS_READY;
	}
	break;
	/*case SIMCOM_GPRS_READY:
	{
		if(SIMCOM_Get_StartGPS_Register_Staus(200)==1)
			NET_STAUS=SIMCOM_START_GPS;
	}
	break;*/
	case SIMCOM_GPRS_READY:
	{
		if(SIMCOM_Get_Transparent_Staus(300)==1)
			NET_STAUS=SIMCOM_TRANSPERENT_MODE;
	}
	break;
	case SIMCOM_CIPClose_MODE:
	{
		if(SIMCOM_Get_CIPClose_Staus(300)==1)
			NET_STAUS=SIMCOM_NET_ERROR;
	}
	break;	
	case SIMCOM_NetClose_MODE:
	{
		if(SIMCOM_Get_NetClose_Staus(300)==1)
			NET_STAUS=SIMCOM_TRANSPERENT_MODE;
	}
	break;	
	case SIMCOM_TRANSPERENT_MODE:
	{
		if(SIMCOM_Get_NetOpen_Staus(300)==1)
			NET_STAUS=SIMCOM_NetOpen_READY;
	}
	break;
	case SIMCOM_NetOpen_READY:
	{
		//server_ip_tmp=Establish_TCP_Connection;
		if(SIMCOM_Get_EstablishTCP_Staus(1000,server_ip_tmp)==1)
			NET_STAUS=SIMCOM_NET_OK;
		//Establish_TCP_Connection=server_ip_tmp;
	}
	break;
	case SIMCOM_NET_OK:
	{
		if(SIMCOM_Get_TCP_Staus(40)==1)
			NET_STAUS=SIMCOM_NetOpen_READY;
	}
	break;
	case SIMCOM_NET_ERROR:
	{
		LET_reset();                    //∏¥Œª÷ÿ∆Ù
		NET_STAUS=SIMCOM_NET_NOT;       //◊¥Ã¨ª˙∏¥Œª
	}
	break;
	}
	if(NET_STAUS!=SIMCOM_NET_NOT&&NET_STAUS!=SIMCOM_NET_OK)
	{
		Network_Intens=Network_Intens++;
	}
	else
	{
		Network_Intens=0;

	}
}
void SIMCOM_Reconnect_status()
{
	NET_STAUS=SIMCOM_NetOpen_READY;
}
void  Server_IP_Config(unsigned char * pbuff)
{
//	Establish_TCP_Connection=pbuff;
}
unsigned char Get_Network_status()
{
	return NET_STAUS;
}
void Set_Network_status()
{
	NET_STAUS=SIMCOM_NetOpen_READY;
}

unsigned char Get_Network_Thres()
{
	if(Network_Intens>=Network_Thres)
		return 1;
	else
		return 0;
}




