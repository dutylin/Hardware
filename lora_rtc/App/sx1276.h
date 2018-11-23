
#ifndef __SX1276_H__
#define __SX1276_H__
//#include "bsp.h"
#include "STM8l15x_conf.h"
//#include "mytypedef.h"
#include "sx1276-LoRa.h"
#include "sx1276-Fsk.h"
/*
================================================================================
-------------------------IMPORT FUNCTIONS FOR DRIVER----------------------------
================================================================================
*/

#define PORT_SX127X_CSN     	   GPIOB
#define PIN_SX127X_CSN             GPIO_Pin_4

#define PORT_SX127X_RST            GPIOB
#define PIN_SX127X_RST             GPIO_Pin_3

//#define PORT_SX127X_OSEN           GPIOA
//#define PIN_SX127X_OSEN            GPIO_Pin_2

#define PORT_SX127X_TX_CTRL        GPIOA
#define PIN_SX127X_TX_CTRL         GPIO_Pin_5

#define PORT_SX127X_RX_CTRL        GPIOA
#define PIN_SX127X_RX_CTRL         GPIO_Pin_4

#define PORT_SX127X_DIO0     	   GPIOD
#define PIN_SX127X_DIO0            GPIO_Pin_1

#define PORT_SX127X_DIO1     	   GPIOD
#define PIN_SX127X_DIO1            GPIO_Pin_2

#define PORT_SX127X_DIO2     	   GPIOD
#define PIN_SX127X_DIO2            GPIO_Pin_3

#define PORT_SX127X_DIO3     	   GPIOC
#define PIN_SX127X_DIO3            GPIO_Pin_3

#define PORT_SX127X_DIO4     	   GPIOC
#define PIN_SX127X_DIO4            GPIO_Pin_2

#define PORT_SX127X_DIO5     	   GPIOC
#define PIN_SX127X_DIO5            GPIO_Pin_1


#define SX1276_NSS_LOW( )          GPIO_ResetBits( PORT_SX127X_CSN, PIN_SX127X_CSN )
#define SX1276_NSS_HIGH( )         GPIO_SetBits( PORT_SX127X_CSN, PIN_SX127X_CSN )

#define SX1276_RESET_LOW( )        GPIO_ResetBits( PORT_SX127X_RST, PIN_SX127X_RST )
#define SX1276_RESET_HIGH( )       GPIO_SetBits( PORT_SX127X_RST, PIN_SX127X_RST )

#define SX1276_OSEN_LOW( )         GPIO_ResetBits( PORT_SX127X_OSEN, PIN_SX127X_OSEN )
#define SX1276_OSEN_HIGH( )        GPIO_SetBits( PORT_SX127X_OSEN, PIN_SX127X_OSEN )

#define SX1276_TX_CTRL_LOW( )      GPIO_ResetBits( PORT_SX127X_TX_CTRL, PIN_SX127X_TX_CTRL )
#define SX1276_TX_CTRL_HIGH( )     GPIO_SetBits( PORT_SX127X_TX_CTRL, PIN_SX127X_TX_CTRL )

#define SX1276_RX_CTRL_LOW( )      GPIO_ResetBits( PORT_SX127X_RX_CTRL, PIN_SX127X_RX_CTRL )
#define SX1276_RX_CTRL_HIGH( )     GPIO_SetBits( PORT_SX127X_RX_CTRL, PIN_SX127X_RX_CTRL )

#define SX1276_READ_DIO0( )    GPIO_ReadInputDataBit( PORT_SX127X_DIO0, PIN_SX127X_DIO0 )
#define SX1276_READ_DIO1( )    GPIO_ReadInputDataBit( PORT_SX127X_DIO1, PIN_SX127X_DIO1 )
#define SX1276_READ_DIO2( )    GPIO_ReadInputDataBit( PORT_SX127X_DIO2, PIN_SX127X_DIO2 )
#define SX1276_READ_DIO3( )    GPIO_ReadInputDataBit( PORT_SX127X_DIO3, PIN_SX127X_DIO3 )
#define SX1276_READ_DIO4( )    GPIO_ReadInputDataBit( PORT_SX127X_DIO4, PIN_SX127X_DIO4 )
#define SX1276_READ_DIO5( )    GPIO_ReadInputDataBit( PORT_SX127X_DIO5, PIN_SX127X_DIO5 )

/*
================================================================================
--------------------------------MACRO DIFINITIONS-------------------------------
================================================================================
*/
typedef enum
{
    MODE_FSK                =   0x00,       //FSK mode
//   	MODE_OOK                =   0x01,       //OOK mode
   	MODE_LORA               =   0x01,       //LoRa mode
}RANGE_MODE;
#define IS_RANGE_MODE( x )   (((x) == MODE_FSK) || ((x) == MODE_LORA))
void SX1276_Par_Error(uint8_t* file, uint32_t line);
#define AX1276_assert_param(expr) ((expr) ? (void)0 : SX1276_Par_Error((uint8_t *)__FILE__, __LINE__))



/*
================================================================================
-------------------------EXPORT FUNCTIONS OF DRIVER-----------------------------
================================================================================
*/

typedef struct
{
    INT8U               Channel;
    INT8U               PowerIndex;
    RANGE_MODE          Mode_Lora_Fsk;
    INT8U               BitrateIndex;
}T_SX1276;
extern T_SX1276 SX1276;

/*Put the device into Lora CAD mode*/
void SX1276_CADMode( void );

/*Put the device into sleep mode*/
void SX1276_SleepMode( void );

/*Receive a packet*/
INT8U SX1276_ReceivePacket( INT8U *buffer );

/*Send a packet*/
void SX1276_SendPacket( INT8U *buffer, INT8U size );

/*Set the device into RX mode*/
void SX1276_SetRxMode( void );

/*Set the datarate of the device*/
void SX1276_SetDatarate( INT8U rate_index );

/*Initialize the SX1276, Lora or FSK mode*/
void SX1276_Init( RANGE_MODE mode );

/* Write a register of the device */
void SX1276_WriteReg( INT8U addr, INT8U value );

/* Write registers of the device */
void SX1276_WriteBuffer( INT8U addr, INT8U *buffer, INT8U size );

/* Write data into fifo */
void SX1276_WriteFifo( INT8U *buffer, INT8U size );

/* Read a register of the device */
INT8U SX1276_ReadReg( INT8U addr );

/* Read a byte from FIFO */
INT8U SX1276_ReadFifo( void );

/* Put the SX1276 into another mode */
void SX1276_SetMode( RANGE_MODE range_mode );

/* Reset the SX1276 */
void SX1276_Reset( void );

/* Set SwitchTx  on */
void SX1276_SetSwitchTx( void );

/* Set SwitchRx  on */
void SX1276_SetSwitchRx( void );

/* Set the Frequency of SX1276 */
void SX1276_SetFreq( INT8U FreqIndex );

/* Set the output power */
void SX1276_SetPower(INT8U power, INT8U pin);

/* Set the PA ramp time of SX1276 */
void SX1276_SetPARampTime( INT8U rametime );

/*Set the preamble count of the device*/
void SX1276_SetPreambleSize( INT16U preamble_cnt );

void SX1276_PowerDown( void );
#endif //__SX1276_H__