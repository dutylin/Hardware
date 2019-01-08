#include "global.h"
#include "MCP2515.h"

#ifdef MCP2515
/**************************************************************
函数：unsigned char mcp2515_clock(void)
功能：时钟配置
**************************************************************/
unsigned char mcp2515_clock(void)
{
    GPIOA->CRH.B.CNF8  =2   ;
    GPIOA->CRH.B.MODE8 =3  ;
    RCC->CFGR.B.MCO=7; //SET MCO OUTPUT CLOCK FOR mcp2515,PLL/2;
    return 0;
}
/**************************************************************
函数：unsigned char mcp2515_reset(void)
功能：复位
**************************************************************/
unsigned char mcp2515_reset(void)
{
    CS_OFF;
    delay_ms(1);
    write_data(MCP_RESET);
    delay_ms(5);
    CS_ON;
    delay_ms(1);
    return 0;
}
/**************************************************************
函数：unsigned char mcp2515_config_rate(void)
功能：波特率配置
**************************************************************/
unsigned char mcp2515_config_rate(void)
{
    CS_OFF;
    delay_ms(1);
    write_data(MCP_WRITE);
    write_data(MCP_CNF1);
    write_data(0x07);			// baud rate=125KHz,let TQ=1, BRP=16;
    write_data(0x32);	  //ps1=6
    write_data(0x83);	  //ps2=3
    CS_ON;
    delay_ms(1);
    return 0;
}
/**************************************************************
函数：unsigned int mcp2515_read_register(const unsigned char address)
功能：读取注册信息
**************************************************************/
unsigned int mcp2515_read_register(const unsigned char address)
{
    unsigned int ret;
    CS_OFF;
    delay_ms(1);
    write_data(MCP_READ);
    // delay_ms(10);
    write_data(address);
    // delay_ms(100);
    ret = read_data();
    delay_ms(1);
    CS_ON;
    delay_ms(1);
    return ret;
}
/**************************************************************
函数：unsigned char mcp2515_read_registers(const unsigned char address,
	    unsigned char values[], const unsigned char n)
功能：读取注册信息
**************************************************************/
/*********read the several register*********************/
unsigned char mcp2515_read_registers(const unsigned char address,unsigned char values[], const unsigned char n)
{
    unsigned char i;
    CS_OFF;
    delay_ms(1);
    write_data(MCP_READ);
    write_data(address);
    for (i=0; i<n; i++)
    {
        values[i] = read_data();
        delay_ms(1);
    }
    CS_ON;
    delay_ms(1);
    return 0;
}
/**************************************************************
函数：unsigned char mcp2515_read_status(void)
功能：读取注册信息
**************************************************************/
unsigned char mcp2515_read_status(void)
{
    unsigned char sta;
    CS_OFF;
    delay_ms(1);
    write_data(MCP_READ_STATUS);
    sta = read_data();
    CS_ON;
    delay_ms(1);
    return sta;
}
/**************************************************************
函数：unsigned char mcp2515_set_register(const unsigned char address,const unsigned char value)
功能：设置CAN ID和数据
**************************************************************/
unsigned char mcp2515_set_register(const unsigned char address,const unsigned char value)
{
    CS_OFF;
    delay_ms(1);
    write_data(MCP_WRITE);
    write_data(address);
    write_data(value);
    CS_ON;
    delay_ms(1);
    return 0;
}
/**************************************************************
函数：unsigned char mcp2515_set_registers(const unsigned char address,
	    const unsigned char values[], const unsigned char n)
功能：设置CAN ID和数据
**************************************************************/
unsigned char mcp2515_set_registers(const unsigned char address,const unsigned char values[], const unsigned char n)
{
    unsigned char i;
    CS_OFF;
    delay_ms(1);
    write_data(MCP_WRITE);
    write_data(address);
    for (i=0; i<n; i++)
    {
        write_data(values[i]);
    }
    CS_ON;
    delay_ms(1);
    return 0;
}
/**************************************************************
函数：unsigned char mcp2515_set_registers(const unsigned char address,
	    const unsigned char values[], const unsigned char n)
功能：设置CAN ID和数据
**************************************************************/
/**************************modify register***************************/
unsigned char mcp2515_modify_register(const unsigned char address,const unsigned char mask, const unsigned char data)
{
    CS_OFF;
    delay_ms(1);
    write_data(MCP_BITMOD);
    write_data(address);
    write_data(mask);
    write_data(data);
    CS_ON;
    delay_ms(1);
    return 0;
}
/**************************************************************
函数：unsigned char mcp2515_set_registers(const unsigned char address,
	    const unsigned char values[], const unsigned char n)
功能：设置CAN ID和数据
**************************************************************/

unsigned char mcp2515_read_can_id( const unsigned char mcp_addr, unsigned char* ext, unsigned int* can_id )
{
    unsigned char tbufdata[4];
    *ext = 0;
    *can_id = 0;
    mcp2515_read_registers( mcp_addr, tbufdata, 4 );
    *can_id = (tbufdata[0]<<3) + (tbufdata[1]>>5);
    return 0;
}
/**************************************************************
函数：unsigned char mcp2515_read_can_msg( const unsigned char buffer_sidh_addr,CAN_MESSAGE* msg)
功能：读取CAN数据
**************************************************************/
unsigned char mcp2515_read_can_msg( const unsigned char buffer_sidh_addr,CAN_MESSAGE* msg)
{
    unsigned char mcp_addr, ctrl;
    mcp_addr = buffer_sidh_addr;
    mcp2515_read_can_id( mcp_addr, &(msg->extended_identifier),&(msg->identifier) );
    ctrl = mcp2515_read_register( mcp_addr-1 );
    msg->dlc = (mcp2515_read_register( mcp_addr+4 )&0x0f);
    if ((msg->dlc & 0x40) || (ctrl & 0x08))
    {
        msg->rtr = 1;
    }
    else
    {
        msg->rtr = 0;
    }
    msg->dlc &= 0x0F;
    mcp2515_read_registers(mcp_addr+5,&(msg->dta[0]),msg->dlc) ;
//msg->dta[0]=mcp2515_read_register( mcp_addr+5);
//msg->dta[1]=mcp2515_read_register( mcp_addr+6);
//msg->dta[2]=mcp2515_read_register( mcp_addr+7);
//msg->dta[3]=mcp2515_read_register( mcp_addr+8);
//msg->dta[4]=mcp2515_read_register( mcp_addr+9);
//msg->dta[5]=mcp2515_read_register( mcp_addr+10);
//msg->dta[6]=mcp2515_read_register( mcp_addr+11);
//msg->dta[7]=mcp2515_read_register( mcp_addr+12);
    return 0;
}
/**************************************************************
函数：unsigned char mcp2515_write_can_id( const unsigned char mcp_addr,const unsigned char ext, const unsigned int can_id)
功能：写ID 范围0-1FFFFFFF
**************************************************************/
unsigned char mcp2515_write_can_id( const unsigned char mcp_addr,const unsigned char ext, const unsigned int can_id)
{
    unsigned char tbufdata[4];
    tbufdata[0] = (unsigned char)(can_id>>3);
    tbufdata[1] = (unsigned char)(can_id<<5);
    tbufdata[2] = 0;
    tbufdata[3] = 0;
    mcp2515_set_registers( mcp_addr, tbufdata, 4 );	 //mcp_addr start from 31H;
    return 0;
}
/**************************************************************
函数：unsigned char mcp2515_write_can_msg( const unsigned char buffer_sidh_addr,const CAN_MESSAGE* msg)
功能：写数据
**************************************************************/
unsigned char mcp2515_write_can_msg( const unsigned char buffer_sidh_addr,const CAN_MESSAGE* msg)
{
    unsigned char mcp_addr, dlc;
    mcp_addr = buffer_sidh_addr;   //id register=31H
    dlc = msg->dlc;
    mcp2515_write_can_id( mcp_addr, msg->extended_identifier,msg->identifier );
    mcp2515_set_register( (mcp_addr+4),(0x0F&dlc));  // write the RTR and DLC	,RTR=0;
    mcp2515_set_registers((mcp_addr+5), &(msg->dta[0]), dlc );  // write data bytes
    return 0;
}
/**************************************************************
函数：unsigned char mcp2515_get_free_txbuf(unsigned char *txbuf_n)
功能：TXB0CTRL=30H, TXB1CTRL=31H,TXB2CTRL=32H;
**************************************************************/
unsigned char mcp2515_get_free_txbuf(unsigned char *txbuf_n)
{
    unsigned char res, i, ctrlval;
    unsigned char ctrlregs[3] = { MCP_TXB0CTRL, MCP_TXB1CTRL, MCP_TXB2CTRL };
    res = 2;
    *txbuf_n = 0x00;
    for (i=0; i<3; i++)
    {
        ctrlval = mcp2515_read_register( ctrlregs[i] );
        if( (ctrlval & 0x08) == 0 )
        {
            *txbuf_n = ctrlregs[i]+1;
            res = 1;
            return res;
        }
    }
    return res;
}
/**************************************************************
函数：unsigned char mcp2515_init_can_buffers(void)
功能：
**************************************************************/
unsigned char mcp2515_init_can_buffers(void)
{
    unsigned char i, a1, a2, a3;
    mcp2515_write_can_id(MCP_RXM0SIDH, 0, 0);
    mcp2515_write_can_id(MCP_RXM1SIDH, 0, 0);
    mcp2515_write_can_id(MCP_RXF0SIDH, 1, 0);
    mcp2515_write_can_id(MCP_RXF1SIDH, 0, 0);
    mcp2515_write_can_id(MCP_RXF2SIDH, 1, 0);
    mcp2515_write_can_id(MCP_RXF3SIDH, 0, 0);
    mcp2515_write_can_id(MCP_RXF4SIDH, 0, 0);
    mcp2515_write_can_id(MCP_RXF5SIDH, 0, 0);
    a1 = MCP_TXB0CTRL;
    a2 = MCP_TXB1CTRL;
    a3 = MCP_TXB2CTRL;
    for (i = 0; i < 14; i++)
    {
        mcp2515_set_register(a1, 0);
        mcp2515_set_register(a2, 0);
        mcp2515_set_register(a3, 0);
        a1++;
        a2++;
        a3++;
    }

    mcp2515_set_register(MCP_RXB0CTRL, 0);
    mcp2515_set_register(MCP_RXB1CTRL, 0);
    return 0;
}
/**************************************************************
函数：unsigned char mcp2515_rts(void)
功能：
**************************************************************/
unsigned char mcp2515_rts(void)
{
    CS_OFF;
    delay_ms(1);
    write_data(0x81);
    CS_ON;
    delay_ms(1);
    return 0;
}
/**************************************************************
函数：unsigned char mcp2515_start_transmit()
功能：CAN发送
**************************************************************/
unsigned char mcp2515_start_transmit()
{
    mcp2515_modify_register( 0x30 , 0x08, 0x08 );
    return 0;
}
/**************************************************************
函数：unsigned char mcp2515_filter_intialize(void)
功能：CAN发送
**************************************************************/
unsigned char mcp2515_filter_intialize(void)
{
    unsigned char buf[10];
    mcp2515_set_register(MCP_RXF0SIDH,0x05);
    mcp2515_set_register(MCP_RXF0SIDL,0x00);

    mcp2515_set_register(MCP_RXM0SIDH,0xFF);
    mcp2515_set_register(MCP_RXM0SIDL,0xE0);
    return 0;
}

#endif
