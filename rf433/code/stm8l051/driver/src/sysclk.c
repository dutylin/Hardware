#include "sysclk.h"


//#define HSE_Enable

/*******************************************************************************
 * 名称: Sysclock_Init
 * 功能: 设置系统时钟频率
 * 形参: 无
 * 返回: 无
 * 说明: 时钟选择

 ******************************************************************************/
void SystemClock_Init(void)
{
//使用宏定义的写法
  
#ifdef  HSE_Enable
     while (!CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE,\
            CLK_CURRENTCLOCKSTATE_DISABLE));
     //切换到外部时钟,并等待时钟却换成功
#else
		CLK_HSICmd(ENABLE);      //使能内部高速时钟源 16MHZ
		CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);  //HSI作为系统时钟源
	  	CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);   //设置系统分频，分频越大系统指令时间越长，功耗越低
	
#endif   

}
