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
 * file       HopeDuino_3SPI.cpp
 * brief      for HopeRF's EVB to use softwave 3-SPI (for CMOSTEK RF-IC)
 * hardware   HopeRF's EVB
 *            
 *
 * version    1.1
 * date       Jan 15 2015
 * author     QY Ruan
 */

#include "HopeDuino_SPI3.h"

/**********************************************************
**Name: 	vSpi3Init
**Func: 	Init Spi-3 Config
**Note: 	
**********************************************************/
void spi3Class::vSpi3Init(void)
{
	DDRB  |= (_CSB|_SDIO|_FCSB|_SDCK);	//SPI-3 for output
	PORTB |= (_CSB|_SDIO|_FCSB);		//set CSB,SDIO,FCSB
	ClrSDCK();							//clear CLK
}

/**********************************************************
**Name: 	vSpi3WriteByte
**Func: 	SPI-3 send one byte
**Input:
**Output:  
**********************************************************/
void spi3Class::vSpi3WriteByte(byte dat)
{

 	byte bitcnt;	
 
	SetFCSB();				//FCSB = 1;
 
 	OutputSDIO();			//SDA output mode
 	SetSDIO();				//    output 1
 
 	ClrSDCK();				
 	ClrCSB();

 	for(bitcnt=8; bitcnt!=0; bitcnt--)
 		{
		ClrSDCK();	
		if(dat&0x80)
 			SetSDIO();
 		else
 			ClrSDIO();
		_delay_us(SPI3_SPEED);
		SetSDCK();
 		dat <<= 1; 		
 		_delay_us(SPI3_SPEED);
 		}
 	ClrSDCK();		
 	SetSDIO();
}

/**********************************************************
**Name: 	bSpi3ReadByte
**Func: 	SPI-3 read one byte
**Input:
**Output:  
**********************************************************/
byte spi3Class::bSpi3ReadByte(void)
{
	byte RdPara = 0;
 	byte bitcnt;
 
  	ClrSDCK();
 	InputSDIO();			
   	ClrCSB(); 
   	
 	_delay_us(SPI3_SPEED);		//Time-Critical
 	_delay_us(SPI3_SPEED);		//Time-Critical   	
   	
 	for(bitcnt=8; bitcnt!=0; bitcnt--)
 		{
 		SetSDCK();
 		RdPara <<= 1;
 		_delay_us(SPI3_SPEED);
 		if(SDIO_H())
 			RdPara |= 0x01;
 		else
 			RdPara |= 0x00;
 		ClrSDCK();
 		_delay_us(SPI3_SPEED);
 		} 
 	ClrSDCK();
 	//OutputSDIO();
 	//SetSDIO();
 	//SetCSB();			
 	return(RdPara);	
}

/**********************************************************
**Name:	 	vSpi3Write
**Func: 	SPI Write One word
**Input: 	Write word
**Output:	none
**********************************************************/
void spi3Class::vSpi3Write(word dat)
{
 	vSpi3WriteByte((byte)(dat>>8)&0x7F);
 	vSpi3WriteByte((byte)dat);
 	SetCSB();
}

void spi3Class::vSpi3WriteCmd(byte dat)
{
 	vSpi3WriteByte(dat&0x7F);
 	SetCSB();
}

void spi3Class::vSpi3WriteTreable(byte addr, word dat)
{

 	vSpi3WriteByte(addr&0x7F);
 	vSpi3WriteByte((byte)(dat>>8));
 	vSpi3WriteByte((byte)dat);
 	SetCSB();
}

void spi3Class::vSpi3BurstWriteCmd(byte addr, byte ptr[], byte length)
{
    byte i;
 	vSpi3WriteByte(addr&0x7F);
 	for(i=0; i<length; i++)
 		vSpi3WriteByte(ptr[i]);
	SetCSB();
}



/**********************************************************
**Name:	 	bSpi3Read
**Func: 	SPI-3 Read One byte
**Input: 	readout addresss
**Output:	readout byte
**********************************************************/
byte spi3Class::bSpi3Read(byte addr)
{
	byte dat;
  	vSpi3WriteByte(addr|0x80);
  	dat = bSpi3ReadByte();
	OutputSDIO();
 	SetSDIO();
 	SetCSB();			
  	return(dat);
}

word spi3Class::wSpi3ReadTreable(byte addr)
{
	word dat;
  	vSpi3WriteByte(addr|0x80);
 	dat   = bSpi3ReadByte();
 	dat <<= 8;
 	dat  |= bSpi3ReadByte();
	OutputSDIO();
 	SetSDIO();
 	SetCSB();			
 	return(dat);
}

void spi3Class::vSpi3BurstReadCmd(byte addr, byte ptr[], byte length)
{
	byte i;

  	vSpi3WriteByte(addr|0x80);
 	for(i=0; i<length; i++)
 		ptr[i] = bSpi3ReadByte();
	OutputSDIO();
 	SetSDIO();
 	SetCSB();			
}


/**********************************************************
**Name:	 	vSpi3WriteFIFO
**Func: 	SPI-3 send one byte to FIFO
**Input: 	one byte buffer
**Output:	none
**********************************************************/
void spi3Class::vSpi3WriteFIFO(byte dat)
{
 	byte bitcnt;	
 
 	SetCSB();	
	OutputSDIO();	
	ClrSDCK();
 	ClrFCSB();			//FCSB = 0
	for(bitcnt=8; bitcnt!=0; bitcnt--)
 		{
 		ClrSDCK();
 		
 		if(dat&0x80)
			SetSDIO();		
		else
			ClrSDIO();
		_delay_us(SPI3_SPEED);
		SetSDCK();
		_delay_us(SPI3_SPEED);
 		dat <<= 1;
 		}
 	ClrSDCK();	
 	_delay_us(SPI3_SPEED);		//Time-Critical
 	_delay_us(SPI3_SPEED);		//Time-Critical
 	SetFCSB();
	SetSDIO();
 	_delay_us(SPI3_SPEED);		//Time-Critical
 	_delay_us(SPI3_SPEED);		//Time-Critical
}

/**********************************************************
**Name:	 	bSpi3ReadFIFO
**Func: 	SPI-3 read one byte to FIFO
**Input: 	none
**Output:	one byte buffer
**********************************************************/
byte spi3Class::bSpi3ReadFIFO(void)
{
	byte RdPara;
 	byte bitcnt;	

 	SetCSB();
	InputSDIO();
 	ClrSDCK();
	ClrFCSB();
	 	
 	_delay_us(SPI3_SPEED);		//Time-Critical
 	_delay_us(SPI3_SPEED);		//Time-Critical
		
 	for(bitcnt=8; bitcnt!=0; bitcnt--)
 		{

		SetSDCK();
		_delay_us(SPI3_SPEED);
 		RdPara <<= 1;
 		if(SDIO_H())
 			RdPara |= 0x01;		//NRZ MSB
 		else
 		 	RdPara |= 0x00;		//NRZ MSB
 		ClrSDCK();
 		_delay_us(SPI3_SPEED); 		 	
 		}
 	
 	ClrSDCK();
 	SetFCSB();
	OutputSDIO();
	SetSDIO();
 	return(RdPara);
}

/**********************************************************
**Name:	 	vSpi3BurstWriteFIFO
**Func: 	burst wirte N byte to FIFO
**Input: 	array length & head pointer
**Output:	none
**********************************************************/
void spi3Class::vSpi3BurstWriteFIFO(byte ptr[], byte length)
{
 	byte i;
 	if(length!=0x00)
	 	{
 		for(i=0;i<length;i++)
 			vSpi3WriteFIFO(ptr[i]);
 		}
 	return;
}

/**********************************************************
**Name:	 	vSpiBurstRead
**Func: 	burst wirte N byte to FIFO
**Input: 	array length  & head pointer
**Output:	none
**********************************************************/
void spi3Class::vSpi3BurstReadFIFO(byte ptr[], byte length)
{
	byte i;
 	if(length!=0)
 		{
 		for(i=0;i<length;i++)
 			ptr[i] = bSpi3ReadFIFO();
 		}	
 	return;
}

void spi3Class::vSpi3SendByte(byte dat)
{
	byte bitcnt;	

 	for(bitcnt=0; bitcnt<8; bitcnt++)
	{
		ClrSDCK();	
		if(dat&0x80)
			SetSDIO();
		else
			ClrSDIO();
		
		_delay_us(SPI3_SPEED);
		dat <<= 1; 	
		SetSDCK();				
		_delay_us(SPI3_SPEED);
	}
}

byte spi3Class::bSpi3RecvByte(void)
{
	byte RdPara = 0;
 	byte bitcnt;
 	
   	
 	for(bitcnt=0; bitcnt<8; bitcnt++)
 	{
 		
 		RdPara <<= 1;
		ClrSDCK();		
 		_delay_us(SPI3_SPEED);
		SetSDCK();
 		if(SDIO_H())
 			RdPara |= 0x01;
 		else
 			RdPara |= 0x00;
 		
 		_delay_us(SPI3_SPEED);
 	} 
 		
 	return(RdPara);	
}

void spi3Class::vSpi3WriteBytes(byte addr,byte* pInBuf,word len)
{
	word i;

	OutputSDIO();
	SetSDIO();				//    output 1
 
 	ClrSDCK();				
 	ClrCSB();
	_delay_us(SPI3_SPEED);
	_delay_us(SPI3_SPEED);

	vSpi3SendByte(addr|0x00);

	for(i=0;i<len;i++)
	{
		vSpi3SendByte(pInBuf[i]);
	}

	ClrSDCK();
	_delay_us(SPI3_SPEED);
	_delay_us(SPI3_SPEED);
	SetCSB();
	SetSDIO();
	InputSDIO();
}

void spi3Class::vSpi3ReadBytes(byte addr,byte* pOutBuf,word len)
{
	word i;

	OutputSDIO();
	SetSDIO();				//    output 1
 
 	ClrSDCK();				
 	ClrCSB();
	_delay_us(SPI3_SPEED);
	_delay_us(SPI3_SPEED);

	vSpi3SendByte(addr|0x80);

	SetSDIO();
	InputSDIO();

	for(i=0;i<len;i++)
	{
		pOutBuf[i]=bSpi3RecvByte();
	}

	ClrSDCK();
	_delay_us(SPI3_SPEED);
	_delay_us(SPI3_SPEED);
	SetCSB();
	SetSDIO();
	InputSDIO();
}


