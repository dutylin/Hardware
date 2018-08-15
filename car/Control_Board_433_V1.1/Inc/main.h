/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define POWER_STATE_Pin GPIO_PIN_2
#define POWER_STATE_GPIO_Port GPIOE
#define LTE_PWR_EN_Pin GPIO_PIN_1
#define LTE_PWR_EN_GPIO_Port GPIOC
#define BATTERY_SUP_Pin GPIO_PIN_2
#define BATTERY_SUP_GPIO_Port GPIOC
#define GAS_SENSE_Pin GPIO_PIN_3
#define GAS_SENSE_GPIO_Port GPIOC
#define UART2_TX_Pin GPIO_PIN_2
#define UART2_TX_GPIO_Port GPIOA
#define DQ_Pin GPIO_PIN_3
#define DQ_GPIO_Port GPIOA
#define FLASH_nCS1_Pin GPIO_PIN_4
#define FLASH_nCS1_GPIO_Port GPIOA
#define FLASH_SCLK_Pin GPIO_PIN_5
#define FLASH_SCLK_GPIO_Port GPIOA
#define FLASH_SDO_Pin GPIO_PIN_6
#define FLASH_SDO_GPIO_Port GPIOA
#define FLASH_SDI_Pin GPIO_PIN_7
#define FLASH_SDI_GPIO_Port GPIOA
#define MOTO_SENSE_Pin GPIO_PIN_4
#define MOTO_SENSE_GPIO_Port GPIOC
#define CHRG_Pin GPIO_PIN_8
#define CHRG_GPIO_Port GPIOE
#define RF_433_EN_Pin GPIO_PIN_10
#define RF_433_EN_GPIO_Port GPIOB
#define LED_BACK_Pin GPIO_PIN_11
#define LED_BACK_GPIO_Port GPIOB
#define MCU_433_ACK_Pin GPIO_PIN_13
#define MCU_433_ACK_GPIO_Port GPIOB
#define MCU_433_TXD_Pin GPIO_PIN_8
#define MCU_433_TXD_GPIO_Port GPIOD
#define MCU_433_RXD_Pin GPIO_PIN_9
#define MCU_433_RXD_GPIO_Port GPIOD
#define LED_PWR_Pin GPIO_PIN_10
#define LED_PWR_GPIO_Port GPIOD
#define LED_GPS_Pin GPIO_PIN_11
#define LED_GPS_GPIO_Port GPIOD
#define LED_433MHz_Pin GPIO_PIN_12
#define LED_433MHz_GPIO_Port GPIOD
#define GPS_PWR_CE_Pin GPIO_PIN_6
#define GPS_PWR_CE_GPIO_Port GPIOC
#define POWER_STATEC9_Pin GPIO_PIN_9
#define POWER_STATEC9_GPIO_Port GPIOC
#define MCU_LTE_RST_Pin GPIO_PIN_15
#define MCU_LTE_RST_GPIO_Port GPIOA
#define MCU_LTE_PWR_Pin GPIO_PIN_10
#define MCU_LTE_PWR_GPIO_Port GPIOC
#define MCU_LTE_TXD_Pin GPIO_PIN_12
#define MCU_LTE_TXD_GPIO_Port GPIOC
#define MCU_LTE_RXD_Pin GPIO_PIN_2
#define MCU_LTE_RXD_GPIO_Port GPIOD
#define PWR_KEY_Pin GPIO_PIN_4
#define PWR_KEY_GPIO_Port GPIOD
#define UART2_RX_Pin GPIO_PIN_6
#define UART2_RX_GPIO_Port GPIOD

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
