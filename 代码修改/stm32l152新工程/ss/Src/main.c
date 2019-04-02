/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "rtc.h"
#include "m25p64.h"
#include "Flash_BSP.h"
#include<string.h>
#define GLOBALS
#include "global.h"
#include "adc.h"
#include "other.h"
#include "CC1110.h"
#include "spi.h"
#include "flash.h"
#include "infrared.h"
#include "oled.h"
#define Select_OLED       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET); 


/***************************预编译注意*****************************
在编译选择传感器类型时，在编译器Options For Target里的宏定义里修改
/***************************传感器变量相关****************************/
extern uint32_t CV_Value[5];
//***************************传感器编译相关**************************/
#ifdef  Liceng
uint8_t Senor_Type=0xE1;
#elif   Maogan
uint8_t Senor_Type=0xE2;
#elif   Maosuo
uint8_t Senor_Type=0xE3;
#elif   Weiyan
uint8_t Senor_Type=0xE4;
#elif   Yali
uint8_t Senor_Type=0xE5;
#endif
extern void LCD_P6x8Str(unsigned char x, unsigned char y,unsigned char ch[]);
extern void LCD_P8x16Str(unsigned char x, unsigned char y,unsigned char ch[]);
extern void LCD_P16x16Ch(unsigned char x, unsigned char y, unsigned char N, const unsigned char * ch);

/**********************传感器数据存取相关**************************/
uint32_t Flash_Read_Buff[4],Flash_Write_Buff[4];
uint32_t ID_Read_Buff[4],ID_Write_Buff[4];
/**************************定时计数相关****************************/
extern uint8_t Timer;
/**************************红外相关*************************/
uint8_t  SET_START,curse_ok;
uint8_t curse=0x28,curse_base=0x04,curse_tmp=0x00;
uint32_t k_code=0x00000000;
uint8_t Senor_IDNUM_Temp=0;
SENOR *pObj;
typedef void MENU_CALLBK(SENOR *pObj);
void Infrared_Fun(SENOR *pObj,MENU_CALLBK *_cbm,MENU_CALLBK *_cbo);
extern unsigned char Infrared_Status;
/**************************功能函数*************************/
extern WPStatus CC1100_WAKEUP_Flag;
extern int xinxi;
extern WPStatus PD_WAKEUP_Flag;
void SendMsg_Int(CC1110Tx_Msg *pMsg,SENOR *pObj);
void AD_Smapling_Function(SENOR *pObj);
void PD_WAKEUP_Int(void);
void AD_Display_Function(SENOR *pObj);
void SPI_ConfigurationOLED(void);
SENOR SenMsg;
extern unsigned char fd0[];
extern unsigned char fd1[];
extern unsigned char fd2[];
extern unsigned char BMP[];
char r[5];
char CV1[5];
char CV11[5];
char CV2[5];
char CV22[5];
char CV3[5];
char CV33[5];
uint32_t VVV1;
uint32_t VVV11;
uint32_t VVV2;
uint32_t VVV22;
uint32_t VVV3;
uint32_t VVV33;
void CC1110_Sendd(void);
void CC1110_Senddata(void);
void CC1110_Sendnum(void);
void CC1110_Senddd(void);
void CC1110_Sendbianhao(void);
void peizhiwuxian(void);
extern uint8_t vid1,vid2,vid3,vid4,oldid,newid;
//数据显示转换部分
char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;
    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }
    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }
    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }
    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }
    /* Null terminate the string. */
    *ptr = 0;
    return string;
} /* NCL_Itoa */
/**************************************************************
函数：void SendMsg_Int(CC1110Tx_Msg *pMsg,SENOR *pObj)
功能：无线发送初始信息
**************************************************************/
void SendMsg_Int(CC1110Tx_Msg *pMsg,SENOR *pObj)
{
    int j;
    memset(pMsg->Invalid,0xFE,5);
    pMsg->Start_Flag=0xAA;
    pMsg->ID=pObj->Senor_ID;
    pMsg->Type=Senor_Type;
    pMsg->Channel=pObj->Senor_Num;
    pMsg->Battery_Power =Battery_Monitor();
    pMsg->End_Flag=0x55;
    FLASH_Read(((uint32_t)0x08080020),((uint32_t)0x08080030),CV_Value);
    for(j=0; j<4; j++)
    {
    //    pMsg->T_Data_H[j]=(uint8_t)CV_Value[j]>>8;
    //    pMsg->T_Data_L[j]=(uint8_t)CV_Value[j]&0x00000ff;
			pMsg->T_Data_H[j]=0x00;//55
			pMsg->T_Data_L[j]=0x04;//99
    }
}
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint32_t  fac_id; 
unsigned char txbuffer[16];
unsigned char rxbuffer[16];
u8 *SpiFlashReadBuffer;
extern unsigned char rtc_wakeup_flag;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
extern unsigned char M25PXX_BUFFER[1024];
extern uint32_t WriteAddressPostion;
RTC_SetTypeDef RTC_SetStructure;
RTC_TimeDateTypeDef  RTC_TimeDateStructure2;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;

IWDG_HandleTypeDef hiwdg;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_RTC_Init(void);
static void MX_IWDG_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);
static void MX_ADC_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
u32 addr;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	u16 j,i;
	
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_RTC_Init();
  //MX_IWDG_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_ADC_Init();
  MX_USART3_UART_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
	//HAL_PWR_EnterSTOPMode
	CC1110Tx_Msg TxMessage;
	// PWR_FastWakeUpCmd(ENABLE);//唤醒片内正常工作电源
	HAL_PWREx_EnableFastWakeUp();
	__HAL_RCC_PWR_CLK_ENABLE();
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//片内电源管理的时钟---使能 
	HAL_PWREx_EnableUltraLowPower();//配置低电源使能
	CC1100_WAKEUP_Flag=Low;
	PD_WAKEUP_Flag=Low;

	NVIC_Config();
	HAL_RCC_DeInit();
	//EnterLowPower();
	SenMsg.Senor_ID=1;
	SenMsg.Senor_Num=4;
	ControlGPIO_Initt();
	//GPIO_SetBits(GPIOC,GPIO_Pin_8);//拉高
	//GPIO_ResetBits(GPIOC,GPIO_Pin_7);//拉低 
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);	
	SPI_Configuration();	
	Select_OLED

	ID_Write_Buff[0]=0x01;
	ID_Write_Buff[1]=0x00;
	ID_Write_Buff[2]=0x40;
	ID_Write_Buff[3]=0x0B;
	FLASH_Write(((uint32_t)0x08080040),((uint32_t)0x08080050),ID_Write_Buff);

	Flash_Write_Buff[0]=0x000000AA;
	Flash_Write_Buff[1]=0x0B;
	Flash_Write_Buff[2]=pObj->Senor_Num;
	Flash_Write_Buff[3]=0x00000055;
	FLASH_Write(((uint32_t)0x08080000),((uint32_t)0x08080010),Flash_Write_Buff);

	//设置时间为2019年1月2日第3周，上午10时12分4秒

	RTC_SetStructure.year =0x19;
	RTC_SetStructure.month = 0x01;
	RTC_SetStructure.date = 0x02;
	RTC_SetStructure.week = 0x03;
	RTC_SetStructure.hour = 0x10;
	RTC_SetStructure.minute =0x12;
	RTC_SetStructure.second =0x04;	
	RTC_SetStructure.wakeup_time = 0x05;//以秒为单位，当前为5s
	SYS_RTCInit(RTC_SetStructure);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
	while(fac_id!=M25P32)
	fac_id=M25PXX_ReadID();
	for(i=1;i<17;i++)
	txbuffer[i-1]=i;
	// txbuffer[15] = 0xa5;
	//M25PXX_Erase_Chip();//测试时用，正常工作时屏蔽该功能
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		RTC_TimeDateStructure2=RTC_GetTimeDate();
		txbuffer[0] = RTC_TimeDateStructure2.year;
		txbuffer[1] = RTC_TimeDateStructure2.month;
		txbuffer[2] = RTC_TimeDateStructure2.date;
		txbuffer[3] = RTC_TimeDateStructure2.hour;
		txbuffer[4] = RTC_TimeDateStructure2.minute;
		txbuffer[5] = RTC_TimeDateStructure2.second;		
  
    //memcpy(&txbuffer[6],adcvalue,3);	//拷贝adc数据到buffer中	
		
		/***************************************************************************************
		    spi flash可以进行扇区擦除和整片擦除操作
		    spi flash存储结构：     
														sectors counts：64
														sector：256pages
														page：256 bytes
		
				验证spi flash通信是否正常,地址范围0-0x3fffff
				WriteAddressPostion表明当前spi flash存储数据的数量，该变量保存在cpu内部flash中
				M25PXX_BUFFER,从spi flash中读取的数据按顺序保存在该数组中		
		****************************************************************************************/

		HAL_IWDG_Refresh(&hiwdg);
		SPI_FLASH_Write(txbuffer,0,9);//地址可以不指定，驱动中地址自动递增
		if( Read_AddressWrite()>=0x400000)//flash存满，采取整片擦除操作
		{
			M25PXX_Erase_Chip();	
			WriteAddressPostion	= 0;
      //增加内部flash写函数保存WriteAddressPostion			
		}
		SPI_FLASH_READ(SpiFlashReadBuffer,WriteAddressPostion,9);//读spi flash数据缓存在M25PXX_BUFFER中

//		for(j=0;j<64;j++)
//		{
//			for(i=0;i<256;i++)
//			{		
//				SPI_FLASH_READ(SpiFlashReadBuffer,addr,16);
//				SPI_FLASH_Write(txbuffer,addr,16);	
//				SPI_FLASH_READ(SpiFlashReadBuffer,addr,16);
//				addr = addr +256;	
//			}
//			M25PXX_Erase_Sector(i*256*256);
//						
//		}
		
		
		
		
		
		
		EnterLowPower();

  


		//Wake_Config();
		Bsp_Config();
		//PD_WAKEUP_Flag=Low;

		/******************初始读取flash 传感器ID和探头数量*********************/
		FLASH_Read(((uint32_t)0x08080000),((uint32_t)0x08080010),Flash_Read_Buff);

		if(*Flash_Read_Buff==0x000000AA&&*(Flash_Read_Buff+3)==0x00000055)
		{
			SenMsg.Senor_ID=*(Flash_Read_Buff+1);
			SenMsg.Senor_Num=*(Flash_Read_Buff+2);			  
		}			
		/*********如果是无光则发送 发送后无线模块进入待机，主板开始采集存储后待机*******/
		if((PD_WAKEUP_Flag<<1|CC1100_WAKEUP_Flag)==1)
		{ 
			PD_WAKEUP_Int();	
			CC1110_Config((uint32_t)115200);	
			SendMsg_Int(&TxMessage,&SenMsg);
			CC1110_Send_Flag=1;				 
			if(CC1110_Send_Flag!=Low)
			{
				if(xinxi==1)
				{ 
				//CC1110_Sendd();
					xinxi=0;
				}
				CC1110_Send_Flag=Low;
			}

			AD_Smapling_Function(&SenMsg);
			CC1100_WAKEUP_Flag=Low;
			PD_WAKEUP_Flag=Low;
			//ControlGPIO_Init();
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
			//GPIO_SetBits(GPIOC,GPIO_Pin_8);
			//delay_ms(200);delay_ms(200);
		}
		else if((PD_WAKEUP_Flag<<1|CC1100_WAKEUP_Flag)==2)//||rtc_wakeup_flag
		{
			PD_WAKEUP_Int();
			Timer=0;
			SPI_Configuration();	
			Select_OLED

			LCD_Init();
			LCD_CLS();
			LCD_P16x16Ch(36,0,2,fd0); 
			FLASH_Read(((uint32_t)0x08080000),((uint32_t)0x08080010),Flash_Read_Buff);
			FLASH_Read(((uint32_t)0x08080020),((uint32_t)0x08080030),CV_Value);

			AD_Smapling_Function(&SenMsg);
			SenMsg.Senor_ID=Flash_Read_Buff[1];
			pObj->Senor_ID=SenMsg.Senor_ID;
			Senor_IDNUM_Temp=pObj->Senor_ID;
			*itoa(Senor_IDNUM_Temp,r,10);//			
			LCD_P8x16Str(86,0,r);
			LCD_P16x16Ch(0,3,8,fd1); 

			VVV1=CV_Value[0]/10;
			VVV11=CV_Value[0]%10;
			*itoa(VVV1,CV1,10);	
			if(VVV1>9)
			{
				LCD_P8x16Str(18,6,CV1);			
				LCD_P6x8Str(34,7,".");
				*itoa(VVV11,CV11,10);	
				LCD_P8x16Str(39,6,CV11);
			}
			else
			{
				LCD_P8x16Str(21,6,CV1);
				LCD_P6x8Str(29,7,".");
				*itoa(VVV11,CV11,10);	
				LCD_P8x16Str(34,6,CV11);
			}

			VVV2=CV_Value[1]/10;
			VVV22=CV_Value[1]%10;					
			*itoa(VVV2,CV2,10);//	
			if(VVV2>9)
			{		
				LCD_P8x16Str(81,6,CV2);
				LCD_P6x8Str(97,7,".");
				*itoa(VVV22,CV22,10);	
				LCD_P8x16Str(102,6,CV22);
			}
			else
			{
				LCD_P8x16Str(84,6,CV2);
				LCD_P6x8Str(92,7,".");
				*itoa(VVV22,CV22,10);	
				LCD_P8x16Str(97,6,CV22);
			}

			//           VVV3=CV_Value[2]/10;
			//					 VVV33=CV_Value[2]%10;						
			//           *itoa(VVV3,CV3,10);//	
			//    			 if(VVV3>9)
			//						{						
			//              LCD_P8x16Str(99,6,CV3);	
			//					    LCD_P6x8Str(115,7,".");
			//					    *itoa(VVV33,CV33,10);	
			//					    LCD_P8x16Str(120,6,CV33);
			//						}
			//					else
			//					  {
			//						  LCD_P8x16Str(102,6,CV3);	
			//					    LCD_P6x8Str(110,7,".");
			//					    *itoa(VVV33,CV33,10);	
			//					    LCD_P8x16Str(115,6,CV33);
			//						}

			delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);
			delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);


			//AD_Display_Function(&SenMsg); 
			PD_WAKEUP_Flag=Low;
			CC1100_WAKEUP_Flag=Low;
			LCD_CLS();
			Power_Control(Power_OFF);        //关闭传感器电源
			//LED_Display_Clear();             //清屏
			LED_Control(Power_OFF);          //关闭数码管电源
		}	
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /**Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL3;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */
  /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc.Init.LowPowerAutoWait = ADC_AUTOWAIT_UNTIL_DATA_READ;
  hadc.Init.LowPowerAutoPowerOff = ADC_AUTOPOWEROFF_IDLE_DELAY_PHASES;
  hadc.Init.ChannelsBank = ADC_CHANNELS_BANK_A;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.NbrOfConversion = 1;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_4CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */

}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /**Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
    
  /* USER CODE END Check_RTC_BKUP */

  /**Initialize RTC and set the Time and Date 
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /**Enable the WakeUp 
  */
  if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 5, RTC_WAKEUPCLOCK_CK_SPRE_16BITS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 9999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1599;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 1599;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 2000;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);

  /*Configure GPIO pins : PC4 PC5 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PC7 PC8 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB7 PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

}

/* USER CODE BEGIN 4 */
/**************************************************************
函数：void AD_Smapling_Function(CV_VALUE(*fun)(uint16_t, uint8_t),SENSOR *pObj)
功能：当无线唤醒时AD处理程序
**************************************************************/
void AD_Smapling_Function(SENOR *pObj)
{
    uint8_t i;
    Power_Control(Power_ON);   //启动传感器供电电源
    for(i=0; i<4; i++)
    {
        delay_us(400);
        Get_ADC(i);
    }
    //Power_Control(Power_OFF);  //关闭传感器电源
    FLASH_Write(((uint32_t)0x08080020),((uint32_t)0x08080030),CV_Value);
}
/**************************************************************
函数：void PD_WAKEUP_Int(void)
功能：当光照唤醒时需要的硬件
**************************************************************/
void PD_WAKEUP_Int(void)
{
    Timer_Config(1599);          //定时1秒
    Led_Control_GPIO();
    LED_Control(Power_ON);       //数码管供电
    //SPI_Configuration();         //SPI开启
    Power_Control(Power_ON);     //探头电源开启
    Infrared_Config(2000,1599);  //红外开启
    NVIC_Config();
}
/**************************************************************
函数：static void _cboption(SENOR *pObj)
功能：红外功能
**************************************************************/
//static void _cbmenu(SENOR *pObj)
//{
//    switch(k_code)
//    {
//    case IR_Up:					
//	
//    case IR_Down:
//        curse_tmp=curse;
//        curse=curse_base;
//        curse_base=curse_tmp;
//        k_code=0;
//        break;
//    case IR_OK:
//        curse_ok=0xff;
//        k_code=0;
//        break;
//    case IR_Cancel:
//        curse_ok=0;
//        SET_START=0;
//        k_code=0;
//    default:
//        break;
//    }
//    if(curse==0x28)
//    {
//        Led_Display_Set(0,pObj->Senor_ID);
//        Senor_IDNUM_Temp=pObj->Senor_ID;          //初始信息
//    }
//    else if(curse==0x04)
//    {
//        Led_Display_Set(1,pObj->Senor_Num);
//        Senor_IDNUM_Temp=pObj->Senor_Num;
//    }
//}
/**************************************************************
函数：static void _cboption(SENOR *pObj)
功能：红外功能
**************************************************************/
//static void _cboption(SENOR *pObj)
//{
//    switch(k_code)
//    {
//    case IR_Up:
//        if(Senor_IDNUM_Temp<1)Senor_IDNUM_Temp=curse;
//        Senor_IDNUM_Temp--;
//        k_code=0;
//        break;
//    case IR_Down:
//        Senor_IDNUM_Temp++;
//        if(Senor_IDNUM_Temp>curse)Senor_IDNUM_Temp=1;
//        k_code=0;
//        break;
//    case IR_OK:
//        if(curse==0x28)pObj->Senor_ID=Senor_IDNUM_Temp;
//        else if(curse==0x04)pObj->Senor_Num=Senor_IDNUM_Temp;
//        Flash_Write_Buff[0]=0x000000AA;
//        Flash_Write_Buff[1]=pObj->Senor_ID;
//        Flash_Write_Buff[2]=pObj->Senor_Num;
//        Flash_Write_Buff[3]=0x00000055;
//        FLASH_Write(((uint32_t)0x08080000),((uint32_t)0x08080010),Flash_Write_Buff);
//        curse_ok=0x00;
//        k_code=0;
//        break;
//    case IR_Cancel:
//        curse_ok=0x00;
//        k_code=0;
//    default:
//        break;
//    }
//    Led_Display_Set(2,Senor_IDNUM_Temp);
//}
/**************************************************************
函数：void Infrared_Fun(SENOR *pObj)
功能：红外功能
**************************************************************/
void Infrared_Fun(SENOR *pObj,MENU_CALLBK *_cbm,MENU_CALLBK *_cbo)
{
    SET_START=1,curse_ok=0;
    while(SET_START==1)
    {
        k_code=IR_KEY_Scan();
        if(curse_ok==0x00)
        {
            _cbm(pObj);
        }
        else if(curse_ok==0xff)
        {
            _cbo(pObj);
					pObj->Senor_ID--;
		     Senor_IDNUM_Temp=pObj->Senor_ID;
         *itoa(Senor_IDNUM_Temp,r,10);//
          LCD_CLS();			
         LCD_P8x16Str(56,4,r);		
        }
				pObj->Senor_ID+2;
		     Senor_IDNUM_Temp=pObj->Senor_ID;
         *itoa(Senor_IDNUM_Temp,r,10);//
          LCD_CLS();			
         LCD_P8x16Str(56,4,r);		
    }
}
/**************************************************************
函数：void AD_Display_Function(CV_VALUE(*fun)(uint16_t, uint8_t),SENOR *pObj)
功能：当光照唤醒时AD处理程序
**************************************************************/
void AD_Display_Function(SENOR *pObj)
{
     uint32_t KEY_CODE=0;
    Timer=0;
    do {
        if(Timer>=(pObj->Senor_Num))Timer=0;
        //Led_Display_Data(Timer,CV_Value[Timer]);  //显示数据

        KEY_CODE=IR_KEY_Scan();

        if(KEY_CODE==IR_Fun1)        //如果有红外信号
        {						
					LCD_CLS();					          
			    LCD_P16x16Ch(15,2,6,fd2); 	
					Senor_IDNUM_Temp=pObj->Senor_ID;
          *itoa(Senor_IDNUM_Temp,r,10);//		
          LCD_P8x16Str(56,4,r);		
 				}
          KEY_CODE=0;
          //Infrared_Fun(pObj,&_cbmenu,&_cboption);
					       
					KEY_CODE=IR_KEY_Scan();

        if(KEY_CODE==IR_Up)  
				{
				 pObj->Senor_ID++;
		     Senor_IDNUM_Temp=pObj->Senor_ID;
         *itoa(Senor_IDNUM_Temp,r,10);//
          LCD_CLS();			
         LCD_P8x16Str(56,4,r);		
				}
        else if(KEY_CODE==IR_Down)  
				{
				 pObj->Senor_ID--;
		     Senor_IDNUM_Temp=pObj->Senor_ID;
         *itoa(Senor_IDNUM_Temp,r,10);//
          LCD_CLS();			
         LCD_P8x16Str(56,4,r);		
				}
				else if(KEY_CODE==IR_OK)  
				{
		     Senor_IDNUM_Temp=pObj->Senor_ID;
         *itoa(Senor_IDNUM_Temp,r,10);//
         LCD_CLS();			
         LCD_P8x16Str(56,4,r);	
				 LCD_P8x16Str(56,6,"YES"); 
					
				CC1110_Config((uint32_t)9600);		
				ControlGPIO_Initt();	
					//LCD_P8x16Str(6,0,"Y"); 
			  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);//拉高
					//LCD_P8x16Str(26,0,"E"); 
		       HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);//拉高  进入配置模式
					//LCD_P8x16Str(46,0,"S"); 
				CC1110_Senddd();delay_ms(200);
					//LCD_P8x16Str(86,0,"P"); 
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);	
				//GPIO_SetBits(GPIOC,GPIO_Pin_7);//拉低
//					delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);
//					GPIO_SetBits(GPIOC,GPIO_Pin_7);//拉高
//					GPIO_SetBits(GPIOC,GPIO_Pin_8);//拉高  进入配置模式
//					CC1110_Sendddd();
//					GPIO_SetBits(GPIOC,GPIO_Pin_7);//拉低
	      Flash_Write_Buff[0]=0x000000AA;
        Flash_Write_Buff[1]=pObj->Senor_ID;
        Flash_Write_Buff[2]=pObj->Senor_Num;
        Flash_Write_Buff[3]=0x00000055;
        FLASH_Write(((uint32_t)0x08080000),((uint32_t)0x08080010),Flash_Write_Buff);
				}

        
    } while(Timer<2); //等待光照结束
    __HAL_TIM_DISABLE_IT(&htim4,TIM_IT_CC1|TIM_IT_UPDATE);
    __HAL_TIM_ENABLE_IT(&htim4,TIM_IT_UPDATE);	
    //TIM2->DIER &= (uint16_t)~TIM_IT_Update;
  //  TIM4->DIER &= (uint16_t)~TIM_IT_CC1|TIM_IT_Update;
    //Power_Control(Power_OFF);        //关闭传感器电源
    //LED_Display_Clear();             //清屏
    //LED_Control(Power_OFF);          //关闭数码管电源
}

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif



////////////SPI2接口配置/////////////////////////////////////////////////////
void SPI_ConfigurationOLED(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_SPI2_CLK_ENABLE();
  
  //  __HAL_RCC_GPIOB_CLK_ENABLE();
    /**SPI2 GPIO Configuration    
    PB13     ------> SPI2_SCK
    PB14     ------> SPI2_MISO
    PB15     ------> SPI2_MOSI 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);



	  /* PB12--OLED RST  引脚定义 */
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
   // GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
      /* PC6--OLED C/D 引脚定义 */
   GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    //GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	  /* 配置SPI2 OLED片选  PB2 */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    //GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	  /* SPI2 Config -------------------------------------------------------------*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
	  /* Enable SPI2 */
	//  SPI_Cmd(SPI2, ENABLE);
}
//extern uint32_t CV_Value[4];

int k=0x01;
void CC1110_Sendd(void)
{
    uint8_t h,m,l;
	  k++;
      
//			f = 0xff;
//			USART3->DR = (f & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //发送完成标志
//	
//			g = 0xfe;
//			USART3->DR = (g & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //发送完成标志

//			CV_Value[3]=SenMsg.Senor_ID;
//			q = CV_Value[3];
//			USART3->DR = (q & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //发送完成标志
		
//////						 SPI_Configuration();	
//////	         Select_OLED
//////            
//////					 LCD_Init();
//////					 LCD_CLS();
//////					 LCD_P16x16Ch(36,0,2,fd1); 
//////delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);
	
	    FLASH_Read(((uint32_t)0x08080040),((uint32_t)0x08080050),ID_Read_Buff);
			h = *ID_Read_Buff;	
			 HAL_UART_Transmit(&huart3, &h, 1, 10);
			//USART3->DR = (*(ID_Read_Buff+1) & (uint16_t)0x01FF);		
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志	
			h = *(ID_Read_Buff+1);
			 HAL_UART_Transmit(&huart3, &(h), 1, 10);			
			m = *(ID_Read_Buff+2);
			//USART3->DR = (m & (uint16_t)0x01FF);		
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志	
			HAL_UART_Transmit(&huart3, &(m), 1, 10);	
			//USART3->DR = (*(ID_Read_Buff+3) & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			h = *(ID_Read_Buff+3);
			HAL_UART_Transmit(&huart3, &(h), 1, 10);	
			l = SenMsg.Senor_ID;
			//USART3->DR = (l & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			HAL_UART_Transmit(&huart3, &(l ), 1, 10);	
			//USART3->DR = (0xf1 & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			l = 0xf1;
			HAL_UART_Transmit(&huart3, &(l ), 1, 10);	
			//USART3->DR = (0xf2 & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			l = 0xf2;
			HAL_UART_Transmit(&huart3, &(l ), 1, 10);			
			//USART3->DR = (0xf3 & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			l = 0xf3;
			HAL_UART_Transmit(&huart3, &(l ), 1, 10);
			//USART3->DR = (0xf4 & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标?
			l = 0xf4;
			HAL_UART_Transmit(&huart3, &(l ), 1, 10);
			//USART3->DR = (0xf5 & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			l = 0xf5;
			HAL_UART_Transmit(&huart3, &(l ), 1, 10);			
			//USART3->DR = (0xf6 & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标?
			l = 0xf6;
			HAL_UART_Transmit(&huart3, &(l ), 1, 10);
			//USART3->DR = (0xf7 & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			l = 0xf7;
			HAL_UART_Transmit(&huart3, &(l ), 1, 10);			
			//USART3->DR = (0xf8 & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			l = 0xf8;
			HAL_UART_Transmit(&huart3, &(l ), 1, 10);			
			//USART3->DR = (0xf9 & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标?
			l = 0xf9;
			HAL_UART_Transmit(&huart3, &(l ), 1, 10);
			//USART3->DR = (0xfe & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			l = 0xfe;
			HAL_UART_Transmit(&huart3, &(l ), 1, 10);			
			//USART3->DR = (0xe7 & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			l = 0xe7;
			HAL_UART_Transmit(&huart3, &(l ), 1, 10);			
}
//int k=0x01;
void CC1110_Senddata(void)
{
      uint8_t h,m,p,v;
	    k++;

//////					 SPI_Configuration();	
//////	         Select_OLED
//////            
//////					 LCD_Init();
//////					 LCD_CLS();
//////					 LCD_P16x16Ch(36,0,2,fd1); 
//////delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);
					 FLASH_Read(((uint32_t)0x08080000),((uint32_t)0x08080010),Flash_Read_Buff);
					 FLASH_Read(((uint32_t)0x08080020),((uint32_t)0x08080030),CV_Value);
          
					 AD_Smapling_Function(&SenMsg);

      FLASH_Read(((uint32_t)0x08080040),((uint32_t)0x08080050),ID_Read_Buff);
			//USART3->DR = (*ID_Read_Buff & (uint16_t)0x01FF);		
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志		
			//l = 0xe7;
	    h = *(ID_Read_Buff);
			HAL_UART_Transmit(&huart3, &(h ), 1, 10);	
			//USART3->DR = (*(ID_Read_Buff+1) & (uint16_t)0x01FF);		
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志	
						h = *(ID_Read_Buff+1);
			HAL_UART_Transmit(&huart3, &(h ), 1, 10);
			//USART3->DR = (*(ID_Read_Buff+2) & (uint16_t)0x01FF);		
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志	
				h = *(ID_Read_Buff+2);
	              HAL_UART_Transmit(&huart3, &(h ), 1, 10);
			//USART3->DR = (*(ID_Read_Buff+3) & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
							h = *(ID_Read_Buff+3);
			HAL_UART_Transmit(&huart3, &(h ), 1, 10);
			//USART3->DR = (SenMsg.Senor_ID & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			HAL_UART_Transmit(&huart3, &(SenMsg.Senor_ID ), 1, 10);
			h = CV_Value[0]>>8;
			//USART3->DR = (h & (uint16_t)0x01FF);		
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			HAL_UART_Transmit(&huart3, &(h ), 1, 10);
     h = CV_Value[0];
			//USART3->DR = (CV_Value[0] & (uint16_t)0x01FF);		
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志	
			HAL_UART_Transmit(&huart3, &(h), 1, 10);
			m = CV_Value[1]>>8;
			//USART3->DR = (m & (uint16_t)0x01FF);		
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			
			HAL_UART_Transmit(&huart3, &(m ), 1, 10);
			//USART3->DR = (CV_Value[1] & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			h = CV_Value[1];
			HAL_UART_Transmit(&huart3, &(h), 1, 10);
			v = 0x00>>8;
			//USART3->DR = (v & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			HAL_UART_Transmit(&huart3, &(v ), 1, 10);

			//USART3->DR = (0x00 & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标?
			v=0;;
			HAL_UART_Transmit(&huart3, &(v ), 1, 10);
		//	USART3->DR = (0x00 & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标?
			HAL_UART_Transmit(&huart3, &(v ), 1, 10);
			//USART3->DR = (0x00 & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			HAL_UART_Transmit(&huart3, &(v ), 1, 10);
			CV_Value[4]=Battery_Monitor();
			p = CV_Value[4];
			//USART3->DR = (p & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			HAL_UART_Transmit(&huart3, &(p ), 1, 10);
			//USART3->DR = (0xf0 & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			v=0xf0;
			HAL_UART_Transmit(&huart3, &(v ), 1, 10);
			//USART3->DR = (0xE7 & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标?
			v=0xE7;
			HAL_UART_Transmit(&huart3, &(v ), 1, 10);
}

//发送配置信息
void CC1110_Senddd(void)
{
    uint8_t co,idh,idl,pz,xd,fc;
	    
			co = 0xC0;
			//USART3->DR = (co & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			HAL_UART_Transmit(&huart3, &(co ), 1, 10);
			idh = 0x00;
			//USART3->DR = (idh & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			HAL_UART_Transmit(&huart3, &(idh ), 1, 10);
			idl = SenMsg.Senor_ID;
			//USART3->DR = (idl & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
	      		HAL_UART_Transmit(&huart3, &(idl ), 1, 10);
			pz = 0x18;
			//USART3->DR = (pz & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			HAL_UART_Transmit(&huart3, &(pz ), 1, 10);
			xd = SenMsg.Senor_ID;
			//USART3->DR = (xd & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			HAL_UART_Transmit(&huart3, &(xd ), 1, 10);
			fc = 0xFC;
			//USART3->DR = (fc & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志		
			HAL_UART_Transmit(&huart3, &(fc ), 1, 10);
}
void CC1110_Sendbianhao(void)
{
 uint8_t tmp;
			Flash_Write_Buff[0]=0x000000AA;
			Flash_Write_Buff[1]=newid;
			Flash_Write_Buff[2]=0x11;
			Flash_Write_Buff[3]=0x00000055;
			FLASH_Write(((uint32_t)0x08080000),((uint32_t)0x08080010),Flash_Write_Buff);
	
			FLASH_Read(((uint32_t)0x08080000),((uint32_t)0x08080010),Flash_Read_Buff);	
			if(*Flash_Read_Buff==0x000000AA&&*(Flash_Read_Buff+3)==0x00000055)
			{
					SenMsg.Senor_ID=*(Flash_Read_Buff+1);				
			}
			tmp = 0x1D;
			//USART3->DR = (0x1D & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志	
			HAL_UART_Transmit(&huart3, &(tmp ), 1, 10);
			tmp = SenMsg.Senor_ID;
			//USART3->DR = (SenMsg.Senor_ID & (uint16_t)0x01FF);
			//while((USART3->SR & 0x0040) == RESET); //发送完成标志
			HAL_UART_Transmit(&huart3, &(tmp ), 1, 10);

}

//void CC1110_Sendnum(void)
//{
//      int i,h,m,p,q,v,f,g,l;
//	    k++;
//	
//////      SPI_Configuration();	
//////		 Select_OLED
//////			
//////		 LCD_Init();
//////		 LCD_CLS();
//////		 LCD_P16x16Ch(36,0,2,fd2); 
//////delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);

////delay_ms(200);delay_ms(200);delay_ms(200);delay_ms(200);
//	    Flash_Write_Buff[0]=0x000000AA;
//			Flash_Write_Buff[1]=newid;
//			Flash_Write_Buff[2]=0x11;
//			Flash_Write_Buff[3]=0x00000055;
//			FLASH_Write(((uint32_t)0x08080000),((uint32_t)0x08080010),Flash_Write_Buff);
//	
//			FLASH_Read(((uint32_t)0x08080000),((uint32_t)0x08080010),Flash_Read_Buff);	
//			if(*Flash_Read_Buff==0x000000AA&&*(Flash_Read_Buff+3)==0x00000055)
//			{
//					SenMsg.Senor_ID=*(Flash_Read_Buff+1);				
//			}
//			FLASH_Read(((uint32_t)0x08080000),((uint32_t)0x08080010),Flash_Read_Buff);
//			FLASH_Read(((uint32_t)0x08080020),((uint32_t)0x08080030),CV_Value);
//          
//			//AD_Smapling_Function(&SenMsg);

//      FLASH_Read(((uint32_t)0x08080040),((uint32_t)0x08080050),ID_Read_Buff);
//			USART3->DR = (*ID_Read_Buff & (uint16_t)0x01FF);		
//			while((USART3->SR & 0x0040) == RESET); //发送完成标志		

//			USART3->DR = (*(ID_Read_Buff+1) & (uint16_t)0x01FF);		
//			while((USART3->SR & 0x0040) == RESET); //发送完成标志	
//			
//			USART3->DR = (*(ID_Read_Buff+2) & (uint16_t)0x01FF);		
//			while((USART3->SR & 0x0040) == RESET); //发送完成标志	
//	
//			USART3->DR = (*(ID_Read_Buff+3) & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //发送完成标志
//			
//			USART3->DR = (SenMsg.Senor_ID & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //发送完成标志
//			
//			USART3->DR = (0xD1 & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //发送完成标志
//			USART3->DR = (0xD2 & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //发送完成标志
//			USART3->DR = (0xD3 & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //发送完成标志
//			USART3->DR = (0xD4 & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //发送完成标志
//			USART3->DR = (0xD5 & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //发送完成标志
//			USART3->DR = (0xD6 & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //发送完成标志
//			USART3->DR = (0xD7 & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //发送完成标志
//			USART3->DR = (0xD8 & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //发送完成标志
//			USART3->DR = (0xD9 & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //发送完成标志
//			
//			
//			USART3->DR = (0xf9 & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //发送完成标志

//			USART3->DR = (0xE7 & (uint16_t)0x01FF);
//			while((USART3->SR & 0x0040) == RESET); //发送完成标志
//}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
