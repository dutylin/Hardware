#include "stm32l1xx_it.h"
#include "main.h"
#include "global.h"
#include "other.h"
#include "infrared.h"
#include "adc.h"

WPStatus CC1100_WAKEUP_Flag;
WPStatus PD_WAKEUP_Flag;
/************************定时计数相关*************************/
uint8_t Timer=0;
uint8_t g_IR_State = 0;
uint32_t g_IR_Code[IR_CODE_LEN];
extern uint32_t CV_Value[4];
/**************************************************************
函数：void NMI_Handler(void)
功能：不可屏蔽
**************************************************************/
void NMI_Handler(void)
{

}
/**************************************************************
函数：void HardFault_Handler(void)
功能：硬件错误
**************************************************************/
void HardFault_Handler(void)
{
    while (1)
    {
    }
}
/**************************************************************
函数：void MemManage_Handler(void)
功能：内存管理错误
**************************************************************/
void MemManage_Handler(void)
{
    while (1)
    {
    }
}
/**************************************************************
函数：void BusFault_Handler(void)
功能：总线错误中断，一般发生在数据访问异常，比如FSMC访问不当
**************************************************************/
void BusFault_Handler(void)
{
    while (1)
    {
    }
}
/**************************************************************
函数：void UsageFault_Handler(void)
功能：用法错误中断，一般是预取值，或者位置指令、数据处理等错误
**************************************************************/
void UsageFault_Handler(void)
{
    while (1)
    {
    }
}
/**************************************************************
函数：void SVC_Handler(void)
功能：系统调用异常，主要是为了调用操作系统内核服务
**************************************************************/
void SVC_Handler(void)
{
}
/**************************************************************
函数：void DebugMon_Handler(void)
功能：调试监视异常
**************************************************************/
void DebugMon_Handler(void)
{
}
/**************************************************************
函数：void PendSV_Handler(void)
功能：挂起异常
**************************************************************/
void PendSV_Handler(void)
{
}
/**************************************************************
函数：void SysTick_Handler(void)
功能：
**************************************************************/
void SysTick_Handler(void)
{


}
/**************************************************************
函数：void EXTI0_IRQHandler(void)
功能；PB0 WIREWAKEUP
**************************************************************/
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        CC1100_WAKEUP_Flag=High;
        PD_WAKEUP_Flag=Low;
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}
/**************************************************************
函数：void EXTI1_IRQHandler(void)
功能；PB1 PD_WAKEUP WIREWAKEUP
**************************************************************/
void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        CC1100_WAKEUP_Flag=Low;
        PD_WAKEUP_Flag=High;
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}
/**************************************************************
函数：void TIM2_IRQHandler(void)
功能：定时器定时1秒产生中断
**************************************************************/
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
        //Get_ADC(Timer);
			      Get_ADC(0);
			 Get_ADC(1);
			 Get_ADC(2);
        Timer++;
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
  FLASH_Write(((uint32_t)0x08080020),((uint32_t)0x08080030),CV_Value);
//			          CC1110_Config((uint32_t)9600);	

//          CC1110_Send_Flag=1;		
//		ControlGPIO_Initt();
//		GPIO_ResetBits(GPIOC,GPIO_Pin_8);//拉低 

//		USART_ReceiveData(USART3);
//		USART_ClearFlag(USART3,USART_FLAG_RXNE);
//		CC1110_Sendd();
    }
}

/**************************************************************
函数：void TIM3_IRQHandler(void)
功能：红外中断接收
**************************************************************/
void TIM4_IRQHandler(void)
{
    static uint32_t s_i = 0;   // 静态局部变量
    if(TIM_GetITStatus(TIM4, TIM_IT_CC1) == SET)
    {
        TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE); //关闭溢出中断
        switch (g_IR_State)
        {
        case IR_STATE_NO: // 第一个下降沿，初始化数据，准备捕获数据
            TIM_SetCounter(TIM4,0);
            s_i = 0;
            g_IR_State = IR_STATE_READ;
            break;
        case IR_STATE_READ:// 保存捕获数值
            if (s_i<IR_CODE_LEN)
            {
                g_IR_Code[s_i++]=TIM_GetCounter(TIM4);
                TIM_SetCounter(TIM4,0);

            } else {
                g_IR_State = IR_STATE_END;
                s_i = 0;
            }
            break;
        case IR_STATE_END:// 捕获结束
            break;
        default:
            break;
        }
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);   //清除中断标志
        TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
        TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);   //开溢出中断

    }
    else if(TIM_GetITStatus(TIM4,TIM_IT_Update)== SET)// TIM溢出，说明脉宽超出设定，码值无效
    {
        if (g_IR_State == IR_STATE_READ)      // 读红外码时溢出说明红外码无效,清除数据,返回无数据状态
        {
            g_IR_State = IR_STATE_NO;
            s_i = 0;
        } else if (g_IR_State == IR_STATE_END) // 如果红外读取结束,则说明数据有效
        {
            g_IR_State = IR_STATE_OK;
        }
        TIM_ClearITPendingBit(TIM4,TIM_IT_Update); //清楚溢出标志
        TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);  //关闭溢出中断
    }
}




