#include "B_SHT30A.h"


vid SHT30_DelayMs(uint8 k)
{
  do
  {
    SHT30_Delay1us(1000);
  }while(k--);
}

vid SHT30_Delay1us(uint16 vTemp)
{
  do
  {
    asm("nop"); //一个asm("nop")函数经过示波器测试代表100ns
    asm("nop");
    asm("nop");
    asm("nop");
   asm("nop");
   asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
   asm("nop");
  }while(vTemp--);
}

/*-------------------------------------------------------
功能描述：函数初始化
参    数：无
返 回 值：无
---------------------------------------------------------*/
vid SHT30_Init(vid)
{
  SHT30_SCL0_O;                          	//设置SCLK为输出
  SHT30_SDA0_O;                          	//设置SDA为输出
  
  SHT30_SCL0_H;
  SHT30_SCL0_L;
  
  SHT30_SDA0_H;
  SHT30_SDA0_L;
  
  SHT30_Soft_Reset();
  
  
  SHT30_DelayMs(1);
  
  SHT30_ClearStaus();
  
  SHT30_DelayMs(1);
  
  
}


uint16 SHT30_ReadSerialNumber()
{
  uint8 	vRval = 0x00;
  uint16 	vVal = 0x00;
  uint8 	vCrc = 0x00;
  
  uint8 i = 0;
  uint8 vBuf[3];
  
  SHT30_Start();
  vRval |= SHT30_SendByte(SHT30_SlaveAddress+0);								//地址写
  vRval |= SHT30_SendByte( 0x37 );			//读取状态寄存器
  vRval |= SHT30_SendByte( 0x80 );
  
  SHT30_Start();
  vRval |= SHT30_SendByte(SHT30_SlaveAddress+1);								//地址读
  
  for(i=0; i<3; i++)
  {
    vBuf[i] = SHT30_RecvByte();                				//存储数据
    if (i == 2)
    {
      
      SHT30_SendACK(1);                         			//最后一个数据需要回NOACK
    }
    else
    {		
      SHT30_SendACK(0);                         			//回应ACK
    }
  }
  
  SHT30_Stop();
  
  if (vRval>0)
  {
    return 0xFFFF;
  }
  
  vCrc = SHT30_CheckCrc8(vBuf, 2);
  if (vCrc == vBuf[2])
  {
    vVal |= vBuf[0];
    vVal <<= 8;
    vVal |= vBuf[1];
  }
  
  return vVal;
}


/*---------------------------------------------------------------------
功能描述: 起始信号
参数说明: 无	
函数返回: 无
---------------------------------------------------------------------*/
vid SHT30_Start()
{
  SHT30_SDA0_H;                         //拉高数据线
  SHT30_Delay1us(1);
  SHT30_SCL0_H;                         //拉高时钟线
  SHT30_Delay1us(1);                     //延时
  SHT30_SDA0_L;                         //产生下降沿
  SHT30_Delay1us(10);                     //延时
  SHT30_SCL0_L;                         //拉低时钟线
  SHT30_Delay1us(10);
}

/*---------------------------------------------------------------------
功能描述: 停止信号
参数说明: 无	
函数返回: 无
---------------------------------------------------------------------*/
vid SHT30_Stop()
{
  SHT30_SCL0_L;                         //拉低时钟线
  SHT30_Delay1us(1);
  SHT30_SDA0_L;                         //拉低数据线
  SHT30_Delay1us(1);                     //延时
  SHT30_SCL0_H;
  SHT30_Delay1us(10);                     //延时
  SHT30_SDA0_H;                         //产生上升沿
  SHT30_Delay1us(10);
}

/*---------------------------------------------------------------------
功能描述: 发送应答信号
参数说明: ack [in] - 应答信号(0:ACK 1:NAK)
函数返回: 无
---------------------------------------------------------------------*/
vid SHT30_SendACK(uint8 ack)
{
  if (ack&0x01)	SHT30_SDA0_H;		//写应答信号
  else	SHT30_SDA0_L;
  
  SHT30_Delay1us(1);
  SHT30_SCL0_H;                         //拉高时钟线
  SHT30_Delay1us(5);                     //延时
  SHT30_SCL0_L;                         //拉低时钟线
  SHT30_SDA0_H;
  SHT30_Delay1us(20);                     //延时
}

/*---------------------------------------------------------------------
功能描述: 接收应答信号
参数说明: 无
函数返回: 返回应答信号
---------------------------------------------------------------------*/
uint8 SHT30_RecvACK()
{
  uint8 CY = 0x00;
  uint16 vConter = 1000;
  SHT30_SDA0_H;
  
  SHT30_SDA0_I;
  vConter = 1000;
  SHT30_SCL0_H;                         //拉高时钟线
  SHT30_Delay1us(1);                     //延时
  while (vConter)
  {
    vConter--;
    CY |= SHT30_SDA0_DAT;                 //读应答信号
    
    if(!CY)	break;
  }
  SHT30_Delay1us(20);                     //延时
  SHT30_SCL0_L;
  SHT30_SDA0_O;
  
  return CY;
}

/*---------------------------------------------------------------------
功能描述: 向IIC总线发送一个字节数据
参数说明: dat [in] - 写字节
函数返回: 0 - 成功   1- 出错
---------------------------------------------------------------------*/
uint8 SHT30_SendByte(uint8 dat)
{
  uint8 vRval = 0x01;
  uint8 i;
  
  for (i=0; i<8; i++)         			//8位计数器
  {
    if (dat&0x80)	SHT30_SDA0_H;
    else	SHT30_SDA0_L;                   //送数据口
    
    SHT30_Delay1us(1);             		//延时
    SHT30_SCL0_H;                		//拉高时钟线
    SHT30_Delay1us(5);             		//延时
    SHT30_SCL0_L;                		//拉低时钟线
    SHT30_Delay1us(1);             		//延时
    dat <<= 1;              			//移出数据的最高位
  }
  
  vRval = SHT30_RecvACK();
  
  return vRval;
}

/*---------------------------------------------------------------------
功能描述: 从IIC总线接收一个字节数据
参数说明: 无
函数返回: 接收字节
---------------------------------------------------------------------*/
uint8 SHT30_RecvByte()
{
  uint8 i;
  uint8 dat = 0;
  uint16 vConter = 100;
  
  SHT30_SDA0_H;                         //使能内部上拉,准备读取数据,
  
  SHT30_SDA0_I;
  for (i=0; i<8; i++)         	        //8位计数器
  {
    SHT30_SCL0_H;                       //拉高时钟线
    SHT30_Delay1us(1);             			//延时
    vConter = 100;
    SHT30_SCL0_I;
    while (SHT30_SCL0_DAT==0)
    {
      vConter--;
      if (vConter == 0) break;
    }
    SHT30_SCL0_O;
    SHT30_Delay1us(3);
    
    dat |= SHT30_SDA0_DAT;              //读数据             
    SHT30_SCL0_L;                       //拉低时钟线
    SHT30_Delay1us(1);             			//延时
    
    if (i<7) dat <<= 1;	
  }
  SHT30_SDA0_O;
  
  return dat;
}


/*---------------------------------------------------------------------
功能描述: SHT30单次测量
参数说明: vBuf [out] - 测量读取结果
函数返回: 0 - 成功  大于1出错
---------------------------------------------------------------------*/
uint8 SHT30_Single_Measure(uint8 *vBuf)
{
  uint8 vRval = 0;
  uint8 i = 0;
  
  SHT30_Start();
  
  vRval |= SHT30_SendByte(SHT30_SlaveAddress+0);					//地址写
  if (!vRval) vRval |= SHT30_SendByte( (SHT30_SINGLE_H_MEASURE_EN>>8)&0xFF );				//使能高精度采集
  if (!vRval) vRval |= SHT30_SendByte( (SHT30_SINGLE_H_MEASURE_EN)&0xFF );
  SHT30_Stop();
  
  if (vRval)	return vRval;
  
  SHT30_SCL0_H;
  SHT30_DelayMs(15);														//15Ms
  
  
  SHT30_Start();
  if (!vRval) vRval |= SHT30_SendByte(SHT30_SlaveAddress+1);					//地址读
  
  if (vRval)	return vRval;
  
  for(i=0; i<6; i++)
  {
    vBuf[i] = SHT30_RecvByte();                	//存储数据
    if (i == 5)
    {
      
      SHT30_SendACK(1);                         //最后一个数据需要回NOACK
    }
    else
    {		
      SHT30_SendACK(0);                         //回应ACK
    }
  }
  
  SHT30_Stop();
  
  return vRval;
}

/*---------------------------------------------------------------------
功能描述: SHT30设置周期测量
参数说明: vBuf [in] - 测量命令
函数返回: 0 - 成功  大于1出错
---------------------------------------------------------------------*/
uint8 SHT30_Periodic_Measure(uint16 vCmd)
{
  uint8 vRval = 0;
  
  SHT30_Start();
  vRval |= SHT30_SendByte(SHT30_SlaveAddress+0);					//地址写
  if (!vRval) vRval |= SHT30_SendByte( (vCmd>>8)&0xFF );
  if (!vRval) vRval |= SHT30_SendByte( (vCmd)&0xFF );
  SHT30_Stop();
  
  return vRval;
}

/*---------------------------------------------------------------------
功能描述: SHT30周期测量 读取数据
参数说明: vBuf [out] - 测量命令
函数返回: 0 - 成功  大于1出错
---------------------------------------------------------------------*/
uint8 SHT30_Periodic_Measure_Read(uint8 *vBuf)
{
  uint8 vRval = 0;
  
  uint8 i = 0;
  
  SHT30_Start();
  vRval |= SHT30_SendByte(SHT30_SlaveAddress+0);					//地址写
  if (!vRval) vRval |= SHT30_SendByte( (SHT30_PERIODOC_MEASURE_READ>>8)&0xFF );				//读取周期采集数据
  if (!vRval) vRval |= SHT30_SendByte( (SHT30_PERIODOC_MEASURE_READ)&0xFF );
  
  SHT30_Start();
  if (!vRval) vRval |= SHT30_SendByte(SHT30_SlaveAddress+1);					//地址读
  
  if (vRval)	return vRval;
  
  for(i=0; i<6; i++)
  {
    vBuf[i] = SHT30_RecvByte();                	//存储数据
    if (i == 5)
    {
      
      SHT30_SendACK(1);                         //最后一个数据需要回NOACK
    }
    else
    {		
      SHT30_SendACK(0);                         //回应ACK
    }
  }
  
  SHT30_Stop();
  
  return vRval;
}

/*---------------------------------------------------------------------
功能描述: SHT30传感器软复位 (周期模式下关闭周期运行)
参数说明: 无
函数返回: 0 - 成功  大于1出错
---------------------------------------------------------------------*/
uint8 SHT30_Soft_Reset()
{
  uint8 vRval = 0;
  
  SHT30_Start();
  
  if (!vRval) vRval |= SHT30_SendByte(SHT30_SlaveAddress+0);					//地址写
  if (!vRval) vRval |= SHT30_SendByte( (SHT30_SOFT_RESET>>8)&0xFF );
  if (!vRval) vRval |= SHT30_SendByte( (SHT30_SOFT_RESET)&0xFF );
  SHT30_Stop();
  
  return vRval;
}

/*---------------------------------------------------------------------
功能描述: SHT30加热命令
参数说明: vCmd [in] - 0x306D打开加热  0x3066关闭加热
函数返回: 0 - 成功  大于1出错
---------------------------------------------------------------------*/
uint8 SHT30_Heater(uint16 vCmd)
{
  uint8 vRval = 0;
  
  SHT30_Start();
  
  if (!vRval) vRval |= SHT30_SendByte(SHT30_SlaveAddress+0);					//地址写
  if (!vRval) vRval |= SHT30_SendByte( (vCmd>>8)&0xFF );
  if (!vRval) vRval |= SHT30_SendByte( (vCmd)&0xFF );
  SHT30_Stop();
  
  return vRval;
}

/*---------------------------------------------------------------------
功能描述: SHT30获取状态寄存器
参数说明: 无
函数返回: 返回状态寄存器
---------------------------------------------------------------------*/
uint16 SHT30_ReadStaus()
{
  uint8 	vRval = 0x00;
  uint16 	vVal = 0x00;
  uint8 	vCrc = 0x00;
  
  uint8 i = 0;
  uint8 vBuf[3];
  
  SHT30_Start();
  if (!vRval) vRval |= SHT30_SendByte(SHT30_SlaveAddress+0);								//地址写
  if (!vRval) vRval |= SHT30_SendByte( (SHT30_READ_STATUS>>8)&0xFF );				//读取状态寄存器
  if (!vRval) vRval |= SHT30_SendByte( (SHT30_READ_STATUS)&0xFF );
  
  SHT30_Start();
  if (!vRval) vRval |= SHT30_SendByte(SHT30_SlaveAddress+1);								//地址读
  
  if (vRval>0)
  {
    return 0xFFFF;
  }
  
  for(i=0; i<3; i++)
  {
    vBuf[i] = SHT30_RecvByte();                				//存储数据
    if (i == 2)
    {
      
      SHT30_SendACK(1);                         			//最后一个数据需要回NOACK
    }
    else
    {		
      SHT30_SendACK(0);                         			//回应ACK
    }
  }
  
  SHT30_Stop();
  
  if (vRval>0)
  {
    return 0xFFFF;
  }
  
  vCrc = SHT30_CheckCrc8(vBuf, 2);
  if (vCrc == vBuf[2])
  {
    vVal |= vBuf[0];
    vVal <<= 8;
    vVal |= vBuf[1];
  }
  
  return vVal;
}

/*---------------------------------------------------------------------
功能描述: SHT30清楚状态寄存器
参数说明: 无
函数返回: 0 - 成功  大于1出错
---------------------------------------------------------------------*/
uint8 SHT30_ClearStaus()
{
  uint8 vRval = 0;
  SHT30_Start();
  
  if (!vRval) vRval |= SHT30_SendByte(SHT30_SlaveAddress+0);					//地址写
  if (!vRval) vRval |= SHT30_SendByte( (SHT30_CLEAR_STATUS>>8)&0xFF );
  if (!vRval) vRval |= SHT30_SendByte( (SHT30_CLEAR_STATUS)&0xFF );
  SHT30_Stop();
  
  return vRval;
}

uint8 SHT30_CheckCrc8(uint8 *vDat, uint8 vLen)  
{  
  uint8 vBit;        // bit mask
  uint8 vCrc = 0xFF; // calculated checksum
  uint8 vByteCtr;    // byte counter
  
  // calculates 8-Bit checksum with given polynomial
  for(vByteCtr = 0; vByteCtr < vLen; vByteCtr++)
  {
    vCrc ^= (vDat[vByteCtr]);
    for(vBit = 8; vBit > 0; --vBit)
    {
      if(vCrc & 0x80) vCrc = (vCrc << 1) ^ POLYNOMIAL;
      else           vCrc = (vCrc << 1);
    }
  }
  
  return vCrc;
}

/*---------------------------------------------------------------------
功能描述: SHT30 测量结果计算
参数说明:  vBuf [in] - 测量读取结果
vTemSymbol [out] - 返回温度符号  0-负温度  1-正温度
vTem [out] - 温度
vHum [out] - 湿度

函数返回: 无
---------------------------------------------------------------------*/
void SHT30_calc(uint8 *vBuf, uint8 *vTemSymbol, uint16 *vTem, uint16 *vHum)
{
  uint16 	vVal = 0x00;
  uint8 	vCrc = 0x00;
  float		vTemp = 0.00;
  
  //温度
  vCrc = SHT30_CheckCrc8(vBuf, 2);
  if (vCrc == vBuf[2])
  {
    vVal = vBuf[0];
    vVal<<=8;
    vVal |= vBuf[1];
    
    vTemp = 175.0*vVal/(65536.0-1.0);
    
    if (vTemp >= 45)
    {
      *vTemSymbol = 1;
      *vTem = (vTemp - 45.0)*10.0;
    }
    else
    {
      *vTemSymbol = 0;
      *vTem = (45.0 - vTemp)*10.0;
    }
    
  }
  
  vBuf += 3;
  vVal = 0x00;
  vCrc = SHT30_CheckCrc8(vBuf, 2);
  if (vCrc == vBuf[2])
  {
    vVal = vBuf[0];
    vVal<<=8;
    vVal |= vBuf[1];
    
    vTemp = 100.0*vVal/(65536.0-1.0);
    *vHum = (vTemp*10);
  }
}

/*---------------------------------------------------------------------
功能描述: SHT30 测量结果计算
参数说明:  vTemSymbol [out] - 返回温度符号 0-负温度  1-正温度
vTem [out] - 温度
vHum [out] - 湿度

函数返回: 无
---------------------------------------------------------------------*/
void SHT30_Get_TH(uint8 *vTemSymbol, uint16 *vTem, uint16 *vHum)
{
  uint8 vDat[8];
  uint8 vRval = 0;
  
  vRval = SHT30_Single_Measure(vDat);
  if (!vRval) SHT30_calc(vDat, vTemSymbol, vTem, vHum);
  
}





