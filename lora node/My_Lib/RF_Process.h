#ifndef RF_Process_H
#define RF_Process_H
#include "stm8l15x.h"

#define SER_HEAD   					0xfdfd
#define DEV_HEAD   					0xfefe

#define PAYLOAD_SIZE			   50
#define CRC_INDEX    			    2
#define LEN_INDEX   			    4
#define DEV_PAYLOAD_INDEX   		6

#define NOT_PAYLOAD_LEN				13
#define HEAD_CRC_LEN                           6
#define HEAD_CRC                           4
#define DEV_NODE   		       		 0x22
#define SER_NODE   		        	 0x11

//#define TEMPERATURE_MAX              400
//#define HUM_MAX                      350
//
//#define TEMPERATURE_MIN              250
//#define HUM_MIN                      400


#define CMD_NO						0x00

#define CMD_SET						0x01
#define PUMP_ON						0x01
#define PUMP_OFF					0x10
#define SENSOR_TEMPERATURE_HUM_SET			        0x12
//#define SENSOR_HUM_SET						0x13


typedef struct{
unsigned int head;
unsigned int crc;
unsigned int len;
unsigned char dev_type;
u32			  sn;
unsigned char cmd_type;
unsigned char cmd_num;
unsigned char payload[PAYLOAD_SIZE];
}USER_DATA_DEF;

typedef struct{
unsigned char dev_type;
unsigned char cmd_type;
u32 dev_dest_sn;
unsigned char cmd;
}server_cmd_def;

typedef struct{
 unsigned int TEMPERATURE_MAX;             // 400
 unsigned int HUM_MAX;                     // 350
 unsigned int TEMPERATURE_MIN;             // 250
 unsigned int HUM_MIN;                     // 400
}SENSOR_DEF;

void RF_Process(void);
void server_command_process(void);
void PUMP_test(void);
void sensor_init(void);
#endif

