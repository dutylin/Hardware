/**
  ******************************************************************************
  * @file    Project/STM8L15x_StdPeriph_Template/main.c
  * @author  MCD Application Team
  * @version V1.6.1
  * @date    30-September-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "common.h"
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "CMT2300drive.h"
#include "uart.h"
#include "sysclk.h"    
    
    
static bool statetx = TRUE;  //  false为RX  true为TX
/**/	extern bool  FixedPktLength;						//false: for contain packet length in Tx message, the same mean with variable lenth
//true : for doesn't include packet length in Tx message, the same mean with fixed length
/**/	extern word  PayloadLength;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

//void Device_Init(void);
void CLK_Init(void);
void setup_Tx(void);
void setup_Rx(void);
void CMT2300_Init();
void loop_Tx(void);
void loop_Rx(void);

#define LEN 21

byte str[LEN] = {'H','o','p','e','R','F',' ','R','F','M',' ','C','O','B','R','F','M','3','0','0','A'};
byte getstr[LEN+1];



/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */


/*****此处通过RFPDK软件导出，然后更改每个数组中的参数 *****/
/************************************************************
频点:  433.92Mhz
速率:  2.4Kpbs
频偏:  +/-10Khz
带宽:  +/-100khz
数据包格式:
		0xAAAAAAAAAAAAAAAA + 0x2DD4 +0x15 +"HopeRF RFM COBRFM300A" 

发射功率: 13dBm
**************************************************************/
word CMTBank[12] = {
					0x0000,
					0x0166,
					0x02EC,
					0x031C,
					0x04F0,
					0x0580,
					0x0614,
					0x0708,
					0x0891,
					0x0902,
					0x0A02,
					0x0BD0  //用于校准RSSI误差
				   };
				   
word SystemBank[12] = {
					0x0CAE,
					0x0DE0,
					0x0E35,
					0x0F00,
					0x1000,
					0x11F4,
					0x1210,
					0x13E2,
					0x1442,
					0x1520,
					0x1600,
					0x1781				  
					};

word FrequencyBank[8] = {  //CMT2300 频率设置
					0x1842,
					0x1971,
					0x1ACE,
					0x1B1C,
					0x1C42,
					0x1D5B,
					0x1E1C,
					0x1F1C					
						 };
						 
word DataRateBank[24] = {
					0x2032,
					0x2118,
					0x2200,
					0x2399,
					0x24E0,
					0x2569,
					0x2619,
					0x2705,
					0x289F,
					0x2939,
					0x2A29,
					0x2B29,
					0x2CC0,
					0x2D51,
					0x2E2A,
					0x2F53,
					0x3000,
					0x3100,
					0x32B4,
					0x3300,
					0x3400,
					0x3501,
					0x3600,
					0x3700
						};	   

word BasebandBank[29] = {
					0x3812, // 1byte RX_preamble  单位8bit
					0x3908, // 8byte TX_preamble  
					0x3A00,
					0x3BAA, //报文头内容为AA
					0x3C02,//同步字设置为1byte
					0x3D00,
					0x3E00,
					0x3F00,
					0x4000,
					0x4100,
					0x4200,//同步字00
					0x43D4,//同步字D4
					0x442D,//同步字2D
					0x4501, //可变包长
					0x461F, //31byte的负载
					0x4700, //Node ID 检测电路设置
					0x4800, //Node ID 缺省位
					0x4900, //Node ID 缺省位
					0x4A00, //Node ID 缺省位
					0x4B00, //Node ID 缺省位
					0x4C00,//CRC 禁止
					0x4D00,//CRC多项式初始值
					0x4E00,//CRC多项式初始值
					0x4F60, //CRC大小顺序
					0x50FF,//白化多项式使能
					0x5102,//PA打开 定义预发射内容，发1。
					0x5200,//重复发包的次数
					0x531F,//包与包的间隔 单位symbol
					0x5410 //发包完自动清除fifo	
						};	

word TXBank[11] = {
					0x5550,
					0x564D,
					0x5706,
					0x5800,
					0x5942,
					0x5AB0,
					0x5B00,
					0x5C37,
					0x5D0A,
					0x5E3F,
					0x5F7F															
					    };			
		

void main(void)
{
  SystemClock_Init();
  //CLK_Init();
  Delay_ms(200);
  CFG->GCR |= CFG_GCR_SWD;
   UART_Config();
  CMT2300_Init();
  if(FALSE == statetx )
  	{
  		setup_Rx();
		while(1)
	    {
          loop_Rx();
	    }
  	}
  else
  	{
  		
        setup_Tx(); 
		while (1)
		{
		   loop_Tx();
		}
  	}
  
}



/**
  * @brief Device Initialize configuration
  * @param None
  * @retval None
  */

void CMT2300_Init()
{
	/**********基础设置初始化一次即可*******/
	FixedPktLength    = FALSE;				
	PayloadLength     = LEN;	
	vInit();
	vCfgBank(CMTBank, 12);
	vCfgBank(SystemBank, 12);
	vCfgBank(FrequencyBank, 8);
	vCfgBank(DataRateBank, 24);
	vCfgBank(BasebandBank, 29);
	vCfgBank(TXBank, 11);
	vEnablePLLcheck();
	bGoSleep();  				//让配置生效
	/**************************************/

}

void setup_Tx(void)
{

	bGoStandby();   //进入配置模式
	
	vEnableAntSwitch(0);  //设置天线切换_IO口切换
	vGpioFuncCfg(GPIO1_INT1+GPIO2_INT2+GPIO3_DOUT); //IO口的映射
	vIntSrcCfg(INT_FIFO_NMTY_TX, INT_TX_DONE);    //IO口中断的映射
	vIntSrcEnable(TX_DONE_EN);           //中断使能        
	
	vClearFIFO();  //清除FIFO
	bGoSleep();    //进入睡眠，让配置生效

 
}

void setup_Rx(void)
{

	bGoStandby();   //进入配置模式
	vEnableAntSwitch(0); //为 1 时 GPIO1 和 GPIO2 不可用
	vGpioFuncCfg(GPIO1_INT1+GPIO2_INT2+GPIO3_DOUT);  //IO口的功能映射

	//vIntSrcCfg(INT_RSSI_VALID, INT_CRC_PASS);   //GPO3映射成CRC_pass中断，此处如果要用该中断，RFPDK需要配置CRC
	vIntSrcCfg(INT_FIFO_WBYTE_RX, INT_PKT_DONE);  //GPO3映射成PKT_DONE中断 //IO口中断的映射
	vIntSrcEnable(PKT_DONE_EN + CRC_PASS_EN);          //中断使能 
	
	vClearFIFO();
	bGoSleep();           //让配置生效
	bGoRx();              //for Rx

}


void loop_Tx()
{
	bSendMessage(str, LEN);
	while(GPO2_L());   // 判断GPIO中断 为低等 为高运行下面代码
	bIntSrcFlagClr();
	vClearFIFO(); 
	Delay_ms(200);
}

void loop_Rx()
{
	byte tmp;
	if(GPO2_H())
		{
		  bGoStandby();
		  tmp = bGetMessage(getstr);  //仿真到此能看到getstr收到的数据包
		  bIntSrcFlagClr();
		  vClearFIFO(); 
		  bGoRx();
		}	
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
