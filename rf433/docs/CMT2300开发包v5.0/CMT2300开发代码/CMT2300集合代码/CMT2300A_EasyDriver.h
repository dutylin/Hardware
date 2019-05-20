/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: 
 *  (1) "AS IS" WITH NO WARRANTY; 
 *  (2) TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, HopeRF SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * Copyright (C) HopeRF
 *
 * website: www.HopeRF.com
 *          www.HopeRF.cn     
 */

/*! 
 * file       CMT2300A_EasyDriver
 * brief      how to use CMT2300A
 * hardware   HopeDuino with RFM230
 *            
 *
 * version    1.0
 * date       Dec 14 2016
 * author     QY Ruan
 */
 
 #ifndef CMT2300A_EasyDriver_h
	#define CMT2300A_EasyDriver_h

	#ifndef F_CPU
		#define F_CPU 16000000UL	//HopeDuino use 16MHz Xo
	#endif	

	#ifndef	byte
		typedef unsigned char byte;
	#endif
	
	#ifndef word
		typedef unsigned int  word;
	#endif	

	#include <avr/io.h>
	#include <util/delay.h>
	#include <HopeDuino_SPI3.h>
	#include <CMT2300A_Register.h>
	
	/** Hardware brief **/    
	//PORTB						//DDRx		PORTx		PINx
	#define _GPO1    0x01       // 0          0          0
	
	//PORTD
	#define _GPO2    0x80       // 0          0          0
	#define _GPO3    0x40       // 0          0          0
	#define _GPO4    0x20       // 0          0          0 
	
	#define	GPO1In()	(DDRB &= (~_GPO1))
	#define GPO1_H() 	(PINB&_GPO1)	
	#define GPO1_L()	((PINB&_GPO1)==0)

	#define	GPO2In()	(DDRD &= (~_GPO2))
	#define GPO2_H() 	(PIND&_GPO2)	
	#define GPO2_L()	((PIND&_GPO2)==0)

	#define	GPO3In()	(DDRD &= (~_GPO3))
	#define GPO3_H() 	(PIND&_GPO3)	
	#define GPO3_L()	((PIND&_GPO3)==0)

	
	
	class cmt2300aEasy
	{
	 public:	
/**/	bool  FixedPktLength;						//false: for contain packet length in Tx message, the same mean with variable lenth
 		                                            //true : for doesn't include packet length in Tx message, the same mean with fixed length
/**/	word  PayloadLength;						//unit: byte  range: 1-2047                                      
		
		byte  PktRssi;
 		                                            
	 	//State Ctrl
	 	bool bGoTx(void);
	 	bool bGoRx(void);
	 	bool bGoSleep(void);	
	 	bool bGoStandby(void);		
	 	void vSoftReset(void);
	 	byte bReadStatus(void);
	 	byte bReadRssi(bool unit_dbm);

		//GPIO & Interrupt CFG
		void vGpioFuncCfg(byte io_cfg);
		void vIntSrcCfg(byte int_1, byte int_2);
		void vEnableAntSwitch(byte mode);					//
		void vIntSrcEnable(byte en_int);
		byte bIntSrcFlagClr(void);
		byte vClearFIFO(void);
		void vEnableRdFifo(void);
		void vEnableWrFifo(void);
		void vSetTxPayloadLength(word length);
	
		//CFG
		void vInit(void);
		void vCfgBank(word cfg[], byte length);

		byte bGetMessage(byte msg[]);
		byte bGetMessageByFlag(byte msg[]);
		bool bSendMessage(byte msg[], byte length);
					
	 private:
	 	bool RssiTrig;
	 	spi3Class Spi3;
	 	
	};
#else
	#warning "HopeDuino_CMT2300A.h have been defined!"

#endif
