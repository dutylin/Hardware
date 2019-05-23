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
#include "radio.h"
#include "common.h"
#include <stdio.h>
#include <string.h>
/** @addtogroup STM8L15x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
 #define RF_PACKET_SIZE 32               /* Define the payload size here */

 u8 g_rxBuffer[RF_PACKET_SIZE];   /* RF Rx buffer */
 u8 g_txBuffer[RF_PACKET_SIZE];   /* RF Tx buffer */

 BOOL g_bEnableMaster = FALSE;     /* Master/Slave selection */

 u16 g_nRecvCount = 0;
 u16 g_nSendCount = 0;
 u16 g_nErrCount  = 0;   
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void OnMaster(void);
void OnSlave(void);
/* Private functions ---------------------------------------------------------*/
/* Manages the master operation */
void OnMaster(void)
{
    char str[32];
    
    switch(RF_Process())
    {
    case RF_IDLE:
    {
        g_nSendCount++;
        g_txBuffer[0] = (u8)g_nSendCount;
        g_txBuffer[1] = g_nSendCount >> 8;
        
        RF_StartTx(g_txBuffer, RF_PACKET_SIZE, 1000);
        break;
    }
    
    case RF_TX_DONE:
    {
      //  led_onAutoOff(LED_INDEX1, 50);
        
        sprintf(str, "send: %d", g_nSendCount);
        views_print_line(1, str);
        
        RF_StartRx(g_rxBuffer, RF_PACKET_SIZE, 1000);
        break;
    }
    
    case RF_RX_DONE:
    {
       // led_onAutoOff(LED_INDEX2, 50);
        
        g_nRecvCount++;
        sprintf(str, "recv: %d", g_nRecvCount);
        views_print_line(2, str);

        break;
    }
    
    case RF_RX_TIMEOUT:
    {
        sprintf(str, "recv: timeout");
        views_print_line(2, str);
        
        break;
    }
    
    case RF_ERROR:
    {
       // led_onAutoOff(LED_INDEX3, 1000);
        
        sprintf(str, "error: %d", ++g_nErrCount);
        views_print_line(3, str);
        
        break;
    }
    
    default:
        break;
    }
}

/* Manages the slave operation */
void OnSlave(void)
{
    char str[32];
    
    switch(RF_Process())
    {
    case RF_IDLE:
    {
        RF_StartRx(g_rxBuffer, RF_PACKET_SIZE, INFINITE);
        break;
    }
    
    case RF_RX_DONE:
    {
     //   led_onAutoOff(LED_INDEX2, 50);
        
        g_nRecvCount++;
        sprintf(str, "recv: %d", g_nRecvCount);
        views_print_line(1, str);

        CMT2300A_DelayMs(10);

        g_nSendCount++;
        g_txBuffer[0] = (u8)g_nSendCount;
        g_txBuffer[1] = g_nSendCount >> 8;
        
        RF_StartTx(g_txBuffer, RF_PACKET_SIZE, 1000);
        
        break;
    }
    
    case RF_TX_DONE:
    {
       // led_onAutoOff(LED_INDEX1, 50);
        
        sprintf(str, "send: %d", g_nSendCount);
        views_print_line(2, str);
        
        break;
    }
    
    case RF_ERROR:
    {
       // led_onAutoOff(LED_INDEX3, 1000);
        
        sprintf(str, "error: %d", ++g_nErrCount);
        views_print_line(3, str);
        
        break;
    }
    
    default:
        break;
    }
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
   RF_Init();
  if(FALSE==CMT2300A_IsExist()) {
        views_print_line(0, "CMT2300A not found!");
        //led_on(LED_INDEX3);
        while(1);
    }
    else {
        views_print_line(0, "CMT2300A ready");
    }
       
  /* Infinite loop */
  while (1)
  {
        if(TRUE==g_bEnableMaster) {
            OnMaster();
        }
        else {
            OnSlave();
        }    
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
