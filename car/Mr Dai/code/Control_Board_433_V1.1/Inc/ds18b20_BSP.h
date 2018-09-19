#ifndef ds18b20_BSP_H_
#define  ds18b20_BSP_H_
void DQ(unsigned char flag);
unsigned char read_DQ(void);
void DS18B20_IO_OUT(void) ;
void DS18B20_IO_IN(void);
unsigned char DS18B20_DQ_IN(void);
void DS18B20_DQ_OUT(unsigned char flag);
#endif
