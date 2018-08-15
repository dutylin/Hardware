#ifndef RF_PROCESS_H_

#define RF_PROCESS_H_

typedef  struct{
	unsigned int len;
	unsigned int dev_sn;//设备的序列号  
	unsigned int temperature;
}RF_INFO_DEF;//电池电压，GPS位置数据，心跳包、蓝牙和433无线模块接收到的外设数据等必要信息

void RF_Process(void);
unsigned char RF_Param(unsigned char *pbuff, RF_INFO_DEF rf_info);

#endif
