
#include "sx1276-LoRa.h"
#include "stm8l15x_tim3.h"
#include "stm8l15x_it.h"

#define RF_MAX_PACKET_SIZE     128      // 发送数据每包最大长度
#define AddrLow1 00
#define AddrLow2 01
#define AddrHigh1 02
#define AddrHigh2 03
/*
===============================================================================
*Function:  SX1276_LoRa_SetReg( ) => Set the AccessSharedReg 0x0D : 0x3F
*Input   :  access_reg,
        LORA_REG                     =   0x00,       //Access LoRa registers page 0x0D: 0x3F
		FSK_REG                      =   0x01,       //Access FSK registers page (in mode LoRa) 0x0D: 0x3F
*Output  :  None
===============================================================================
*/
void SX1276_LoRa_SetReg( ACCESS_REG access_reg )
{
    INT8U tmp;
    tmp = SX1276_ReadReg( REG_LR_OPMODE );
    tmp &= ~( 1<<6 );
    tmp |= ( access_reg << 6 );
    SX1276_WriteReg( REG_LR_OPMODE, tmp );
}
/*
===============================================================================
*Function:  SX1276_LoRa_SetMode( ) => Put the SX1276 into another work mode
*Input   :  lora_chip_mode,
        LORA_MODE_SLEEP              =   0x00,       //sleep mode
	    LORA_MODE_STDBY              =   0x01,       //standby mode
	    LORA_MODE_FSTX               =   0x02,       //frequency synthesis tx mode
	    LORA_MODE_TX                 =   0x03,       //transmit mode
	    LORA_MODE_FSRX               =   0x04,       //frequency synthesis rx mode
	    LORA_MODE_RXC                =   0x05,       //receive continuous mode
	    LORA_MODE_RXS                =   0x06,       //receive single mode
	    LORA_MODE_CAD                =   0x07,       //channel activity dection mode
*Output  :  None
===============================================================================
*/
void SX1276_LoRa_SetMode( LORA_CHIP_MODE lora_chip_mode )
{
	INT8U tmp;
	tmp = SX1276_ReadReg( REG_LR_OPMODE );
	tmp &= ~( ( 1<<2) | ( 1<<1 ) | ( 1<<0 ) );
	tmp |= ( lora_chip_mode << 0 );
	SX1276_WriteReg( REG_LR_OPMODE, tmp );
}
/*
===============================================================================
*Function:  SX1276_LoRa_SetBW( ) => Set the new Signal Bandwidth value
*Input   :  bw,
			0   7.8 kHz
			1   10.4 kHz
			2   15.6 kHz
			3   20.8kHz
			4   31.25 kHz
			5   41.7 kHz
			6   62.5 kHz
			7   125 kHz
			8   250 kHz
			9   500 kHz
*Output  :  None
===============================================================================
*/
void SX1276_LoRa_SetBW( INT8U bw )
{
	INT8U tmp;
	bw = ( bw <= 9 ) ? bw : 9;
	tmp = SX1276_ReadReg( REG_LR_MODEMCONFIG1 );
	tmp &= ~( ( 1<<7 ) | ( 1<<6 ) | ( 1<<5 ) | ( 1<<4 ) );
	tmp |= ( bw << 4 );
	SX1276_WriteReg( REG_LR_MODEMCONFIG1, tmp );
}
/*
===============================================================================
*Function:  SX1276_LoRa_SetSpreadingFactor( ) => Set the new Spreading Factor value
*Input   :  sf,
			6    64 chips / symbol
			7    128 chips / symbol
			8    256 chips / symbol
			9    512 chips / symbol
			10   1024 chips / symbol
			11   2048 chips / symbol
			12   4096 chips / symbol
*Output  :  None
===============================================================================
*/
void SX1276_LoRa_SetSpreadingFactor( INT8U sf )
{
	INT8U tmp;
	if( ( sf >= 6 ) && ( sf <= 12 ) )
	{
		tmp = SX1276_ReadReg( REG_LR_MODEMCONFIG2 );
		tmp &= ~( ( 1<<7 ) | ( 1<<6 ) | ( 1<<5 ) | ( 1<<4 ) );
		tmp |= ( sf << 4 );
		SX1276_WriteReg( REG_LR_MODEMCONFIG2, tmp );
	}
}
/*
===============================================================================
*Function:  SX1276_LoRa_SetErrorCoding( ) => Set the new Error Coding value
*Input   :  ec,
			1    4/5
			2    4/6
			3    4/7
			4    4/8
*Output  :  None
===============================================================================
*/
void SX1276_LoRa_SetErrorCoding( INT8U ec )
{
	INT8U tmp;
	if( ( ec >= 1 ) && ( ec <= 4 ) )
	{
		tmp = SX1276_ReadReg( REG_LR_MODEMCONFIG1 );
		tmp &= ~( ( 1<<3 ) | ( 1<<2 ) | ( 1<<1 ) );
		tmp |= ( ec << 1 );
		SX1276_WriteReg( REG_LR_MODEMCONFIG1, tmp );
	}
}
/*
===============================================================================
*Function:  SX1276_LoRa_SetDataRate( ) => Set the Datarate of SX1276
*Input   :  INT8U rate, 0-4
*Output  :  None
===============================================================================
*/
static const INT8U LoRa_DataRate_Table[6][3] =
{
	{ 7, 12, 1 },   //0.3kbps,       BW: 125KHz, Spreading Factor: 12, Error Coding value: 4/5,
	{ 8, 11, 1 },   //1.2kbps,		 BW: 250KHz, Spreading Factor: 11, Error Coding value: 4/5,
	{ 9, 11, 1 },   //2.4kbps, 	     BW: 500KHz, Spreading Factor: 11, Error Coding value: 4/5,
	{ 8, 8, 2 },    //4.8kbps, 		 BW: 250KHz, Spreading Factor: 8,  Error Coding value: 4/6,
	{ 9, 8, 2 },    //9.6kbps, 		 BW: 500KHz, Spreading Factor: 8,  Error Coding value: 4/6,
	{ 9, 7, 2 }     //19.2kbps       BW: 500KHz, Spreading Factor: 7, Error Coding value: 4/6,
};
void SX1276_LoRa_SetDataRate( INT8U rate )
{
    INT8U tmp;
    rate = rate <= 5 ? rate : 5;
    SX1276_LoRa_SetMode( LORA_MODE_STDBY );
    if( ( rate == 2 ) || ( rate == 3 ) )
    {
        SX1276_WriteReg( REG_LR_MODEMCONFIG3, DEF_LR_MODEMCONFIG3 );  //Regs 0x26, Low Data Rata Optimization Disabled
    }
    else
	{
		SX1276_WriteReg( REG_LR_MODEMCONFIG3, 0x08 );  //Regs 0x26, Low Data Rata Optimization Enabled
    }
    if( ( rate == 2 ) || ( rate == 4 ) || ( rate == 5 ) )
    {
        SX1276_WriteReg( 0x36, 0x02 );
        SX1276_WriteReg( 0x3A, 0x7F ); //Special setting when BW = 9, 500KHz

        tmp = SX1276_ReadReg( 0x31 );
        tmp |= ( 1<<0 );
        SX1276_WriteReg( 0x31, tmp );
    }
    else
    {
        SX1276_WriteReg( 0x36, 0x03 );  //reset setting

        tmp = SX1276_ReadReg( 0x31 );
        tmp &= ~( 1<<0 );
        SX1276_WriteReg( 0x31, tmp );
        SX1276_WriteReg( 0x2F, 0x40 );
        SX1276_WriteReg( 0x30, 0x00 );
    }
	SX1276_LoRa_SetBW( LoRa_DataRate_Table[rate][0] ); //带宽[0: 7.8kHz, 1: 10.4 kHz, 2: 15.6 kHz, 3: 20.8 kHz, 4: 31.2 kHz,5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8:250 kHz, 9: 500 kHz, other: Reserved]
	SX1276_LoRa_SetSpreadingFactor( LoRa_DataRate_Table[rate][1] ); //扩频因子[6:64, 7: 128, 8: 256, 9: 512,10: 1024, 11: 2048, 12:4096  chips]
	SX1276_LoRa_SetErrorCoding( LoRa_DataRate_Table[rate][2] ); //循环纠错码 [1:4/5, 2: 4/6, 3: 4/7, 4: 4/8] 
}
/*
===============================================================================
*Function:  SX1276_LoRa_SetCRCEn( ) => Enables/Disables the packet CRC generation
*Input   :  crc_en,
       CRC_ENABLE                       =   0x01,       //Enables the packet CRC generation
	   CRC_DISABLE                      =   0x00,       //Disables the packet CRC generation
*Output  :  None
===============================================================================
*/
void SX1276_LoRa_SetCRCEn( CRC_EN crc_en )
{
	INT8U tmp;
	tmp = SX1276_ReadReg( REG_LR_MODEMCONFIG2 );
	tmp &= ~( 1<<2 );
	tmp |= ( crc_en << 2 );
	SX1276_WriteReg( REG_LR_MODEMCONFIG2, tmp );
}
/*
===============================================================================
*Function:  SX1276_LoRa_SetImplicitHeaderEn( ) => Enables/Disables the Implicit Header mode in LoRa
*Input   :  implicitheader_en,
       IMPLICITHEADER_ON            =   0x01,       //Enables the Implicit Header mode in LoRa
	   IMPLICITHEADER_OFF           =   0x00,       //Disables the Implicit Header mode in LoRa
*Output  :  None
===============================================================================
*/
void SX1276_LoRa_SetImplicitHeaderEn( IMPLICITHEADER_EN implicitheader_en )
{
	INT8U tmp;
	tmp = SX1276_ReadReg( REG_LR_MODEMCONFIG1 );
	tmp &= ~( 1<<0 );
	tmp |= ( implicitheader_en << 0 );
	SX1276_WriteReg( REG_LR_MODEMCONFIG1, tmp );
}
/*
===============================================================================
*Function:  SX1276_LoRa_GetImplicitHeaderOn( ) => Check if implicit header mode in LoRa in enabled or disabled
*Input   :  None
*Output  :  1, implicit header mode in LoRa in enabled
			0, implicit header mode in LoRa in disabled
===============================================================================
*/
//INT8U SX1276_LoRa_GetImplicitHeaderOn( void )
//{
//	INT8U tmp;
//	tmp = SX1276_ReadReg( REG_LR_MODEMCONFIG1 );
//	tmp &= ( 1<<0 );
//	return tmp;
//}
/*
===============================================================================
*Function:  SX1276_LoRa_SetFreqHoppingPeriod( ) =>  Enables/Disables the frequency hopping
*Input   :  hopping_period,
            0               Disables the frequency hopping
            1 - 255         Enables the frequency hopping and set the hopping period
*Output  :  None
===============================================================================
*/
void SX1276_LoRa_SetFreqHoppingPeriod( INT8U  hopping_period )
{
	SX1276_WriteReg( REG_LR_HOPPERIOD, hopping_period );
}
/*
===============================================================================
*Function:  SX1276_LoRa_SetPayloadLength( ) => Set payload length
*Input   :  payload_length, 0x01 - 0xff
*Output  :  None
===============================================================================
*/
void SX1276_LoRa_SetPayloadLength( INT8U payload_tength )
{
	payload_tength = ( payload_tength >= 1 ) ? payload_tength : 1;
	SX1276_WriteReg( REG_LR_PAYLOADLENGTH, payload_tength );
}
/*
===============================================================================
*Function:  SX1276_LoRa_SetSymbTimeout( ) => Set Symbol Timeout based on symbol length
*Input   :  value, 0x0000 - 0x03FF
*Output  :  None
===============================================================================
*/
void SX1276_LoRa_SetSymbTimeout( INT16U value )
{
	INT8U tmp;
    value = ( value <= 0x03FF ) ? value : 0x03FF;
	tmp = SX1276_ReadReg( REG_LR_MODEMCONFIG2 );
	tmp &= ~( ( 1<<1 ) | ( 1<<0 ) );
	tmp |= ( value >> 8 );
	SX1276_WriteReg( REG_LR_MODEMCONFIG2, tmp );

	SX1276_WriteReg( REG_LR_SYMBTIMEOUTLSB, value );
}
/*
===============================================================================
*Function:  SX1276_LoRa_SetLowDatarateOptimize( ) => Configure the device to optimize low datarate transfers
*Input   :  ldoptimize_en,
       LDOPTIMIZE_ON                =   0x01,       //Enables the low datarate optimization
	   LDOPTIMIZE_OFF               =   0x00,       //Disables the low datarate optimization
*Output  :  None
===============================================================================
*/
void SX1276_LoRa_SetLowDatarateOptimize( LDOPTIMIZE_EN ldoptimize_en)
{
	INT8U tmp;
	tmp = SX1276_ReadReg( REG_LR_MODEMCONFIG3 );
	tmp &= ~( 1<<3 );
	tmp |= ( ldoptimize_en << 3 );
	SX1276_WriteReg( REG_LR_MODEMCONFIG3, tmp );
}
/*
===============================================================================
*Function:  SX1276_LoRa_Init( ) => Initializes the SX1276
*Input   :  None
*Output  :  None
===============================================================================
*/
INT8U RegsTable1[] =           //0x06 -  0x0F
{
	DEF_LR_FRFMSB,
	DEF_LR_FRFMID,
	DEF_LR_FRFLSB,  //6C, 80, 00  freq 434MHz
	DEF_LR_PACONFIG | RFLR_PACONFIG_PASELECT_PABOOST,  //choice PA_BOOST
	DEF_LR_PARAMP,  //the PA ramp time of SX1276 40us
	( DEF_LR_OCP & RFLR_OCP_TRIM_MASK ) | RFLR_OCP_TRIM_240_MA,  //OCP for PA 240mA
	DEF_LR_LNA,  //LNA gain G1
	DEF_LR_FIFOADDRPTR,  //SPI pointer 0x00
	DEF_LR_FIFOTXBASEADDR,  //Txbase pointer 0x80
	DEF_LR_FIFORXBASEADDR   //Rxbase pointer 0x00
};
INT8U RegsTable2[] =          //0x1D -  0x24
{
	DEF_LR_MODEMCONFIG1,  //Disables the Implicit Header mode, Bw125kHz, CRC4/5
	DEF_LR_MODEMCONFIG2 | 0x03,  // CRC off, SymbTimeout(9:8) 11b
	DEF_LR_SYMBTIMEOUTLSB | 0xff,  // SymbTimeout(7:0) 0xff
	DEF_LR_PREAMBLEMSB,
	DEF_LR_PREAMBLELSB,  //8 bytes preamble
	DEF_LR_PAYLOADLENGTH,  //1 bytes payload
	DEF_LR_PAYLOADMAXLENGTH,  //255 bytes
	DEF_LR_HOPPERIOD   //0,Disables the frequency hopping
};
void SX1276_LoRa_Init( void )
{
	SX1276_SetMode( MODE_LORA );
	SX1276_LoRa_SetMode( LORA_MODE_STDBY );
//	SX1276_WriteReg( REG_LR_FIFO, DEF_LR_FIFO );  //Regs 0x00
	SX1276_WriteBuffer( REG_LR_FRFMSB, RegsTable1, 0x0F - 0x06 + 1 );  //Regs 0x06 - 0x0F
    //SX1276_WriteReg( REG_LR_IRQFLAGSMASK, 0x01 );  //Regs 0x11 ,
    SX1276_WriteReg( REG_LR_IRQFLAGSMASK, 0xFF );  //Regs 0x11 ,
	SX1276_WriteBuffer( REG_LR_MODEMCONFIG1, RegsTable2, 0x24 - 0x1D + 1 );  //Regs 0x1D - 0x24
//	SX1276_WriteReg( REG_LR_MODEMCONFIG3, DEF_LR_MODEMCONFIG3 );  //Regs 0x26, Low Data Rata Optimization Disabled
    SX1276_WriteReg( REG_LR_MODEMCONFIG3, 0x08 );  //Regs 0x26, Low Data Rata Optimization Enabled
	SX1276_WriteReg( REG_LR_DIOMAPPING1, DEF_LR_DIOMAPPING1 );  //Regs 0x40
	SX1276_WriteReg( REG_LR_DIOMAPPING2, DEF_LR_DIOMAPPING2 );  //Regs 0x41
	SX1276_WriteReg( REG_LR_PADAC, DEF_LR_PADAC | RFLR_PADAC_20DBM_ON );  //Regs 0x4D
    SX1276_WriteReg( 0x70, 0x10 );
}
/*
===============================================================================
*Function:  SX1276_LoRa_ClearFlag( ) => Clear the flag
*Input   :  flag_clear,
        FLAG_RXTIMEOUT               =   0x07,       //Timeout interrupt
	    FLAG_RXDONE                  =   0x06,       //Packet reception complete interrupt
	    FLAG_PAYLOADCRCERROR         =   0x05,       //Payload CRC error interrupt
	    FLAG_VALIDHEADER             =   0x04,       //Valid header received in Rx
	    FLAG_TXDONE                  =   0x03,       //FIFO Payload transmission complete interrupt
	    FLAG_CADDONE                 =   0x02,       //CAD complete
	    FLAG_FHSSCHANGECHANNEL       =   0x01,       //FHSS change channel interrupt
	    FLAG_CADDETECTED             =   0x00,       //Valid Lora signal detected during CAD operation
*Output  :  None
===============================================================================
*/
//void SX1276_LoRa_ClearFlag( FLAG_CLEAR flag_clear )
//{
//	INT8U tmp;
//	tmp = SX1276_ReadReg( REG_LR_IRQFLAGS );
//	tmp &= 0x00;
//	tmp |= ( 1 << flag_clear );
//	SX1276_WriteReg( REG_LR_IRQFLAGS, tmp );
//}
/*
===============================================================================
*Function:  SX1276_LoRa_SendPacket( ) => Send packet via the SX1276
*Input   :  buffer, the buffer stores data
            size, how many bytes should be written
*Output  :  None
===============================================================================
*/
void SX1276_LoRa_SendPacket( INT8U *buffer, INT8U size )
{
    if( size == 0 )                    { return; }
    
   // SX1276_LoRa_SetMode( LORA_MODE_STDBY );
    SX1276_WriteReg( REG_LR_HOPPERIOD, 0 );
    /* only need FLAG_TXDONE */
    SX1276_WriteReg( REG_LR_IRQFLAGSMASK, DEF_LR_IRQFLAGSMASK | RFLR_IRQFLAGS_RXTIMEOUT
                                                              | RFLR_IRQFLAGS_RXDONE
                                                              | RFLR_IRQFLAGS_PAYLOADCRCERROR
                                                              | RFLR_IRQFLAGS_VALIDHEADER
                                                              //| RFLR_IRQFLAGS_TXDONE
                                                              | RFLR_IRQFLAGS_CADDONE
                                                              | RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL
                                                              | RFLR_IRQFLAGS_CADDETECTED );
    /* Initializes the payload size */
//		SX1276_LoRa_SetPayloadLength( size );
    SX1276_WriteReg( REG_LR_PAYLOADLENGTH, size );

    SX1276_WriteReg( REG_LR_FIFOTXBASEADDR, 0x00 ); // Full buffer used for Tx
    SX1276_WriteReg( REG_LR_FIFOADDRPTR, 0x00 );

    /* Write payload buffer to LORA modem */
    SX1276_WriteFifo( buffer, size );
    SX1276_WriteReg( REG_LR_DIOMAPPING1, DEF_LR_DIOMAPPING1 | RFLR_DIOMAPPING1_DIO0_01  //TxDone
                                                            | RFLR_DIOMAPPING1_DIO1_00  //RxTimeout
                                                            | RFLR_DIOMAPPING1_DIO2_00  //FhssChangeChannel
                                                            | RFLR_DIOMAPPING1_DIO3_01 );  //ValidHeader
    SX1276_WriteReg( REG_LR_DIOMAPPING2, DEF_LR_DIOMAPPING2 | RFLR_DIOMAPPING2_DIO4_01  //PllLock
                                                            | RFLR_DIOMAPPING2_DIO5_00 );  //Mode Ready
    SX1276_SetSwitchTx(  );
    SX1276_LoRa_SetMode( LORA_MODE_TX );

    while( ! SX1276_READ_DIO0( ) ); //TxDone
    SX1276_WriteReg( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_TXDONE );
    SX1276_LoRa_SetMode( LORA_MODE_STDBY );
}
/*
===============================================================================
*Function:  SX1276_LoRa_StartRx( ) => Sets the radio in Rx mode
*Input   :  None
*Output  :  None
===============================================================================
*/
void SX1276_LoRa_StartRx( void )
{
	SX1276_LoRa_SetMode( LORA_MODE_STDBY );

	/* need FLAG_RXDONE, FLAG_PAYLOADCRCERROR, FLAG_FHSSCHANGECHANNEL */
	SX1276_WriteReg( REG_LR_IRQFLAGSMASK, DEF_LR_IRQFLAGSMASK | RFLR_IRQFLAGS_RXTIMEOUT
															  //| RFLR_IRQFLAGS_RXDONE
															  //| RFLR_IRQFLAGS_PAYLOADCRCERROR
															  | RFLR_IRQFLAGS_VALIDHEADER
															  | RFLR_IRQFLAGS_TXDONE
															  | RFLR_IRQFLAGS_CADDONE
															  //| RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL
															  | RFLR_IRQFLAGS_CADDETECTED
															  );
	SX1276_LoRa_SetFreqHoppingPeriod( 0 );  //By the FreqHopping is disabled

	SX1276_WriteReg( REG_LR_DIOMAPPING1, DEF_LR_DIOMAPPING1 | RFLR_DIOMAPPING1_DIO0_00  //RxDone
	                              							| RFLR_DIOMAPPING1_DIO1_00  //RxTimeout
	                              							| RFLR_DIOMAPPING1_DIO2_00  //FhssChangeChannel
	                              							| RFLR_DIOMAPPING1_DIO3_00 );  //CadDone
	SX1276_WriteReg( REG_LR_DIOMAPPING2, DEF_LR_DIOMAPPING2 | RFLR_DIOMAPPING2_DIO4_00  //CadDetected
	                                                        | RFLR_DIOMAPPING2_DIO5_00 );  //Mode Ready

	/* Rx continuous model */
	SX1276_WriteReg( REG_LR_FIFOADDRPTR, SX1276_ReadReg( REG_LR_FIFORXBASEADDR ) );  // Set FifoAddrPtr to FifoRxBaseAddr
    SX1276_SetSwitchRx( );
    //SX1276_LoRa_FlushFifo( );
	SX1276_LoRa_SetMode( LORA_MODE_RXC );
}
/*
===============================================================================
*Function:  SX1276_LoRa_RxProcess( ) => Process the LoRa modem Rx state machines depending on the SX1276 operating mode
*Input   :  None
*Output  :  result,
===============================================================================
*/
extern void USART_Send( INT8U *x, INT8U y );
INT8U SX1276_LoRa_RxProcess( void )
{
    INT8U result = 0;
    if( SX1276_READ_DIO1( ) )
    {
        SX1276_WriteReg( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_RXTIMEOUT );
    }
   // if( SX1276_READ_DIO0( ) )
    {
        SX1276_WriteReg( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_RXDONE );
        if( ( SX1276_ReadReg( REG_LR_IRQFLAGS ) & RFLR_IRQFLAGS_PAYLOADCRCERROR ) == RFLR_IRQFLAGS_PAYLOADCRCERROR )
        {
            SX1276_WriteReg( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_PAYLOADCRCERROR );
            result = 0;
        } else {
            result = 1;
        }
        SX1276_WriteReg(REG_LR_IRQFLAGS, 0xff);
        SX1276_LoRa_SetMode( LORA_MODE_STDBY );
    }
    return result;
}
/*
===============================================================================
*Function:  SX1276_LoRa_GetRxPacket( ) => Gets a copy of the current received buffer
*Input   :  buffer, the buffer stores data
*Output  :  size, hao many bytes received
===============================================================================
*/

void delay_3ms(u16 num)//不是很精确
{
  u8 i = 0;
  while(num--)
  {
    for (i=0; i<100; i++);
  }
}

INT8U SX1276_LoRa_GetRxPacket( INT8U *buffer )
{
    INT8U size, i;
    size = SX1276_ReadReg( REG_LR_NBRXBYTES );
    if(size <= 4)
        return 0;
    if( size > RF_MAX_PACKET_SIZE )
    {
        while( size -- )
        {
            *buffer = SX1276_ReadFifo( );
        }
        return 0;
    }
    SX1276_WriteReg( REG_LR_FIFOADDRPTR, SX1276_ReadReg( REG_LR_FIFORXCURRENTADDR ) );
    /*地址判断*/
    if((AddrLow1 == SX1276_ReadFifo()) && (AddrLow2 == SX1276_ReadFifo()) && (AddrHigh1 == SX1276_ReadFifo()) && (AddrHigh2 == SX1276_ReadFifo()))
    {
       
        //sleep_time_count = 0;
	size = size -4;
        for( i = 0; i < size; i++ )
	{
		*buffer ++ = SX1276_ReadFifo( );
	}
    } else {
        size = 0;
    }
    return size;
}
/*
===============================================================================
*Function:  SX1276_LoRa_SetSF6( ) => Special setting when Spreading Factor = 6
*Input   :  None
*Output  :  None
===============================================================================
*/
void SX1276_LoRa_SetSF6( void )
{
	INT8U tmp;
	SX1276_LoRa_SetImplicitHeaderEn( IMPLICITHEADER_ON );  //Enables the Implicit Header mode in LoRa

	tmp = SX1276_ReadReg( 0x31 );
	tmp &= ~( ( 1<<2) | ( 1<<1 ) | ( 1<<0 ) );
	tmp |= 0x05;
	SX1276_WriteReg( 0x31, tmp );

	tmp = SX1276_ReadReg( 0x37 );
	tmp &= 0x00;
	tmp |= 0x0C;
	SX1276_WriteReg( 0x37, tmp );
}
/*
===============================================================================
*Function:  SX1276_LoRa_FlushFifo( ) =>Flush the FIFO in LoRa model
*Input   :  None
*Output  :  None
===============================================================================
*/
//void SX1276_LoRa_FlushFifo( void )
//{
//    INT8U tmp;
//    return;
//    SX1276_LoRa_SetReg( FSK_REG );  //Access FSK registers page (in mode LoRa) 0x0D: 0x3F
//    tmp =  SX1276_ReadReg( 0x3F );
//    tmp |= ( 1<<4 );
//    SX1276_WriteReg( 0x3F, tmp );
//    SX1276_LoRa_SetReg( LORA_REG );  //Access LoRa registers page 0x0D: 0x3F
//}

/*
===============================================================================
*Function:  SX1276_Lora_SetPreambleSize( ) =>Set the preamble count
*Input   :  cnt, count of the preamble size
*Output  :  None
===============================================================================
*/
void SX1276_Lora_SetPreambleSize( INT16U cnt )
{
    SX1276_LoRa_SetMode( LORA_MODE_STDBY );
    SX1276_WriteReg( REG_LR_PREAMBLEMSB, cnt >> 8 );
    SX1276_WriteReg( REG_LR_PREAMBLELSB, cnt );
}












































