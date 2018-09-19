//***************************************
// B_LUX_V30采集程序
//****************************************
#include "B_LUX_V30.h"


/*---------------------------------------------------------------------
 功能描述: 延时纳秒 不同的工作环境,需要调整此函数
 参数说明: 无	
 函数返回: 无
 ---------------------------------------------------------------------*/
vid B_LUX_delay_nms(uint16 k)	
{						
  uint16 i,j;				
  for(i=0;i<k;i++)
  {			
    for(j=0;j<2000;j++)			
    {
      ;
    }
  }						
}					

/*---------------------------------------------------------------------
 功能描述: 延时5微秒  不同的工作环境,需要调整此函数
 参数说明: 无	
 函数返回: 无
 ---------------------------------------------------------------------*/
vid B_LUX_Delay5us()
{
  asm("nop"); //一个asm("nop")函数经过示波器测试代表100ns
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
}

/*---------------------------------------------------------------------
 功能描述: 延时5毫秒  不同的工作环境,需要调整此函数
 参数说明: 无	
 函数返回: 无
 ---------------------------------------------------------------------*/
vid B_LUX_Delay5ms()
{
  uint16 n = 20000;
  
  while (n--);
}

/*---------------------------------------------------------------------
 功能描述: 起始信号
 参数说明: 无	
 函数返回: 无
 ---------------------------------------------------------------------*/
vid B_LUX_Start()
{
  B_LUX_SDA0_H;                         //拉高数据线
  B_LUX_SCL0_H;                         //拉高时钟线
  B_LUX_Delay5us();                     //延时
  B_LUX_SDA0_L;                         //产生下降沿
  B_LUX_Delay5us();                     //延时
  B_LUX_SCL0_L;                         //拉低时钟线
}

/*---------------------------------------------------------------------
 功能描述: 停止信号
 参数说明: 无	
 函数返回: 无
 ---------------------------------------------------------------------*/
vid B_LUX_Stop()
{
  B_LUX_SDA0_L;                         //拉低数据线
  B_LUX_SCL0_H;                         //拉高时钟线
  B_LUX_Delay5us();                     //延时
  B_LUX_SDA0_H;                         //产生上升沿
  B_LUX_Delay5us();                     //延时
  B_LUX_SCL0_L;
  B_LUX_Delay5us();
}

/*---------------------------------------------------------------------
 功能描述: 发送应答信号
 参数说明: ack - 应答信号(0:ACK 1:NAK)
 函数返回: 无
 ---------------------------------------------------------------------*/
vid B_LUX_SendACK(uint8 ack)
{
  if (ack&0x01)	B_LUX_SDA0_H;		//写应答信号
  else	B_LUX_SDA0_L;
  
  B_LUX_SCL0_H;                         //拉高时钟线
  B_LUX_Delay5us();                     //延时
  B_LUX_SCL0_L;                         //拉低时钟线
  B_LUX_Delay5us();
  B_LUX_SDA0_H;
  B_LUX_Delay5us();                     //延时
}

/*---------------------------------------------------------------------
 功能描述: 接收应答信号
 参数说明: 无
 函数返回: 返回应答信号
 ---------------------------------------------------------------------*/
uint8 B_LUX_RecvACK()
{
  uint8 CY = 0x00;
  B_LUX_SDA0_H;
  
  B_LUX_SDA0_I;
  
  B_LUX_SCL0_H;                         //拉高时钟线
  B_LUX_Delay5us();                     //延时
  
  
  CY |= B_LUX_SDA0_DAT;                 //读应答信号
  
  B_LUX_Delay5us();                     //延时
  
  B_LUX_SCL0_L;                         //拉低时钟线
  
  B_LUX_SDA0_O;
  
  return CY;
}

/*---------------------------------------------------------------------
 功能描述: 向IIC总线发送一个字节数据
 参数说明: dat - 写字节
 函数返回: 无
 ---------------------------------------------------------------------*/
vid B_LUX_SendByte(uint8 dat)
{
  uint8 i;
  
  for (i=0; i<8; i++)         			//8位计数器
  {
    if (dat&0x80)	B_LUX_SDA0_H;
    else	B_LUX_SDA0_L;                   //送数据口
    
    B_LUX_Delay5us();             		//延时
    B_LUX_SCL0_H;                		//拉高时钟线
    B_LUX_Delay5us();             		//延时
    B_LUX_SCL0_L;                		//拉低时钟线
    B_LUX_Delay5us();             		//延时
    dat <<= 1;              			//移出数据的最高位
  }
  
  B_LUX_RecvACK();
}

/*---------------------------------------------------------------------
 功能描述: 从IIC总线接收一个字节数据
 参数说明: 无
 函数返回: 接收字节
 ---------------------------------------------------------------------*/
uint8 B_LUX_RecvByte()
{
  uint8 i;
  uint8 dat = 0;
  B_LUX_SDA0_I;
  
  B_LUX_SDA0_H;                         //使能内部上拉,准备读取数据,
  for (i=0; i<8; i++)         	        //8位计数器
  {
    
    
    B_LUX_SCL0_H;                       //拉高时钟线
    B_LUX_Delay5us();             	//延时
    dat |= B_LUX_SDA0_DAT;              //读数据               
    B_LUX_SCL0_L;                       //拉低时钟线
    B_LUX_Delay5us();             	//延时
    
    if (i<7)
      dat <<= 1;
    	
  }
  B_LUX_SDA0_O;
  
  return dat;
}

/*---------------------------------------------------------------------
 功能描述: 写MAX44009
 参数说明: REG_Address - 寄存器地址
 函数返回: 无
 ---------------------------------------------------------------------*/
vid B_LUX_Single_Write(uint8 REG_Address, uint8 REG_data)
{
  B_LUX_Start();                                        //起始信号
  B_LUX_SendByte(B_LUX_SlaveAddress);                   //发送设备地址+写信号
  B_LUX_SendByte(REG_Address);                          //内部寄存器地址，请参考中文pdf22页 
  B_LUX_SendByte(REG_data);                             //内部寄存器数据，请参考中文pdf22页 
  B_LUX_Stop();                                         //发送停止信号
  B_LUX_Delay5ms();
}

/*---------------------------------------------------------------------
 功能描述: 读MAX44009内部数据
 参数说明: 无
 函数返回: 无
 ---------------------------------------------------------------------*/
uint8 B_LUX_read(uint8 REG_Address)
{  
  uint8 rval = 0;
  B_LUX_Start();                                        //起始信号
  B_LUX_SendByte(B_LUX_SlaveAddress+0);                 //发送设备地址+读信号
  B_LUX_SendByte(REG_Address);                          //内部寄存器地址，请参考中文pdf22页 
  B_LUX_Start();                                        //起始信号
  B_LUX_SendByte(B_LUX_SlaveAddress+1);                 //发送设备地址+读信号
  rval = B_LUX_RecvByte();                              //BUF[0]存储0x32地址中的数据
  B_LUX_SendACK(1);                                     //回应ACK
  B_LUX_Stop();                                         //停止信号
  B_LUX_Delay5ms();
  
  return rval;
  
}

/*---------------------------------------------------------------------
 功能描述: 读MAX44009内部数据
 参数说明: 无
 函数返回: 无
 ---------------------------------------------------------------------*/
vid B_LUX_Multi_read(uint8 REG_Address1, uint8 REG_Address2, uint8 *vBuf)
{   	
  //寄存器1
  B_LUX_Start();                                        //起始信号
  B_LUX_SendByte(B_LUX_SlaveAddress+0);                 //发送设备地址+读信号
  B_LUX_SendByte(REG_Address1);                         //内部寄存器地址，请参考中文pdf22页 
  B_LUX_Start();                                        //起始信号
  B_LUX_SendByte(B_LUX_SlaveAddress+1);                 //发送设备地址+读信号
  vBuf[0] = B_LUX_RecvByte();                           //BUF[0]存储0x32地址中的数据
  B_LUX_SendACK(1);

  //连续寄存器2
  B_LUX_Start();                                        //起始信号
  B_LUX_SendByte(B_LUX_SlaveAddress+0);                 //发送设备地址+读信号
  B_LUX_SendByte(REG_Address2);                         //内部寄存器地址，请参考中文pdf22页 
  B_LUX_Start();                                        //起始信号
  B_LUX_SendByte(B_LUX_SlaveAddress+1);                 //发送设备地址+读信号
  vBuf[1] = B_LUX_RecvByte();                           //BUF[0]存储0x32地址中的数据
  B_LUX_SendACK(1);                                     //回应ACK
  
  
  B_LUX_Stop();                                         //停止信号
  B_LUX_Delay5ms();
}

/*---------------------------------------------------------------------
 功能描述: 传感器寄存器配置函数
 参数说明: val - 配置参数
 函数返回: 无
 ---------------------------------------------------------------------*/
vid B_Lux_RegCfg(uint8 val)
{
  uint8 valCfg = 0;
  
  valCfg = (0x00<<7)|(0x00<<6)|(0x00<<3)|(0x00<<0);               //默认800ms采集一次   //配置为自动动模式  //配置积分时间800ms (光照精度0.045Lux  最大光照度 188006.4Lux)                      
  
  if (val&0x40)                //采集方式
  {
    valCfg |= 0x40;
  }
  if (val&0x40)                //采集模式 0-自动模式  1-手动模式
  {
    valCfg |= 0x40;
  }
  if (val&0x08)                //分流比
  {
    valCfg |= 0x08;
  }
  if (val&0x04)                //积分时间
  {
    valCfg |= 0x04;
  }
  if (val&0x02)
  {
    valCfg |= 0x02;
  }
  if (val&0x01)
  {
    valCfg |= 0x01;
  }
  
  B_LUX_Single_Write(0x02, valCfg);                               //配置参数
}

/*---------------------------------------------------------------------
 功能描述: 初始化光照传感器
 参数说明: 无
 函数返回: 无
 ---------------------------------------------------------------------*/
vid B_LUX_Init()
{
  
  B_LUX_SCL0_O;
  B_LUX_SDA0_O;
  B_LUX_SCL0_H;
  B_LUX_SDA0_H;
  
  B_LUX_delay_nms(100);	                                          //延时100ms
  
  B_Lux_RegCfg(0x00);
  
}

  
/*---------------------------------------------------------------------
 功能描述: 光照读取函数
 参数说明: 无
 函数返回: 返回光照值
 ---------------------------------------------------------------------*/
vid B_LUX_GetLux(uint32 *vLux)
{
  uint8   vIndex = 0;                     //指数
  uint8   vMantissa = 0;                  //尾数
  uint8   vLuxBuf[3];
  uint16  val16 = 0;
  fp32    vflux = 0.0;
  
  B_LUX_Multi_read(0x03, 0x04, vLuxBuf);
  
  //指数
  vIndex = vLuxBuf[0]&0xF0;
  vIndex >>= 4;
  
  //尾数
  vMantissa   = (vLuxBuf[0]&0x0F);
  vMantissa <<= 4;
  vMantissa  |= (vLuxBuf[1]&0x0F);
  
  if (vIndex == 0x01)
  {
    vIndex = 0x00;
  }
  else if (vIndex > 0x0E)
  {
    vIndex = 0x0E;
  }
  
  val16 = (0x01<<vIndex);
  
  vflux = ( (fp32)val16 ) * ( (fp32)vMantissa) * 0.045;
  
  (*vLux) = (uint32)(vflux*1000.0);                      //*1000 小数位
  //(*vLux) = (*vLux) * 1.4;                               //小球透明外壳矫正
  B_LUX_delay_nms(1000);                                 //延时1s
  
} 

