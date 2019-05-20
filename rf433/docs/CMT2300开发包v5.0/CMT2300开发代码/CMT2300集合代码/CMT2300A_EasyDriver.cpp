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


#include <CMT2300A_EasyDriver.h>

/**********************************************************
State Ctrl
**********************************************************/
/**********************************************************
**Name:     bGoTx
**Function: Entry Tx Mode
**Input:    none
**Output:   none
**********************************************************/
bool cmt2300aEasy::bGoTx(void)
{
 byte tmp, i;
 
 Spi3.vSpi3Write(((word)CMT23_MODE_CTL<<8)+MODE_GO_TFS);	
 for(i=0; i<50; i++)
 	{
 	_delay_us(200);	
	tmp = (MODE_MASK_STA & Spi3.bSpi3Read(CMT23_MODE_STA));	
	if(tmp==MODE_STA_TFS)
		break;
	}
 if(i>=50)
 	return(false);
  
 Spi3.vSpi3Write(((word)CMT23_MODE_CTL<<8)+MODE_GO_TX);		
 for(i=0; i<50; i++)
 	{
 	_delay_us(200);	
	tmp = (MODE_MASK_STA & Spi3.bSpi3Read(CMT23_MODE_STA));	
	if(tmp==MODE_STA_TX)
		break;
	}
 if(i>=50)
 	return(false);
 else
 	return(true);
}

/**********************************************************
**Name:     bGoRx
**Function: Entry Rx Mode
**Input:    none
**Output:   none
**********************************************************/
bool cmt2300aEasy::bGoRx(void)
{
 byte tmp, i;
 RssiTrig = false;
 Spi3.vSpi3Write(((word)CMT23_MODE_CTL<<8)+MODE_GO_RFS);	
 for(i=0; i<50; i++)
 	{
 	_delay_us(200);	
	tmp = (MODE_MASK_STA & Spi3.bSpi3Read(CMT23_MODE_STA));	
	if(tmp==MODE_STA_RFS)
		break;
	}
 if(i>=50)
 	return(false);
  
 Spi3.vSpi3Write(((word)CMT23_MODE_CTL<<8)+MODE_GO_RX);		
 for(i=0; i<50; i++)
 	{
 	_delay_us(200);	
	tmp = (MODE_MASK_STA & Spi3.bSpi3Read(CMT23_MODE_STA));	
	if(tmp==MODE_STA_RX)
		break;
	}
 if(i>=50)
 	return(false);
 else
 	return(true);
}

/**********************************************************
**Name:     bGoSleep
**Function: Entry Sleep Mode
**Input:    none
**Output:   none
**********************************************************/
bool cmt2300aEasy::bGoSleep(void)
{
 byte tmp;
 
 Spi3.vSpi3Write(((word)CMT23_MODE_CTL<<8)+MODE_GO_SLEEP);	
 _delay_us(100);		//enough?
 tmp = (MODE_MASK_STA & Spi3.bSpi3Read(CMT23_MODE_STA));	
 if(tmp==MODE_STA_SLEEP)
 	return(true);
 else
 	return(false);
}

/**********************************************************
**Name:     bGoStandby
**Function: Entry Standby Mode
**Input:    none
**Output:   none
**********************************************************/
bool cmt2300aEasy::bGoStandby(void)
{
 byte tmp, i;	
 
 RssiTrig = false;
 Spi3.vSpi3Write(((word)CMT23_MODE_CTL<<8)+MODE_GO_STBY);	
 for(i=0; i<50; i++)
 	{
 	_delay_us(200);	
	tmp = (MODE_MASK_STA & Spi3.bSpi3Read(CMT23_MODE_STA));	
	if(tmp==MODE_STA_STBY)
		break;
	}
 if(i>=50)
 	return(false);
 else
 	return(true);
}

/**********************************************************
**Name:     vSoftReset
**Function: Software reset Chipset
**Input:    none
**Output:   none
**********************************************************/
void cmt2300aEasy::vSoftReset(void)
{
 Spi3.vSpi3Write(((word)CMT23_SOFTRST<<8)+0xFF); 
 _delay_us(1000);				//enough?
}

/**********************************************************
**Name:     bReadStatus
**Function: read chipset status
**Input:    none
**Output:   none
**********************************************************/
byte cmt2300aEasy::bReadStatus(void)
{
 return(MODE_MASK_STA & Spi3.bSpi3Read(CMT23_MODE_STA));		
}

/**********************************************************
**Name:     bReadRssi
**Function: Read Rssi
**Input:    true------dBm;
            false-----Code;
**Output:   none
**********************************************************/
byte cmt2300aEasy::bReadRssi(bool unit_dbm)
{
 if(unit_dbm)
 	return(Spi3.bSpi3Read(CMT23_RSSI_DBM));
 else		
 	return(Spi3.bSpi3Read(CMT23_RSSI_CODE));
}

/**********************************************************
GPIO & Interrupt CFG
**********************************************************/
/**********************************************************
**Name:     vGpioFuncCfg
**Function: GPIO Function config
**Input:    none
**Output:   none
**********************************************************/
void cmt2300aEasy::vGpioFuncCfg(byte io_cfg)
{
 Spi3.vSpi3Write(((word)CMT23_IO_SEL<<8)+io_cfg);
}

/**********************************************************
**Name:     vIntSrcCfg
**Function: config interrupt source  
**Input:    int_1, int_2
**Output:   none
**********************************************************/
void cmt2300aEasy::vIntSrcCfg(byte int_1, byte int_2)
{
 byte tmp;
 tmp = INT_MASK & Spi3.bSpi3Read(CMT23_INT1_CTL);
 Spi3.vSpi3Write(((word)CMT23_INT1_CTL<<8)+(tmp|int_1));
 
 tmp = INT_MASK & Spi3.bSpi3Read(CMT23_INT2_CTL);
 Spi3.vSpi3Write(((word)CMT23_INT2_CTL<<8)+(tmp|int_2));
}

/**********************************************************
**Name:     vEnableAntSwitch
**Function:  
**Input:    
**Output:   none
**********************************************************/
void cmt2300aEasy::vEnableAntSwitch(byte mode)
{
 byte tmp;
 tmp = Spi3.bSpi3Read(CMT23_INT1_CTL);
 tmp&= 0x3F;
 switch(mode)
 	{
 	case 1:
 		tmp |= RF_SWT1_EN; break;		//GPO1=RxActive; GPO2=TxActive	
 	case 2:
 		tmp |= RF_SWT2_EN; break;		//GPO1=RxActive; GPO2=!RxActive
 	case 0:
 	default:
 		break;							//Disable	
 	}
 Spi3.vSpi3Write(((word)CMT23_INT1_CTL<<8)+tmp);
}


/**********************************************************
**Name:     vIntSrcEnable
**Function: enable interrupt source 
**Input:    en_int
**Output:   none
**********************************************************/
void cmt2300aEasy::vIntSrcEnable(byte en_int)
{
 Spi3.vSpi3Write(((word)CMT23_INT_EN<<8)+en_int);				
}

/**********************************************************
**Name:     vIntSrcFlagClr
**Function: clear flag
**Input:    none
**Output:   equ CMT23_INT_EN
**********************************************************/
byte cmt2300aEasy::bIntSrcFlagClr(void)
{
 byte tmp;
 byte int_clr2 = 0;
 byte int_clr1 = 0;
 byte flg = 0;
 
 tmp = Spi3.bSpi3Read(CMT23_INT_FLG);
 if(tmp&LBD_STATUS_FLAG)		//LBD_FLG_Active
 	int_clr2 |= LBD_CLR;
 
 if(tmp&PREAMBLE_PASS_FLAG)		//Preamble Active
 	{
 	int_clr2 |= PREAMBLE_PASS_CLR;
 	flg |= PREAMBLE_PASS_EN;
	}
 if(tmp&SYNC_PASS_FLAG)			//Sync Active
 	{
 	int_clr2 |= SYNC_PASS_CLR;		
 	flg |= SYNC_PASS_EN;		
 	}
 if(tmp&NODE_PASS_FLAG)			//Node Addr Active
 	{
 	int_clr2 |= NODE_PASS_CLR;	
 	flg |= NODE_PASS_EN;
 	}
 if(tmp&CRC_PASS_FLAG)			//Crc Pass Active
 	{
 	int_clr2 |= CRC_PASS_CLR;
 	flg |= CRC_PASS_EN;
 	}
 if(tmp&RX_DONE_FLAG)			//Rx Done Active
 	{
 	int_clr2 |= RX_DONE_CLR;
 	flg |= PKT_DONE_EN;
 	}
 	
 if(tmp&COLLISION_ERR_FLAG)		//这两个都必须通过RX_DONE清除
 	int_clr2 |= RX_DONE_CLR;
 if(tmp&DC_ERR_FLAG)
 	int_clr2 |= RX_DONE_CLR;
 	
 Spi3.vSpi3Write(((word)CMT23_INT_CLR2<<8)+int_clr2);	//Clear flag
 
 
 tmp = Spi3.bSpi3Read(CMT23_INT_CLR1);
 if(tmp&TX_DONE_FLAG)
 	{
 	int_clr1 |= TX_DONE_CLR;
 	flg |= TX_DONE_EN;
 	}	
 if(tmp&SLEEP_TIMEOUT_FLAG)
 	{
 	int_clr1 |= SLEEP_TIMEOUT_CLR;
 	flg |= SLEEP_TMO_EN;
 	} 
 if(tmp&RX_TIMEOUT_FLAG)
 	{
 	int_clr1 |= RX_TIMEOUT_CLR;
 	flg |= RX_TMO_EN;
 	}	
 Spi3.vSpi3Write(((word)CMT23_INT_CLR1<<8)+int_clr1);	//Clear flag 
 
 return(flg);
}

/**********************************************************
**Name:     bClearFIFO
**Function: clear FIFO buffer
**Input:    none
**Output:   FIFO state
**********************************************************/
byte cmt2300aEasy::vClearFIFO(void)
{
 byte tmp;	
 tmp = Spi3.bSpi3Read(CMT23_FIFO_FLG);
 Spi3.vSpi3Write(((word)CMT23_FIFO_CLR<<8)+FIFO_CLR_RX+FIFO_CLR_TX);
 return(tmp);
}

void cmt2300aEasy::vEnableWrFifo(void)
{
 byte tmp;
 tmp = Spi3.bSpi3Read(CMT23_FIFO_CTL);
 tmp |= (SPI_FIFO_RD_WR_SEL|FIFO_RX_TX_SEL);
 Spi3.vSpi3Write(((word)CMT23_FIFO_CTL<<8)+tmp);
}

void cmt2300aEasy::vEnableRdFifo(void)
{
 byte tmp;
 tmp = Spi3.bSpi3Read(CMT23_FIFO_CTL);
 tmp &= (~(SPI_FIFO_RD_WR_SEL|FIFO_RX_TX_SEL));
 Spi3.vSpi3Write(((word)CMT23_FIFO_CTL<<8)+tmp);
}

/**********************************************************
CFG
**********************************************************/
/**********************************************************
**Name:     vInit
**Function: Init. CMT2300A
**Input:    none
**Output:   none
**********************************************************/
void cmt2300aEasy::vInit(void)
{
 //byte i;
 byte tmp;
 //word len;
 Spi3.vSpi3Init();
 GPO1In();
 GPO2In();
 GPO3In();
 
 vSoftReset();
 _delay_ms(20);
 bGoStandby();  //在 standby  模式下进行参数的配置

 	
 tmp = Spi3.bSpi3Read(CMT23_MODE_STA);
 tmp|= EEP_CPY_DIS;
 tmp&= (~RSTN_IN_EN);			//Disable RstPin	
 Spi3.vSpi3Write(((word)CMT23_MODE_STA<<8)+tmp);

 bIntSrcFlagClr();
 
}

void cmt2300aEasy::vCfgBank(word cfg[], byte length)
{
 byte i;
 
 if(length!=0)
 	{	
 	for(i=0; i<length; i++)	
 		Spi3.vSpi3Write(cfg[i]);
 	}	
}


/******************************************************************************
**函数名称：bGetMessage
**函数功能：接收一包数据
**输入参数：无
**输出参数：非0——接收成功
**          0——接收失败
******************************************************************************/
byte cmt2300aEasy::bGetMessage(byte msg[])
{
 byte i;	
 
 vEnableRdFifo();	
 if(FixedPktLength)
 	{
  	Spi3.vSpi3BurstReadFIFO(msg, PayloadLength);
	i = PayloadLength;
	}
 else
 	{
	i = Spi3.bSpi3ReadFIFO();	
 	Spi3.vSpi3BurstReadFIFO(msg, i);
 	}
 return(i);
}

byte cmt2300aEasy::bGetMessageByFlag(byte msg[])
{
 byte tmp;
 byte tmp1;
 byte rev = 0;
 tmp = Spi3.bSpi3Read(CMT23_INT_FLG);
 if((tmp&SYNC_PASS_FLAG)&&(!RssiTrig))
 	{
 	PktRssi = bReadRssi(false);
 	RssiTrig = true;
 	}
 
 tmp1 = Spi3.bSpi3Read(CMT23_CRC_CTL);
 vEnableRdFifo();	 
 if(tmp1&CRC_ENABLE)		//Enable CrcCheck
 	{
 	if(tmp&CRC_PASS_FLAG)
 		{
 		if(FixedPktLength)
 			{
  			Spi3.vSpi3BurstReadFIFO(msg, PayloadLength);
			rev = PayloadLength;
			}
 		else
 			{	
			rev = Spi3.bSpi3ReadFIFO();	
 			Spi3.vSpi3BurstReadFIFO(msg, rev);
 			}
 		RssiTrig = false;
 		}
 	}
 else
 	{
	if(tmp&RX_DONE_FLAG) 		
		{
 		if(FixedPktLength)
 			{
  			Spi3.vSpi3BurstReadFIFO(msg, PayloadLength);
			rev = PayloadLength;
			}
 		else
 			{	
			rev = Spi3.bSpi3ReadFIFO();	
 			Spi3.vSpi3BurstReadFIFO(msg, rev);
 			}	
 		RssiTrig = false;		
		}
 	}
 
 if(tmp&COLLISION_ERR_FLAG)			//错误处理
	rev = 0xFF;
 return(rev);
}

/******************************************************************************
**函数名称：bSendMessage
**函数功能：发射一包数据
**输入参数：无
**输出参数：
**          
******************************************************************************/
bool cmt2300aEasy::bSendMessage(byte msg[], byte length)
{
 //mode1
 //vSetTxPayloadLength(length);
 //bGoStandby();
 //vEnableWrFifo();	
 //Spi3.vSpi3BurstWriteFIFO(msg, length);
 //bGoTx();
 
 //mode2
 bIntSrcFlagClr();  //清中断
 vSetTxPayloadLength(length);
 bGoTx();
 vEnableWrFifo();	
 Spi3.vSpi3BurstWriteFIFO(msg, length);
 return(true);
}

void cmt2300aEasy::vSetTxPayloadLength(word length)
{
 byte tmp;	
 byte len;
 bGoStandby();
 tmp = Spi3.bSpi3Read(CMT23_PKT_CTRL1);
 tmp&= 0x8F;
 
 if(length!=0)
 	{
 	if(FixedPktLength)
		len = length-1;
 	else
		len = length;
	}
 else
 	len = 0;
 
 tmp|= (((byte)(len>>8)&0x07)<<4);
 Spi3.vSpi3Write(((word)CMT23_PKT_CTRL1<<8)+tmp);
 Spi3.vSpi3Write(((word)CMT23_PKT_LEN<<8)+(byte)len);	//Payload length
 //bGoSleep();
}

	
