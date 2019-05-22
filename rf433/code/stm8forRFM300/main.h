/**
  ******************************************************************************
  * @file    EXTI_InterruptPriority\main.h
  * @author  XIAOXIAO  Team
  * @version V2.0.0
  * @date    25-February-2011
  * @brief   This file contains the external declaration for main.c file.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */   

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Evalboard I/Os configuration */

#define GPIO_PORT_C  (GPIOC)


#define GPIO_PINS (GPIO_PIN_3 | GPIO_PIN_5  |GPIO_PIN_6  | GPIO_PIN_2)

#define GPIO_PINS_CSB  (GPIO_PIN_3)
#define GPIO_PINS_SDIO  (GPIO_PIN_2)
#define GPIO_PINS_FCSB  (GPIO_PIN_6)
#define GPIO_PINS_SDCK  (GPIO_PIN_5)

#define GPIO1    (GPIO_PIN_1)      
   



/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#endif /* __MAIN_H__ */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
