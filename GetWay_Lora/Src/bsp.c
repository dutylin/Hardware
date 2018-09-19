#include "main.h"
#include "bsp.h"

#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
#include "lte_hal.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
Uart_Types UsartType1,UsartType2,UsartType3;


/* USER CODE END PV */


//串口接收空闲中断
void UsartReceive_IDLE(unsigned char uart_num)
{
	uint32_t temp;

	if(uart_num==2)
	{
		temp=__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE);
		if((temp!=RESET))
		{
			__HAL_UART_CLEAR_IDLEFLAG(&huart2);

			temp=huart2.Instance->SR;
			temp=huart2.Instance->DR;
			HAL_UART_DMAPause(&huart2);
			temp=hdma_usart2_rx.Instance->CNDTR;
			UsartType2.real_index=UsartType2.real_index+UsartType2.rx_len_var;
			if(UsartType2.real_index>=buffer_size)
				UsartType2.real_index=UsartType2.real_index-buffer_size;
			UsartType2.rx_len_var=UsartType2.rx_len;
			UsartType2.rx_len=buffer_size-temp;
			if(UsartType2.rx_len<UsartType2.rx_len_var)
			{
				UsartType2.rx_len_var=UsartType2.rx_len+buffer_size-UsartType2.rx_len_var;
			}
			else
				UsartType2.rx_len_var=UsartType2.rx_len-UsartType2.rx_len_var;
			  UsartType2.receive_flag=1;
			//if(get_connection_step()>=MAX_STEP)
			//GetServer_Info_Poll();//服务器下发指令

			//SIMCOM_Register_Network();


			HAL_UART_DMAResume(&huart2);
			HAL_UART_Receive_DMA(&huart2,UsartType2.usartDMA_rxBuf,buffer_size);

		}
	}

if(uart_num==3)
	{
		temp=__HAL_UART_GET_FLAG(&huart3,UART_FLAG_IDLE);
		if((temp!=RESET))
		{
			__HAL_UART_CLEAR_IDLEFLAG(&huart3);

			temp=huart3.Instance->SR;
			temp=huart3.Instance->DR;
			HAL_UART_DMAPause(&huart3);
			temp=hdma_usart3_rx.Instance->CNDTR;
			UsartType3.real_index=UsartType3.real_index+UsartType3.rx_len_var;
			if(UsartType3.real_index>=buffer_size)
				UsartType3.real_index=UsartType3.real_index-buffer_size;
			UsartType3.rx_len_var=UsartType3.rx_len;
			UsartType3.rx_len=buffer_size-temp;
			if(UsartType3.rx_len<UsartType3.rx_len_var)
			{
				UsartType3.rx_len_var=UsartType3.rx_len+buffer_size-UsartType3.rx_len_var;
			}
			else
				UsartType3.rx_len_var=UsartType3.rx_len-UsartType3.rx_len_var;
			UsartType3.receive_flag=1;
			//if(get_connection_step()>=MAX_STEP)
			//GetServer_Info_Poll();//服务器下发指令

			//SIMCOM_Register_Network();


			HAL_UART_DMAResume(&huart3);
			HAL_UART_Receive_DMA(&huart3,UsartType3.usartDMA_rxBuf,buffer_size);

		}
	}

}

void uart_tx(unsigned char num,unsigned char* comm,unsigned short len)
{
       if(num==1)
       	{
		   HAL_UART_DMAStop(&huart1);
		   HAL_UART_DMAResume(&huart1);
			  UsartType1.real_index=0;
		   UsartType1.rx_len=0;
		   UsartType1.rx_len_var=0;
		   UsartType1.loop_index=0;
		   HAL_UART_Receive_DMA(&huart1,UsartType1.usartDMA_rxBuf,buffer_size);
		   //delay_ms(100);
		   //uart5_dma_state=0;
		   HAL_UART_Transmit(&huart1,comm,len,100);

	   }
       if(num==2)
       	{
		   HAL_UART_DMAStop(&huart2);
		   HAL_UART_DMAResume(&huart2);
			  UsartType2.real_index=0;
		   UsartType2.rx_len=0;
		   UsartType2.rx_len_var=0;
		   UsartType2.loop_index=0;
		   HAL_UART_Receive_DMA(&huart2,UsartType2.usartDMA_rxBuf,buffer_size);
		   //delay_ms(100);
		   //uart5_dma_state=0;
		   HAL_UART_Transmit(&huart2,comm,len,100);

	   }
       if(num==3)
       	{
		   HAL_UART_DMAStop(&huart3);
		   HAL_UART_DMAResume(&huart3);
			  UsartType3.real_index=0;
		   UsartType3.rx_len=0;
		   UsartType3.rx_len_var=0;
		   UsartType3.loop_index=0;
		   HAL_UART_Receive_DMA(&huart3,UsartType3.usartDMA_rxBuf,buffer_size);
		   //delay_ms(100);
		   //uart5_dma_state=0;
		   HAL_UART_Transmit(&huart3,comm,len,100);

	   }


}



void uart_init()
{

//	HAL_UART_Receive_DMA(&huart5,UsartType5.usartDMA_rxBuf,buffer_size);
//	__HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);    //??????
//	__HAL_UART_CLEAR_IDLEFLAG(&huart5);
//
	__HAL_UART_DISABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断
	HAL_UART_DMAStop(&huart2);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断
	__HAL_UART_CLEAR_IDLEFLAG(&huart2);
	HAL_UART_Receive_DMA(&huart2,UsartType2.usartDMA_rxBuf,buffer_size);
	//HAL_UART_Receive_DMA(&huart2,UsartType2.usartDMA_rxBuf,buffer_size);
	
	
		__HAL_UART_DISABLE_IT(&huart3, UART_IT_IDLE);	//使能空闲中断
	HAL_UART_DMAStop(&huart3);
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);	//使能空闲中断
	__HAL_UART_CLEAR_IDLEFLAG(&huart3);
	HAL_UART_Receive_DMA(&huart3,UsartType3.usartDMA_rxBuf,buffer_size);
}

void Pump_Ctrl(unsigned char flag)
{
	if(flag==ON)
		  HAL_GPIO_WritePin(Pump_Ctrl_GPIO_Port, Pump_Ctrl_Pin, GPIO_PIN_SET);
  else
		  HAL_GPIO_WritePin(Pump_Ctrl_GPIO_Port, Pump_Ctrl_Pin, GPIO_PIN_RESET);

}
