//发送数据接口
static bool GPRS_UART_SendData(u8 DataBuff[], u16 DataLen)
{
	UARTx_SendData(SIMCOM_UART_CH, DataBuff, DataLen);
	
	return TRUE;
}
 
 
 
//接收数据接口
static int GPRS_UART_ReadData(u8 **pDataBuff,u8 ByteTimeOutMs, u16 TimeOutMs, u16 *pReceiveDelayMs)
{
	u32 cnt = 0;
	u16 TempTime;
	
	if(ByteTimeOutMs < 1) ByteTimeOutMs = 1;	//字节超时时间，2个帧之间的间隔最小时间
	TimeOutMs /= ByteTimeOutMs;
	TimeOutMs += 1;
	TempTime = TimeOutMs;
	while(TimeOutMs --)
	{
		cnt = UARTx_GetRxCnt(SIMCOM_UART_CH);
		OSTimeDlyHMSM(0,0,0,ByteTimeOutMs);;
		if((cnt > 0) && (cnt == UARTx_GetRxCnt(SIMCOM_UART_CH)))
		{
			if(pReceiveDelayMs!=NULL)	//需要返回延时
			{
				*pReceiveDelayMs = (TempTime-TimeOutMs)*ByteTimeOutMs;
			}
			*pDataBuff = g_SIMCOM_Buff;						//接收缓冲区
			
			return cnt;
		}
#if SYS_WDG_EN_
		IWDG_Feed();										//喂狗
#endif			
	}
	
	return 0;
}
 
 
//清除接收缓冲区
static void GPRS_UART_ClearData(void)
{
	UARTx_ClearRxCnt(SIMCOM_UART_CH);	//清除串口缓冲区
}
/////////////////////////////////////////////////////////////////////////////////////////////
//SIM900/SIM800通信支持
//GSM模块相关定义
#define SIMCOM_UART_CH			UART_CH3			//串口3
#define SIMCOM_UART_BAUD		115200				//波特率
#define SIMCOM_UART_BUFF_SIZE	(1024*4)			//接收缓冲区大小
 
//相关控制引脚
__inline void SIMCOM_SetDTR(u8 Level)		{(PGout(4)=Level);}			//DTR
__inline void SIMCOM_SetPWRKEY(u8 Level)	{(PGout(3)=Level);}			//PWRKEY
__inline u8 SIMCOM_GetSTATUS(void)			{return PGin(5)?1:0;}		//STATUS
__inline u8 SIMCOM_GetDCD(void)				{return PDin(11)?1:0;}		//DCD-上拉输入，高电平AT指令模式，低电平为透传模式
 
//引脚初始化
__inline void SIMCOM_IO_Init(void)	
{
	SYS_DeviceClockEnable(DEV_GPIOD, TRUE);					//使能GPIOD时钟
	SYS_DeviceClockEnable(DEV_GPIOG, TRUE);					//使能GPIOG时钟
	SYS_GPIOx_Init(GPIOG, BIT3|BIT4, OUT_PP, SPEED_2M);		//推挽输出
	SYS_GPIOx_OneInit(GPIOD, 11, IN_IPU, IN_NONE);			//DCD 上拉输入
	SYS_GPIOx_OneInit(GPIOG, 5, IN_IPD, IN_NONE);			//STATUS 下拉输入
}


