/**
  ******************************************************************************
  * @file SysTick/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version  V3.0.0
  * @date  04/06/2009
  * @brief  Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and 
  *         peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"	 
#include "stm32f10x_usart.h"
#include "stm32f10x_can.h"
#include <ctype.h>
#include <stdio.h>
extern void TimingDelay_Decrement(void);
int fuck;


void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval : None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval : None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval : None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval : None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval : None
  */
void SysTick_Handler(void)
{
  TimingDelay_Decrement();
}



/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
extern uint8_t TxBuffer1[]; 
extern uint8_t TxBuffer2[]; 
extern int RxBuffer1[];
extern uint8_t RxBuffer2[];
extern __IO uint8_t TxCounter1;
extern __IO uint8_t TxCounter2;
extern __IO uint8_t RxCounter1; 
extern __IO uint8_t RxCounter2;

extern uint8_t rec_ff,tx_flag;
uint8_t rec_f;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/



/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles USB Low Priority or CAN RX0 interrupts 
  *   requests.
  * @param  None
  * @retval : None
  */
	  CanRxMsg RxMessage;
void USB_LP_CAN1_RX0_IRQHandler(void)
{


  RxMessage.StdId=0x00;
  RxMessage.ExtId=0x00;
  RxMessage.IDE=0;
  RxMessage.DLC=0;
  RxMessage.FMI=0;
  RxMessage.Data[0]=0x00;
  RxMessage.Data[1]=0x00;

  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
  /* 比较是否是发送的数据和ID */ 
//  if((RxMessage.ExtId==0x1234) && (RxMessage.IDE==CAN_ID_EXT)
//     && (RxMessage.DLC==2) && ((RxMessage.Data[1]|RxMessage.Data[0]<<8)==0xDECA))
//  {
  /* 比较报文长度与内容 */ 
  if((RxMessage.DLC==2) && ((RxMessage.Data[1]|RxMessage.Data[0]<<8)==0xDECA))
  {
    rec_f = 1; 
		//fuck = 1; 	
		//rec_f = 0; 					       //接收成功
    //CC1110_Sendd();
  }
  else
  {
		rec_f = 0; 
     //fuck = 0; 						   //接收失败
  }
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval : None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

