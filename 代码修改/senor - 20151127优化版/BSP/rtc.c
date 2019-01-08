void SYS_RTCInit(void)
{
    static NVIC_InitTypeDef  NVIC_InitStructure;
    static EXTI_InitTypeDef  EXTI_InitStructure;
    static RTC_InitTypeDef RTC_InitStructure;
    static RTC_TimeTypeDef RTC_TimeStructure;
    static uint32_t AsynchPrediv = 0, SynchPrediv = 0;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /*!< Allow access to RTC */
    PWR_RTCAccessCmd(ENABLE);

    /*!< 使能外部低速时钟 */
    RCC_LSEConfig(RCC_LSE_ON);

    /*!< 等待外部低速时钟就绪 */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {}

    /*!< 配置RTC时钟来源 */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /*!< 使能RTC时钟 */
    RCC_RTCCLKCmd(ENABLE);

    /*!< Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();

    //配置日历
    /* 配置RTC数据寄存器以及时钟分频 */
    SynchPrediv = 0xFF;
    AsynchPrediv = 0x7F;//分频为1Hz
    RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
    RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;//24小时制
    /* 检查RTC初始化 */
    if (RTC_Init(&RTC_InitStructure) == ERROR)
    {
        printf("\n\r        /!\\***** RTC Prescaler Config failed ********/!\\ \n\r");
    }
    //配置时间 9点15分01秒
    RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
    RTC_TimeStructure.RTC_Hours = 0x09;
    RTC_TimeStructure.RTC_Minutes = 0x15;
    RTC_TimeStructure.RTC_Seconds = 0x01;
    RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);

    //配置自动定时功能
    /* 中断配置 *******************************************************/
    EXTI_ClearITPendingBit(EXTI_Line20);
    EXTI_InitStructure.EXTI_Line = EXTI_Line20;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*使能 RTC Wakeup Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* RTC 唤醒中端配置: Clock Source: RTCDiv_16, Wakeup Time Base: 4s */
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
    RTC_SetWakeUpCounter(0x1FFF);//0x1FFF = 8191; 4s = (8191+1)*(1/(32768/16))

    /* 使能 the Wakeup Interrupt */
    RTC_ITConfig(RTC_IT_WU    RTC_ITConfig(RTC_IT_WUT, EN    /* RTC 唤醒中端配置: Clock Source: RTCDiv_16, Wakeup Time Base: 4s */
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
    RTC_SetWakeUpCounter(0x1FFF);//0x1FFF = 8191; 4s = (8191+1)*(1/(32768/16))

    /* 使能 the Wakeup Interrupt */
    RTC_IT    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*使能 RTC Wakeup Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* RTC 唤醒中端配置: Clock Source: RTCDiv_16, Wakeup Time Base: 4s */
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
    RTC_SetWakeUpCounter(0x1FFF);//0x1FFF = 8191; 4s = (8191+1)*(1/(32768/16))

    /* 使能 the Wakeup Interrupt */
    RTC_ITConfig(RTC_IT_WUT, ENABLE);  
}
void RTC_WKUP_IRQn(void)
{
	if(RTC_GetITStatus(RTC_IT_WUT))
	{
		RTC_ClearFlag(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_	if(RTC_GetITStatus(RTC_IT_WUT))
	{
		RT    RTC_SetWakeUpCounter(0x1FFF);//0x1FFF = 8191; 4s = (8191+1)*(1/(32768/16))

    /* 使能 the Wakeup Interrupt */
    RTC_ITConfig(RTC_IT_WUT, ENABLE);

}

void RTC_WKUP_IRQn(void)
{
	if(RTC_GetITStatus(RTC_IT_WUT))
	{
		RTC_ClearFlag(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line20);

	}		EXTI_ClearITPendingBit(		RTC_ClearFlag(RTC_IT_WUT);
		EXTI_Clea    RTC_Format = RTC_Format_BCD; //与初始化设置保持一致就可
	RTC_GetDate( RTC_Format,  RTC_DateStruct);
	RTC_GetTime( RTC_Format,  RTC_TimeStruct);	RTC_TimeDateStructure.date = RTC_DateStruct.RTC_Date;
	RTC_TimeDateStructure.month = RTC_DateStruct.RTC_Month;
	RTC_TimeDateStructure.year = RTC_DateStruct.RTC_Year;

	RTC_TimeDateStructure.hour = RTC_TimeStruct.RTC_Hours;
	RTC_TimeDateStructure.minute = RTC_TimeStruct.RTC_Minutes;
	RTC_TimeDateStructure.se	RTC_TimeDateStructure.second = RTC_TimeStruct.RTC_Seconds;
}




