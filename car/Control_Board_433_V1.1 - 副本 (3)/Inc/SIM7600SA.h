#ifndef SIM7600SA_H_
#define SIM7600SA_H_
#include "stm32f4xx_hal.h"
#include "GPS_analysis.h"

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



typedef struct
{
	unsigned char *data_buff;
	uint32_t  len;
	uint32_t Raddress;
	uint32_t index;

} flash_data_def;

typedef struct
{
	unsigned int  server_head;
	uint32_t  len;
	unsigned char function_status;      //设置参数：0x01;查询参数:0x10
	unsigned char dev_sn[12];              //设备的序列号
	unsigned char *server_IP;			  //服务器IP地址
	//unsigned int  server_port;          //服务器端口
	unsigned char gps_time_interval;    //采集间隔
	unsigned char rf_dev_sn[4]; 		  //无线设备号
	unsigned char rf_time_interval;	  //无线采样频率
	unsigned int  crc_value;
} device_com_down;

typedef struct
{
	unsigned int  device_head;

	unsigned long int  len;      			//数据长度

	unsigned int   lte_heart;             //有效心跳包：0x5aa5，其他值为业务数据

	unsigned char *gps_pos_info;          //gps信息
	unsigned int  *tempature;              //温度信息：dev_sn(2bytes)+temperature(2bytes)...
	unsigned int  battery_voltage;        //电池电压


	unsigned char dev_sn[12];//设备的序列号
	unsigned char *server_IP;	//服务器IP地址
	unsigned int  server_port;//服务器端口
	unsigned char gps_time_interval;//采集间隔
	unsigned char rf_time_interval;//无线采样频率
	unsigned int  crc_value;
} device_com_up;




void LTE_M_init(void);
unsigned char Get_Network_status(void);

unsigned char  SIMCOM_GetGPS_Info(unsigned char * buff,GPS_INFO *gps,unsigned char gps_format,unsigned int waittime);
void SIMCOM_Reconnect_status(void);
signed char  SIMCOM_Get_QuitTransparent_Staus(unsigned int waittime);
signed char  SIMCOM_Get_TO_DataMode_Staus(unsigned int waittime);
void SIMCOM_Register_Network(void);
unsigned char Get_Network_Thres(void);
void Set_Network_status(void);
void  Server_IP_Config(unsigned char * pbuff);



#endif
