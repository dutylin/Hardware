#include "HMC5883L.h"
#include "I2C.h"
//#include "delay.h"
void hmc_write_reg(u8 reg,u8 data)
{
	IIC_Start();
	IIC_Send_Byte(WRITE_ADDRESS);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	IIC_Send_Byte(data);
	IIC_Wait_Ack();
	IIC_Stop();
	//delay_ms(5);
}

u8 hmc_read_reg(u8 reg)
{
	u8 data;
	IIC_Start();
	IIC_Send_Byte(WRITE_ADDRESS);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	IIC_Stop();
	IIC_Start();
	IIC_Send_Byte(READ_ADDRESS);
	IIC_Wait_Ack();
	data=IIC_Read_Byte();
	IIC_NAck();
	IIC_Stop();
	return data;
}

void hmc_read_XYZ(short int *data)
{

	*data++=((int16_t)hmc_read_reg(DATAX_M)<<8)|hmc_read_reg(DATAX_L);
	*data++=((int16_t)hmc_read_reg(DATAY_M)<<8)|hmc_read_reg(DATAY_L);
	*data++=((int16_t)hmc_read_reg(DATAZ_M)<<8)|hmc_read_reg(DATAZ_L);
	*data++=((int16_t)hmc_read_reg(TEMP_M)<<8)|hmc_read_reg(TEMP_L);	
}
u8 temp;
u8 configa_reg_value;
void hmc_init(void)
{
	hmc_write_reg(CONFIGB,0x40); 
	hmc_write_reg(0X0B,0x01);
	temp=hmc_read_reg(0X0B);
	hmc_write_reg(0X20,0x40);
	hmc_write_reg(0X21,0x01);
	hmc_write_reg(CONFIGA,0x1D);
 
	delay_ms(10);
}

u8 get_config_reg()
{
	return configa_reg_value=hmc_read_reg(CONFIGA);
}
 u8 tmp;
u8 check_rdy_status()
{
   
    tmp = hmc_read_reg(STATUS_REG);
    if(tmp&0x01)
	return 1;
    else
       return 0;
}


