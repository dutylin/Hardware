#ifndef Module_H_
#define Module_H_
#define Ser_Head 0xfdfd
#define Dev_Head 0xfefe

#define HEAD_INDEX   0
#define HEAD_INDEX_LENGTH  2
#define PAYLOAD_LENGTH_INDEX   2	
#define PAYLOAD_LENGTH					4
#define CPU_ID_LENGTH					12
#define GPS_INFO_LENGTH					27
#define TEMPERATURE_GROUPS_LENGTH		3
#define TEMPERATURE_SN_LENGTH		   		4
#define TEMPERATURE_PAYLOAD_LENGTH		   8
#define ADC_PAYLOAD_LENGTH		   8


#define GPS_data_update							0x01   //gps数据有更新
#define Temper_data_update					0x02   //温度数据有更新
#define UART_data_update						0x04   //其他串口数据有更新
#define SPI_Flash_data_update				0x08   //spi有数据更新
#define Heart_data_update						0x10   //心跳包更新
#define TX_data_update							0x80  //数据打包完成，需要发送 
#define SPI_Flash_data_update_mask	0xf7  
#define TX_data_update_mask					0x7f
#define Heart_data_update_mask			0xef
#define GPS_data_update_mask			0xfe



 #define GPS_data_index						    0x01  //gps数据有更新
 #define Temper_data_index						0x02   //温度数据有更新
 #define UART_data_index							0x03   //其他串口数据有更新
 #define SPI_Flash_data_index					0x04   //spi有数据更新
 #define Heart_data__index						0x05   //心跳包更新
 #define TX_data_index							  0x06  //数据打包完成，需要发送 
 #define  SPI_Flash_data_mask__index	0x07  
 #define TX_data_update_mask_index		0x08
#define  Heart_data_mask_index				0x09



void Modue_Run_Process(void);
unsigned char GetServer_Info_Poll(void);

#endif

