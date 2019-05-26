#include "key.h"
#include "main.h"
#include "eeprom.h"
void key_intterupt_config( EXTI_Trigger_TypeDef EXTI_Trigger)
{
  GPIO_Init(KEY_UP_PORT, KEY_UP_PIN, GPIO_Mode_In_FL_IT);                //up
  EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger);
  
  GPIO_Init(KEY_DOWN_PORT, KEY_DOWN_PIN, GPIO_Mode_In_FL_IT);                //DOWN
  EXTI_SetPinSensitivity(EXTI_Pin_2, EXTI_Trigger);
  
  GPIO_Init(KEY_EAST_PORT, KEY_EAST_PIN, GPIO_Mode_In_FL_IT);                //EAST
  EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger);
  
  GPIO_Init(KEY_WEST_PORT, KEY_WEST_PIN, GPIO_Mode_In_FL_IT);                //WEST
  EXTI_SetPinSensitivity(EXTI_Pin_0, EXTI_Trigger);
  
  GPIO_Init(KEY_SOUTH_PORT, KEY_SOUTH_PIN, GPIO_Mode_In_FL_IT);                //SOUTH
  EXTI_SetPinSensitivity(EXTI_Pin_6, EXTI_Trigger);
  
  GPIO_Init(KEY_START_PORT, KEY_START_PIN, GPIO_Mode_In_FL_IT);                //START
  EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger);
  
  GPIO_Init(KEY_NORTH_PORT, KEY_NORTH_PIN, GPIO_Mode_In_FL_IT);                //NORTH
  EXTI_SetPinSensitivity(EXTI_Pin_4, EXTI_Trigger);
  
  GPIO_Init(KEY_BREAK_PORT, KEY_BREAK_PIN, GPIO_Mode_In_FL_IT);                //BREAK
  EXTI_SetPinSensitivity(EXTI_Pin_0, EXTI_Trigger_Falling_Low);
  
  GPIO_Init(KEY_STOP_PORT, KEY_STOP_PIN, GPIO_Mode_In_FL_IT);                //STOP
  EXTI_SetPinSensitivity(EXTI_Pin_5, EXTI_Trigger);  
}
void KEY_Init()
{
  params_typedef *params;
  EXTI_Trigger_TypeDef EXTI_Trigger;
  GPIO_Init(KEY_UP_PORT,KEY_UP_PIN, GPIO_Mode_In_FL_IT);
  GPIO_Init(KEY_DOWN_PORT,KEY_DOWN_PIN, GPIO_Mode_In_FL_IT);
  GPIO_Init(KEY_EAST_PORT,KEY_EAST_PIN, GPIO_Mode_In_FL_IT);
  GPIO_Init(KEY_WEST_PORT,KEY_WEST_PIN, GPIO_Mode_In_FL_IT); 
  GPIO_Init(KEY_SOUTH_PORT,KEY_SOUTH_PIN, GPIO_Mode_In_FL_IT);
  GPIO_Init(KEY_START_PORT,KEY_NORTH_PIN, GPIO_Mode_In_FL_IT); 
  GPIO_Init(KEY_NORTH_PORT,KEY_START_PIN, GPIO_Mode_In_FL_IT);
  GPIO_Init(KEY_STOP_PORT,KEY_STOP_PIN, GPIO_Mode_In_FL_IT); 
  GPIO_Init(KEY_BREAK_PORT,KEY_BREAK_PIN, GPIO_Mode_In_FL_IT);
  
  /*
#define INCH					0X01   //点动
#define SELFLOCK				0X02 //自锁
#define INHIBITION			0X03//相互拟制
#define NOINHIBITION		0X04//非拟制
  */
  params = system_params_get();
  switch(params->keyfunc)
  {
  case INCH:			EXTI_Trigger = EXTI_Trigger_Falling_Low; break;
  case SELFLOCK:		EXTI_Trigger = EXTI_Trigger_Rising; break;
  case INHIBITION:		EXTI_Trigger = EXTI_Trigger_Falling_Low; break;
  case NOINHIBITION:	        EXTI_Trigger = EXTI_Trigger_Falling_Low; break;
  default:			EXTI_Trigger = EXTI_Trigger_Falling_Low; break;	
  }
  key_intterupt_config(EXTI_Trigger);
}


