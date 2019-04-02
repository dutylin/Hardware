#ifndef FLASH_BSP_H_
#define FLASH_BSP_H_
unsigned char SPI1_ReadWriteByte(unsigned char TxData);
void W25QXX_CS(unsigned char flag);
void SPI1_Init(void);
unsigned char SPI1_ReadWriteByte(unsigned char TxData);
void W25QXX_CLK(unsigned char flag);

void W25QXX_SDO(unsigned char flag);
void IO_TEST(void);
#endif
