  #ifndef __SPI_H
    #define __SPI_H
   
  #define SPI3_SPEED 20

  #include "stm8s.h"
  #include "main.h"
  #include "Delay.h"
   
  #ifndef	byte
          typedef unsigned char byte;
  #endif
          
  #ifndef word
          typedef unsigned int  word;
  #endif


  #define	SetCSB()	GPIO_WriteHigh(GPIO_PORT_C,GPIO_PINS_CSB)
  #define	ClrCSB()	GPIO_WriteLow(GPIO_PORT_C,GPIO_PINS_CSB)
          
  #define	SetFCSB()	GPIO_WriteHigh(GPIO_PORT_C,GPIO_PINS_FCSB)
  #define	ClrFCSB()	GPIO_WriteLow(GPIO_PORT_C,GPIO_PINS_FCSB)
          
  #define	SetSDCK()	GPIO_WriteHigh(GPIO_PORT_C,GPIO_PINS_SDCK)
  #define	ClrSDCK()	GPIO_WriteLow(GPIO_PORT_C,GPIO_PINS_SDCK)
          
  #define	SetSDIO()	GPIO_WriteHigh(GPIO_PORT_C,GPIO_PINS_SDIO)
  #define	ClrSDIO()	GPIO_WriteLow(GPIO_PORT_C,GPIO_PINS_SDIO)
          
  #define   InputSDIO()		GPIO_Init(GPIO_PORT_C,GPIO_PINS_SDIO,GPIO_MODE_IN_FL_NO_IT)
  #define	OutputSDIO()	GPIO_Init(GPIO_PORT_C,GPIO_PINS_SDIO,GPIO_MODE_OUT_PP_LOW_FAST)  
          
  #define	SDIO_H()	GPIO_ReadInputPin(GPIO_PORT_C,GPIO_PINS_SDIO) 
  #define	SDIO_L()	GPIO_ReadInputPin(GPIO_PORT_C,GPIO_PINS_SDIO) 

  class spi3Class
	{
	 public:
		void vSpi3Init(void);				/** initialize software SPI-3 **/	
	 	void vSpi3Write(word dat);			/** SPI-3 send one word **/
		byte bSpi3Read(byte addr);			/** SPI-3 read one byte **/
		
		void vSpi3WriteFIFO(byte dat);		/** SPI-3 send one byte to FIFO **/
		byte bSpi3ReadFIFO(void);			/** SPI-3 read one byte from FIFO **/
		void vSpi3BurstWriteFIFO(byte ptr[], byte length);			/** SPI-3 burst send N byte to FIFO**/
		void vSpi3BurstReadFIFO(byte ptr[], byte length);			/** SPI-3 burst read N byte to FIFO**/
	 private:
	    void vSpi3WriteByte(byte dat);		/** SPI-3 send one byte **/
	    byte bSpi3ReadByte(void);			/** SPI-3 read one byte **/

	};


  #else
          #warning "spi.h have been defined!"
                             
  #endif 

