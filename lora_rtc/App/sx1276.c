
#include "sx1276.h"
#include "stm8l15x_spi.h"
#include "bsp.h"
//#include "sx1276-lora.c"
//#include "sx1276-fsk.c"

T_SX1276 SX1276;

INT8U SPI_ExchangeByte(INT8U input)
{
    SPI_SendData(SPI1, input);
    while (RESET == SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE));   // 等待数据传输完成
    while( SPI_GetFlagStatus(SPI1, SPI_FLAG_BSY) == SET );
    //while (RESET == SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE));  // 等待数据接收完成
    return (SPI_ReceiveData(SPI1));
}

extern void DelayMs(INT8U x);
/*
===============================================================================
*Function:  SX1276_Par_Error( ) => Parameter check error
*Input   :  file, witch file
            line, witch line
*Output  :  None
===============================================================================
*/
void SX1276_Par_Error(uint8_t* file, uint32_t line)
{
    while( 1 )
    {
        file = file;
        line = line;
    }
}
/*
===============================================================================
*Function:  SX1276_Init( ) => Initialize the SX2376, Lora or FSK mode
*Input   :  None
*Output  :  None
===============================================================================
*/
INT8U btmp;
void SX1276_Init( RANGE_MODE mode )
{
	

    SX1276_Reset( );
	
	btmp=SX1276_ReadReg( REG_LR_VERSION ) ;

    do
    {
        SX1276_WriteReg( REG_OPMODE, 0x08 );
//        WDT_Fed();
    } while (SX1276_ReadReg( REG_OPMODE ) != 0x08 );

    if( ( mode != MODE_FSK ) && ( mode != MODE_LORA ) )     { mode = MODE_LORA; }
    SX1276.Mode_Lora_Fsk = mode;

    if( SX1276.Mode_Lora_Fsk == MODE_LORA )
    {
        SX1276_LoRa_Init( );
//        SX1276_LoRa_SetImplicitHeaderEn( IMPLICITHEADER_OFF );
//        SX1276_LoRa_SetSymbTimeout( 0x3FF );
    }
    else
    {
        SX1276_Fsk_Init( );
    }
}
/*
===============================================================================
*Function:  SX1276_WriteReg( ) => Write a register of the device
*Input   :  addr, the address of the register
            value, the value to be written
*Output  :  None
===============================================================================
*/
void SX1276_WriteReg( INT8U addr, INT8U value )
{
    SX1276_NSS_LOW( );
    SPI_ExchangeByte( addr | 0x80 );
    SPI_ExchangeByte( value );
    SX1276_NSS_HIGH( );
}
/*
===============================================================================
*Function:  SX1276_WriteBuffer( ) => Write  registers of the device
*Input   :  addr, the  first address of the registers
            *buffer, the values to be written
            size, how many bytes should be written
*Output  :  None
===============================================================================
*/
void SX1276_WriteBuffer( INT8U addr, INT8U *buffer, INT8U size )
{
	INT8U i;
	SX1276_NSS_LOW( );
	SPI_ExchangeByte( addr | 0x80 );
	for( i=0; i<size; i++ )
	{
		SPI_ExchangeByte( buffer[i] );
	}
	SX1276_NSS_HIGH( );
}
/*
===============================================================================
*Function:  SX1276_WriteFifo( ) => Write data into fifo
*Input   :  buffer, the buffer stores data
            size, how many bytes should be written
*Output  :  None
===============================================================================
*/
void SX1276_WriteFifo( INT8U *buffer, INT8U size )
{
	INT8U i;
	SX1276_NSS_LOW( );
    SPI_ExchangeByte( 0x00 | 0x80 );
	for( i=0; i<size; i++ )
	{
		SPI_ExchangeByte( buffer[i] );
	}
	SX1276_NSS_HIGH( );
}
/*
===============================================================================
*Function:  SX1276_ReadReg( ) => Read a register of the device
*Input   :  addr, the address of the register
*Output  :  the read value
===============================================================================
*/
INT8U SX1276_ReadReg( INT8U addr )
{
    INT8U btmp;
    SX1276_NSS_LOW( );
    SPI_ExchangeByte( addr & 0x7F );
	btmp = SPI_ExchangeByte( 0 );
    SX1276_NSS_HIGH( );
    return btmp;
}
/*
===============================================================================
*Function:  SX1276_ReadFifo( ) =>Read a byte from FIFO
*Input   :  None
*Output  :  The read value
===============================================================================
*/
INT8U SX1276_ReadFifo( void )
{
    INT8U btmp;
    SX1276_NSS_LOW( );
    SPI_ExchangeByte( 0x00 & 0x7F );
    btmp = SPI_ExchangeByte( 0 );
    SX1276_NSS_HIGH( );
    return btmp;
}
/*
===============================================================================
*Function:  SX1276_SetMode( ) => Put the SX1276 into another mode
*Input   :  range_mode,
        	MODE_LORA               =   0x01,       // LoRa mode
    		MODE_FSK                =   0x00,       //FSK/OOK mode

*Output  :  None
===============================================================================
*/
void SX1276_SetMode( RANGE_MODE range_mode )
{
	INT8U tmp;
	SX1276_LoRa_SetMode( LORA_MODE_SLEEP );  //SX1276_Fsk_SetMode( FSK_MODE_SLEEP );
	tmp = SX1276_ReadReg( 0x01 );
    tmp &= ~( 1<<7 );
//	tmp &= ~( ( 1<<7 ) | ( 1<<6 ) | ( 1<<5 ) );
	tmp |= ( range_mode << 7 );
	SX1276_WriteReg( 0x01, tmp );
}
/*
===============================================================================
*Function: SX1276_Reset( ) => Reset the SX1276
*Input   :  None
*Output  :  None
===============================================================================
*/
void SX1276_Reset( void )
{
//    SX1276.Channel = 0xFF;          //initialized : Invalid channel
//    SX1276.PowerIndex = 0xFF;       //initialized : power index value channel
	SX1276_RESET_LOW( );
	DelayMs( 1 );
	SX1276_RESET_HIGH( );
	DelayMs( 6 );
}

void SX1276_PowerDown( void )
{
//    SX1276.Channel = 0xFF;          //initialized : Invalid channel
//    SX1276.PowerIndex = 0xFF;       //initialized : power index value channel
	SX1276_RESET_LOW( );
	//DelayMs( 1 );
	//SX1276_RESET_HIGH( );
	//DelayMs( 6 );
}
/*
===============================================================================
*Function: SX1276_SetSwitchTx( ) => Set SwitchTx  on
*Input   :  None
*Output  :  None
===============================================================================
*/
void SX1276_SetSwitchTx( void )
{
//#if MAX_POWER > 20
//	SX1276_CTRL1_LOW( );
//	SX1276_CTRL2_HIGH( );
//#else
//    SX1276_CTRL1_HIGH( );
//	SX1276_CTRL2_LOW( );
//#endif
	SX1276_TX_CTRL_HIGH( );
	SX1276_RX_CTRL_LOW( );
	DelayMs( 3 );
}
/*
===============================================================================
*Function: SX1276_SetSwitchRx( ) => Set SwitchRx  on
*Input   :  None
*Output  :  None
===============================================================================
*/
void SX1276_SetSwitchRx( void )
{
    SX1276_TX_CTRL_LOW( );
    SX1276_RX_CTRL_HIGH( );
    DelayMs( 1 );
}
/*
===============================================================================
*Function:  SX1276_SetFreq( ) => Set the Frequency of SX1276, TCXO = 26MHz, Frf = 2(19)*fRF/26
*Input   :  FreqIndex, 0-31, 410 - 441
*Output  :  None
===============================================================================
*/
static const INT8U Freq_Table[][3] =
{
    { 0x66, 0x80, 0x00 },          //410,  0
    { 0x66, 0xC0, 0x00 },          //411,  1
    { 0x67, 0x00, 0x00 },          //412,  2
    { 0x67, 0x40, 0x00 },          //413,  3
    { 0x67, 0x80, 0x00 },          //414,  4
    { 0x67, 0xC0, 0x00 },          //415,  5
    { 0x68, 0x00, 0x00 },          //416,  6
    { 0x68, 0x40, 0x00 },          //417,  7
    { 0x68, 0x80, 0x00 },          //418,  8
    { 0x68, 0xC0, 0x00 },          //419,  9
    { 0x69, 0x00, 0x00 },          //420,  10
    { 0x69, 0x40, 0x00 },          //421,  11
    { 0x69, 0x80, 0x00 },          //422,  12
    { 0x69, 0xC0, 0x00 },          //423,  13
    { 0x6A, 0x00, 0x00 },          //424,  14
    { 0x6A, 0x40, 0x00 },          //425,  15
    { 0x6A, 0x80, 0x00 },          //426,  16
    { 0x6A, 0xC0, 0x00 },          //427,  17
    { 0x6B, 0x00, 0x00 },          //428,  18
    { 0x6B, 0x40, 0x00 },          //429,  19
    { 0x6B, 0x80, 0x00 },          //430,  20
    { 0x6B, 0xC0, 0x00 },          //431,  21
    { 0x6C, 0x00, 0x00 },          //432,  22
    { 0x6C, 0x40, 0x00 },          //433,  23
    { 0x6C, 0x80, 0x00 },          //434,  24
    { 0x6C, 0xC0, 0x00 },          //435,  25
    { 0x6D, 0x00, 0x00 },          //436,  26
    { 0x6D, 0x40, 0x00 },          //437,  27
    { 0x6D, 0x80, 0x00 },          //438,  28
    { 0x6D, 0xC0, 0x00 },          //439,  29
    { 0x6E, 0x00, 0x00 },          //440,  30
    { 0x6E, 0x40, 0x00 }           //441,  31
};
/*
===============================================================================
*Function:  SX1276_SetFreq( ) => Set the Frequency of SX1276
*Input   :  FreqIndex, 0-31, 470 - 501
*Output  :  None
===============================================================================
*/
//static const INT8U Freq_Table[][3] =
//{
//    { 0x75, 0x80, 0x00 },          //470,  0
//    { 0x75, 0xC0, 0x00 },          //471,  1
//    { 0x76, 0x00, 0x00 },          //472,  2
//    { 0x76, 0x40, 0x00 },          //473,  3
//    { 0x76, 0x80, 0x00 },          //474,  4
//    { 0x76, 0xC0, 0x00 },          //475,  5
//    { 0x77, 0x00, 0x00 },          //476,  6
//    { 0x77, 0x40, 0x00 },          //477,  7
//    { 0x77, 0x80, 0x00 },          //478,  8
//    { 0x77, 0xC0, 0x00 },          //479,  9
//    { 0x78, 0x00, 0x00 },          //480,  10
//    { 0x78, 0x40, 0x00 },          //481,  11
//    { 0x78, 0x80, 0x00 },          //482,  12
//    { 0x78, 0xC0, 0x00 },          //483,  13
//    { 0x79, 0x00, 0x00 },          //484,  14
//    { 0x79, 0x40, 0x00 },          //485,  15
//    { 0x79, 0x80, 0x00 },          //486,  16
//    { 0x79, 0xC0, 0x00 },          //487,  17
//    { 0x7A, 0x00, 0x00 },          //488,  18
//    { 0x7A, 0x40, 0x00 },          //489,  19
//    { 0x7A, 0x80, 0x00 },          //490,  20
//    { 0x7A, 0xC0, 0x00 },          //491,  21
//    { 0x7B, 0x00, 0x00 },          //492,  22
//    { 0x7B, 0x40, 0x00 },          //493,  23
//    { 0x7B, 0x80, 0x00 },          //494,  24
//    { 0x7B, 0xC0, 0x00 },          //495,  25
//    { 0x7C, 0x00, 0x00 },          //496,  26
//    { 0x7C, 0x40, 0x00 },          //497,  27
//    { 0x7C, 0x80, 0x00 },          //498,  28
//    { 0x7C, 0xC0, 0x00 },          //499,  29
//    { 0x7D, 0x00, 0x00 },          //500,  30
//    { 0x7D, 0x40, 0x00 }           //501,  31
//};
void SX1276_SetFreq( INT8U FreqIndex )
{
    FreqIndex = ( FreqIndex <= 31 ) ? FreqIndex : 31;
//    if( FreqIndex == SX1276.Channel )   { return; }

    SX1276_LoRa_SetMode( LORA_MODE_STDBY );
//    SX1276.Channel = FreqIndex;
    SX1276_WriteReg( 6,  Freq_Table[FreqIndex][0] );
    SX1276_WriteReg( 7,  Freq_Table[FreqIndex][1] );
    SX1276_WriteReg( 8,  Freq_Table[FreqIndex][2] );
}
/*
===============================================================================
*Function:  SX1276_SetDatarate( ) => Set the datarate of the device
*Input   :  rate_index,  datarate index
*Output  :  None
===============================================================================
*/
void SX1276_SetDatarate( INT8U rate_index )
{
    if( ( SX1276.Mode_Lora_Fsk != MODE_FSK ) && ( SX1276.Mode_Lora_Fsk != MODE_LORA ) )
    { SX1276.Mode_Lora_Fsk = MODE_LORA; }

    if( SX1276.Mode_Lora_Fsk == MODE_LORA )
    {
        SX1276_LoRa_SetDataRate( rate_index );
    }
    else
    {
        SX1276_Fsk_SetDataRate( rate_index );
    }
}
/*
===============================================================================
*Function:  SX1276_SleepMode( ) => Put the device into sleep mode
*Input   :  None
*Output  :  None
===============================================================================
*/
void SX1276_SleepMode( void )
{
        INT8U tmp;
	tmp = SX1276_ReadReg( REG_OPMODE );
	tmp &= ~( ( 1<<2 ) | ( 1<<1 ) | ( 1<<0 ) );
	SX1276_WriteReg( REG_OPMODE, tmp );
	SX1276_TX_CTRL_LOW( );
	SX1276_RX_CTRL_LOW( );

}
/*
===============================================================================
*Function:  SX1276_SetRxMode( ) => Set the device into RX mode
*Input   :  None
*Output  :  None
===============================================================================
*/
INT8U  status_reg;
void SX1276_SetRxMode( void )
{
    if( ( SX1276.Mode_Lora_Fsk != MODE_FSK ) && ( SX1276.Mode_Lora_Fsk != MODE_LORA ) )
    { SX1276.Mode_Lora_Fsk = MODE_LORA; }

    if( SX1276.Mode_Lora_Fsk == MODE_LORA )
    {
     
        SX1276_LoRa_StartRx( );
        while((status_reg &0x04)!=0x04)
         status_reg=SX1276_ReadReg(REG_LR_MODEMSTAT ); 
    }
    else
    {
        SX1276_Fsk_StartRx( );
    }
//    while( !( SX1276_ReadReg( 0x18 ) & ( 1<<3 ) ) );
}
/*
===============================================================================
*Function:  SX1276_ReceivePacket( ) => Receive a packet
*Input   :  buffer,  Buffer to store the array
*Output  :  received size
===============================================================================
*/
INT8U SX1276_ReceivePacket( INT8U *buffer )
{
    INT8U RxLength = 0;
    if( SX1276.Mode_Lora_Fsk == MODE_LORA )
    {
        if( SX1276_LoRa_RxProcess( ) )
        {
            RxLength = SX1276_LoRa_GetRxPacket( buffer );
        }
    }
    else
    {
        if( SX1276_Fsk_RxProcess( ) )
        {
            RxLength = SX1276_Fsk_GetRxPacket( buffer );
        }
    }
    return RxLength;
}
/*
===============================================================================
*Function:  SX1276_SendPacket( ) => Send a packet
*Input   :  buffer,  Buffer to store the array
            size, how many bytes should be sent
*Output  :  None
===============================================================================
*/
void SX1276_SendPacket( INT8U *buffer, INT8U size )
{
    if( ( SX1276.Mode_Lora_Fsk != MODE_FSK ) && ( SX1276.Mode_Lora_Fsk != MODE_LORA ) )
    { SX1276.Mode_Lora_Fsk = MODE_LORA; }

    if( SX1276.Mode_Lora_Fsk == MODE_LORA )
    {
        SX1276_LoRa_SendPacket( buffer, size );
    }
    else
    {
        SX1276_Fsk_SendPacket_Var( buffer, size );
    }
}
/*
===============================================================================
*Function:  SX1276_SetPreambleSize( ) => Set the preamble count of the device
*Input   :  preamble_cnt, the count of the preamble size
*Output  :  None
===============================================================================
*/
void SX1276_SetPreambleSize( INT16U preamble_cnt )
{
    if( ( SX1276.Mode_Lora_Fsk != MODE_FSK ) && ( SX1276.Mode_Lora_Fsk != MODE_LORA ) )
    { SX1276.Mode_Lora_Fsk = MODE_LORA; }


    if( SX1276.Mode_Lora_Fsk == MODE_LORA )
    {
        SX1276_Lora_SetPreambleSize( preamble_cnt );
    }
    else
    {
        SX1276_Fsk_SetPreambleSize( preamble_cnt );
    }
}
/*
===============================================================================
*Function:  SX1276_CADMode( ) => Put the device into Lora CAD mode
*Input   :  None
*Output  :  None
===============================================================================
*/
void SX1276_CADMode( void )
{
	SX1276_LoRa_SetMode( LORA_MODE_STDBY );
        SX1276_WriteReg( REG_LR_IRQFLAGSMASK, DEF_LR_IRQFLAGSMASK | RFLR_IRQFLAGS_RXTIMEOUT
															  | RFLR_IRQFLAGS_RXDONE
															  | RFLR_IRQFLAGS_PAYLOADCRCERROR
															  | RFLR_IRQFLAGS_VALIDHEADER
															  | RFLR_IRQFLAGS_TXDONE
															  //| RFLR_IRQFLAGS_CADDONE
															  | RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL
															  //| RFLR_IRQFLAGS_CADDETECTED
															  );
	SX1276_LoRa_SetFreqHoppingPeriod( 0 );  //By the FreqHopping is disabled

	SX1276_WriteReg( REG_LR_DIOMAPPING1, DEF_LR_DIOMAPPING1 | RFLR_DIOMAPPING1_DIO0_00  //RxDone
	                              							| RFLR_DIOMAPPING1_DIO1_00  //RxTimeout
	                              							| RFLR_DIOMAPPING1_DIO2_00  //FhssChangeChannel
	                              							| RFLR_DIOMAPPING1_DIO3_00 );  //CadDone
	SX1276_WriteReg( REG_LR_DIOMAPPING2, DEF_LR_DIOMAPPING2 | RFLR_DIOMAPPING2_DIO4_00  //CadDetected
	                                                        | RFLR_DIOMAPPING2_DIO5_00 );  //Mode Ready

    SX1276_SetSwitchRx( );
    SX1276_LoRa_SetMode( LORA_MODE_CAD );
}

///*
//===============================================================================
//*Function:  SX1276_SetPower( ) => Set the output power( 0-15, power + 5 )
//*Input   :  power,  0-15, 5dbm   power +
//*Output  :  None
//===============================================================================
//*/
//void SX1276_SetPower( INT8U power )
//{
//	INT8U tmp;
//    SX1276.PowerIndex = ( power <= 15 ) ? power : 15;
//    power = SX1276.PowerIndex;
//
//    SX1276_LoRa_SetMode( LORA_MODE_STDBY );
//    tmp = SX1276_ReadReg( 0x4D );
//    tmp &= ~( ( 1<<2 ) | ( 1<<1 ) | ( 1<<0 ) );
//    tmp |= ( 0x07 << 0 );
//    SX1276_WriteReg( 0x4D, tmp );
//    tmp = SX1276_ReadReg( 0x09 );
//    tmp |= ( 1<<7 );
//    tmp &= ~( ( 1<<3 ) | ( 1<<2 ) | ( 1<<1 ) | ( 1<<0 ) );
//    tmp |= ( power << 0 );
//    SX1276_WriteReg( 0x09, tmp );
//}
/*
===============================================================================
*Function:  SX1276_SetPower( ) => Set the output power( 0-15, power + 5 )
*Input   :  power,  0-15,    pin, 0:PA_RFO, 1:PA_BOSST
*Output  :  None
===============================================================================
*/
void SX1276_SetPower(INT8U power, INT8U pin)
{
	INT8U tmp;
	if(0==pin)		//power
	{
//	    SX1276.PowerIndex = ( power <= 15 ) ? power : 15;
//	    power = SX1276.PowerIndex;

	    SX1276_LoRa_SetMode( LORA_MODE_STDBY );
	    tmp = SX1276_ReadReg( 0x4D );
	    tmp &= ~( ( 1<<2 ) | ( 1<<1 ) | ( 1<<0 ) );
	    tmp |= ( 0x04 << 0 );
	    SX1276_WriteReg( 0x4D, tmp );
	    tmp = SX1276_ReadReg( 0x09 );
	    tmp &= ~( 1<<7 );
	    tmp |= ( ( 1<<6 ) | ( 1<<5 ) );
	    tmp &= ~( ( 1<<3 ) | ( 1<<2 ) | ( 1<<1 ) | ( 1<<0 ) );
	    tmp |= ( power << 0 );
	    SX1276_WriteReg( 0x09, tmp );
	}
	else		//power+5
	{
		SX1276_LoRa_SetMode( LORA_MODE_STDBY );
	    tmp = SX1276_ReadReg( 0x4D );
	    tmp &= ~( ( 1<<2 ) | ( 1<<1 ) | ( 1<<0 ) );
	    tmp |= ( 0x07 << 0 );
	    SX1276_WriteReg( 0x4D, tmp );
	    tmp = SX1276_ReadReg( 0x09 );
	    tmp |= ( 1<<7 );
	    tmp &= ~( ( 1<<3 ) | ( 1<<2 ) | ( 1<<1 ) | ( 1<<0 ) );
	    tmp |= ( power << 0 );
	    SX1276_WriteReg( 0x09, tmp );
	}
}

/*
===============================================================================
*Function:  SX1276_SetPARampTime( ) => Set the PA ramp time of SX1276
*Input   :  bw,
			0      3.4 ms
			1      2 ms
			2      1 ms
			3      500 us
			4      250 us
			5      125 us
			6      100 us
			7      62 us
			8      50 us
			9      40 us
			10     31 us
			11     25 us
			12     20 us
			13     15 us
			14     12 us
			15     10 us
*Output  :  None
===============================================================================
*/
void SX1276_SetPARampTime( INT8U rametime )
{
	INT8U tmp;
	rametime = ( rametime <= 15 ) ?  rametime : 15;

	tmp = SX1276_ReadReg( 0x0A );
	tmp &= ~( ( 1<<3 ) | ( 1<<2 ) | ( 1<<1 ) | ( 1<<0 ) );
	tmp |= ( rametime << 0 );
	SX1276_WriteReg( 0x0A, tmp );
}


