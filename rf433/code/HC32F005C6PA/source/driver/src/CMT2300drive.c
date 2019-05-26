#include <CMT2300drive.h>
/**/	 bool  FixedPktLength;						//false: for contain packet length in Tx message, the same mean with variable lenth
//true : for doesn't include packet length in Tx message, the same mean with fixed length
/**/	 word  PayloadLength;
bool bGoTx(void)
{
byte tmp, i;

INIT_TX:
 vSpi3Write(((word)CMT23_MODE_CTL<<8)+MODE_GO_TX);		
 for(i=0; i<50; i++)
 	{
 	Delay_us(200);	
	tmp = (MODE_MASK_STA & bSpi3Read(CMT23_MODE_STA));	
	if(tmp==MODE_STA_TX)
		break;
	}
 if(i>=50)
 	{
 	
		bGoStandby();
 		goto INIT_TX;
 	}
 else
 	return(TRUE);
}

/**********************************************************
**Name:     bGoRx
**Function: Entry Rx Mode
**Input:    none
**Output:   none
**********************************************************/

byte vReadIngFlag1(void)
{
	return(bSpi3Read((byte)(CMT23_INT_FLG>>8)));

}

byte vReadIngFlag2(void)
{
	return(bSpi3Read((byte)(CMT23_INT_CLR1	>>8)));

}

	byte  PktRssi;
 	bool RssiTrig;
bool bGoRx(void)
{
 byte tmp, i;
 RssiTrig = FALSE;

 INIT_RX:
 vSpi3Write(((word)CMT23_MODE_CTL<<8)+MODE_GO_RX);		
 for(i=0; i<50; i++)
 	{
 	Delay_us(200);	
	tmp = (MODE_MASK_STA & bSpi3Read(CMT23_MODE_STA));	
	if(tmp==MODE_STA_RX)
		break;
	}
 if(i>=50)
 	{
		bGoStandby();
 		goto INIT_RX;
 	}
 else
 	return(TRUE);
}

/**********************************************************
**Name:     bGoSleep
**Function: Entry Sleep Mode
**Input:    none
**Output:   none
**********************************************************/
bool bGoSleep(void)
{
 byte tmp;
 
 vSpi3Write(((word)CMT23_MODE_CTL<<8)+MODE_GO_SLEEP);	
 Delay_ms(100);		//enough?
 tmp = (MODE_MASK_STA & bSpi3Read(CMT23_MODE_STA));	
 if(tmp==MODE_STA_SLEEP)
 	return(TRUE);
 else
 	return(FALSE);
}

/**********************************************************
**Name:     bGoStandby
**Function: Entry Standby Mode
**Input:    none
**Output:   none
**********************************************************/
bool bGoStandby(void)
{
 byte tmp, i;	
 
 RssiTrig = FALSE;
 vSpi3Write(((word)CMT23_MODE_CTL<<8)+MODE_GO_STBY);	
 for(i=0; i<50; i++)
 	{
 	Delay_us(400);	
	tmp = (MODE_MASK_STA & bSpi3Read(CMT23_MODE_STA));	
	if(tmp==MODE_STA_STBY)
		break;
	}
 if(i>=50)
 	return(FALSE);
 else
 	return(TRUE);
}

/**********************************************************
**Name:     vSoftReset
**Function: Software reset Chipset
**Input:    none
**Output:   none
**********************************************************/
void vSoftReset(void)
{
 vSpi3Write(((word)CMT23_SOFTRST<<8)+0xFF); 
 Delay_us(1000);				//enough?
}

/**********************************************************
**Name:     bReadStatus
**Function: read chipset status
**Input:    none
**Output:   none
**********************************************************/
byte bReadStatus(void)
{
 return(MODE_MASK_STA & bSpi3Read(CMT23_MODE_STA));		
}

/**********************************************************
**Name:     bReadRssi
**Function: Read Rssi
**Input:    true------dBm;
            false-----Code;
**Output:   none
**********************************************************/
byte bReadRssi(bool unit_dbm)
{
 if(unit_dbm)
 	return(bSpi3Read(CMT23_RSSI_DBM));
 else		
 	return(bSpi3Read(CMT23_RSSI_CODE));
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
void vGpioFuncCfg(byte io_cfg)
{
 vSpi3Write(((word)CMT23_IO_SEL<<8)+io_cfg);
}

/**********************************************************
**Name:     vIntSrcCfg
**Function: config interrupt source  
**Input:    int_1, int_2
**Output:   none
**********************************************************/
void vIntSrcCfg(byte int_1, byte int_2)
{
 byte tmp;
 tmp = INT_MASK & bSpi3Read(CMT23_INT1_CTL);
 vSpi3Write(((word)CMT23_INT1_CTL<<8)+(tmp|int_1));
 
 tmp = INT_MASK & bSpi3Read(CMT23_INT2_CTL);
 vSpi3Write(((word)CMT23_INT2_CTL<<8)+(tmp|int_2));
}

/**********************************************************
**Name:     vEnableAntSwitch
**Function:  
**Input:    
**Output:   none
**********************************************************/
void vEnableAntSwitch(byte mode)
{
 byte tmp;
 tmp = bSpi3Read(CMT23_INT1_CTL);
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
 vSpi3Write(((word)CMT23_INT1_CTL<<8)+tmp);
}


/**********************************************************
**Name:     vIntSrcEnable
**Function: enable interrupt source 
**Input:    en_int
**Output:   none
**********************************************************/
void  vEnablePLLcheck(void)
{
  byte tmp;
  tmp = bSpi3Read(CMT23_EN_CTL);
  tmp |= LD_STOP_EN; 	
  vSpi3Write(((word)CMT23_EN_CTL<<8)+tmp);
}

/**********************************************************
**Name:     vEnablePLLcheck
**Function: enable PLLcheck 
**Input:    en_int
**Output:   none
**********************************************************/
void vIntSrcEnable(byte en_int)
{
 vSpi3Write(((word)CMT23_INT_EN<<8)+en_int);				
}


/**********************************************************
**Name:     vIntSrcFlagClr
**Function: clear flag
**Input:    none
**Output:   equ CMT23_INT_EN
**********************************************************/
byte bIntSrcFlagClr(void)
{
 byte tmp;
 byte int_clr2 = 0;
 byte int_clr1 = 0;
 byte flg = 0;
 
 tmp = bSpi3Read(CMT23_INT_FLG);
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
 	
 vSpi3Write(((word)CMT23_INT_CLR2<<8)+int_clr2);	//Clear flag
 
 
 tmp = bSpi3Read(CMT23_INT_CLR1);
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
 vSpi3Write(((word)CMT23_INT_CLR1<<8)+int_clr1);	//Clear flag 
 
 return(flg);
}

/**********************************************************
**Name:     bClearFIFO
**Function: clear FIFO buffer
**Input:    none
**Output:   FIFO state
**********************************************************/
byte vClearFIFO(void)
{
 byte tmp;	
 tmp = bSpi3Read(CMT23_FIFO_FLG);
 vSpi3Write(((word)CMT23_FIFO_CLR<<8)+FIFO_CLR_RX+FIFO_CLR_TX);
 return(tmp);
}

void vEnableWrFifo(void)
{
 byte tmp;
 tmp = bSpi3Read(CMT23_FIFO_CTL);
 tmp |= (SPI_FIFO_RD_WR_SEL|FIFO_RX_TX_SEL);
 vSpi3Write(((word)CMT23_FIFO_CTL<<8)+tmp);
}

void vEnableRdFifo(void)
{
 byte tmp;
 tmp = bSpi3Read(CMT23_FIFO_CTL);
 tmp &= (~(SPI_FIFO_RD_WR_SEL|FIFO_RX_TX_SEL));
 vSpi3Write(((word)CMT23_FIFO_CTL<<8)+tmp);
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
void vInit(void)
{
 //byte i;
 byte tmp;
 bool tmp1;
 //word len;
 vSpi3Init();
 GPO3In();

 vSoftReset();
 Delay_ms(20);
tmp1 = bGoStandby();
if(tmp1 == FALSE)
{
	while(1);
}
 //	
 tmp = bSpi3Read(CMT23_MODE_STA);
 tmp|= EEP_CPY_DIS;
 tmp&= (~RSTN_IN_EN);			//Disable RstPin	
 vSpi3Write(((word)CMT23_MODE_STA<<8)+tmp);

 bIntSrcFlagClr();
 
}

void vCfgBank(word cfg[], byte length)
{
 byte i;
 
 if(length!=0)
 	{	
 	for(i=0; i<length; i++)	
 		vSpi3Write(cfg[i]);
 	}	
}


/******************************************************************************
**函数名称：bGetMessage
**函数功能：接收一包数据
**输入参数：无
**输出参数：非0——接收成功
**          0——接收失败
******************************************************************************/
byte bGetMessage(byte msg[])
{
 byte i;	
 
 vEnableRdFifo();	
 if(FixedPktLength)
 	{
  	vSpi3BurstReadFIFO(msg, PayloadLength);
	i = PayloadLength;
	}
 else
 	{
	i = bSpi3ReadFIFO();	
 	vSpi3BurstReadFIFO(msg, i);
 	}
 return(i);
}

byte bGetMessageByFlag(byte msg[])
{
 byte tmp;
 byte tmp1;
 byte rev = 0;
 tmp = bSpi3Read(CMT23_INT_FLG);
 if((tmp&SYNC_PASS_FLAG)&&(!RssiTrig))
 	{
 	PktRssi = bReadRssi(FALSE);
 	RssiTrig = TRUE;
 	}
 
 tmp1 = bSpi3Read(CMT23_CRC_CTL);
 vEnableRdFifo();	 
 if(tmp1&CRC_ENABLE)		//Enable CrcCheck
 	{
 	if(tmp&CRC_PASS_FLAG)
 		{
 		if(FixedPktLength)
 			{
  			vSpi3BurstReadFIFO(msg, PayloadLength);
			rev = PayloadLength;
			}
 		else
 			{	
			rev = bSpi3ReadFIFO();	
 			vSpi3BurstReadFIFO(msg, rev);
 			}
 		RssiTrig = FALSE;
 		}
 	}
 else
 	{
	if(tmp&RX_DONE_FLAG) 		
		{
 		if(FixedPktLength)
 			{
  			vSpi3BurstReadFIFO(msg, PayloadLength);
			rev = PayloadLength;
			}
 		else
 			{	
			rev = bSpi3ReadFIFO();	
 			vSpi3BurstReadFIFO(msg, rev);
 			}	
 		RssiTrig = FALSE;		
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
bool bSendMessage(byte msg[], byte length)
{
 //mode1
 //vSetTxPayloadLength(length);
 //bGoStandby();
 //vEnableWrFifo();	
 //vSpi3BurstWriteFIFO(msg, length);
 //bGoTx();
 
 //mode2
 bIntSrcFlagClr();  //清中断
 vSetTxPayloadLength(length);
 bGoTx();
 vEnableWrFifo();	
 vSpi3BurstWriteFIFO(msg, length);
 return(TRUE);
}

void vSetTxPayloadLength(word length)
{
 byte tmp;	
 byte len;
 bGoStandby();
 tmp = bSpi3Read(CMT23_PKT_CTRL1);
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
 vSpi3Write(((word)CMT23_PKT_CTRL1<<8)+tmp);
 vSpi3Write(((word)CMT23_PKT_LEN<<8)+(byte)len);	//Payload length
 //bGoSleep();
}
