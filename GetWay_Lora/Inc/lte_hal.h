#ifndef SIM7600SA_H_
#define SIM7600SA_H_
#include "stm32f1xx_hal.h"

#define Ser_Head 0xfdfd
#define Dev_Head 0xfefe

#define SIMCOM_TIME_OUT					20


//网络注册状态


//网络注册状态


#define	SIMCOM_NET_NOT 						0    	//开机未完成
#define	SIMCOM_READY_YES					1     //开机完成
#define	SIMCOM_Echo_OFF						2   	//关闭回显
#define	SIMCOM_NORMAL_MODE				3  	//进入正常工作模式
#define	SIMCOM_SIMCARD_READY			4  	//检查sim卡成功
#define	SIMCOM_Network_Intensity_READY		5  	//信号满足要求
#define	SIMCOM_GPRS_READY					6     //gprs注册成功
#define	SIMCOM_START_GPS					7  	//开启GPS
#define	SIMCOM_TRANSPERENT_MODE				8  	//进入透传模式
#define	SIMCOM_NetOpen_READY				9     //net open
#define	SIMCOM_NET_OK						10 	//链接成功
#define	SIMCOM_NET_OK2     11
#define	SIMCOM_NET_ERROR					0XFF	//错误


void LTE_M_init(void);
unsigned char Get_Network_status(void);

void SIMCOM_Reconnect_status(void);
signed char  SIMCOM_Get_QuitTransparent_Staus(unsigned int waittime);
signed char  SIMCOM_Get_TO_DataMode_Staus(unsigned int waittime);
void SIMCOM_Register_Network(void);
unsigned char Get_Network_Thres(void);
void Set_Network_status(void);
void  Server_IP_Config(unsigned char * pbuff);



#endif
