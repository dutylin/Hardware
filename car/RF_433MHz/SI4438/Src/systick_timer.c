#include "main.h"
#include "systick_timer.h"



//采样、延时时间设置,
/*    0:LTE运行延时
        1:433MHZ延时时间
        2:GPS采样频率
        3:433MHZ采样频率
        4:其他
*/

uint32_t Time_delay[systick_count];//采样、延时时间设置

unsigned int   RF_Delay_Time =10;
unsigned int   TxRx_Start_Time =150;

Module_Sample_Time sample_time_flag;

uint32_t TimingDelay;
//系统延时函数，单位：ms
void delay_us(uint32_t nus)
{
	uint32_t us;
	us = nus;
	while(us != 0)
		us--;
}
//系统延时函数，单位：ms
void delay_ms(uint32_t nms)
{

	TimingDelay = nms;
	while(TimingDelay != 0);
}
//系统定时函数，非阻塞，单位：ms

void noblock_delay_ms(unsigned char index,uint32_t nms)
{
	Time_delay[index]=nms;
}
uint32_t get_Time_delay(unsigned char index)
{
	return Time_delay[index];
}


uint32_t Time_delay[systick_count];
/*
各模块定时器延时
*/
void Sample_timer_dec()
{
	unsigned char i;
	for(i=0; i<systick_count; i++)
	{
		if(Time_delay[i]>0)
			Time_delay[i]=Time_delay[i]-1;
	}

}



unsigned char  Sample_Time_Interval_Cmp(uint32_t src,uint32_t dst,unsigned char index)
{
	if(src!=dst)
	{
		//EEPROM_Store(dst,index);
		return 1;
	}
	return 0;

}
//设置采样频率、延时时间
void Sample_Time_Interval_Setting(unsigned char index,uint32_t value)
{
	switch(index)
	{
	case RF_DELAY_INDEX:
	{
		if(Sample_Time_Interval_Cmp(RF_Delay_Time,value,index))
			RF_Delay_Time=value;

	}break;

	case TxRx_Start_INDEX:
	{
		if(Sample_Time_Interval_Cmp(TxRx_Start_Time,value,index))
			TxRx_Start_Time=value;

	}break;

	}
	//保存参数到flash中，掉电保存


}
Module_Sample_Time Get_Sample_flag_Status()
{
	return sample_time_flag;
}


void Reset_Sample_flag(unsigned char index)
{
	switch (index)
	{
	case 0:Time_delay[0] = RF_Delay_Time;
		sample_time_flag.RF_delay_flag=0;
		break;
	case 1:Time_delay[1] = TxRx_Start_Time;
		sample_time_flag.TXRX_sample_flag=0;
		break;	
	}

}


void Sample_Timer_Init()
{
   unsigned char i;
	for(i=0; i<systick_count; i++)
	{
		if(Time_delay[i]==0)
		{
			switch(i)
			{
			case 0:
			{
				Time_delay[i] = RF_Delay_Time;
				sample_time_flag.RF_delay_flag=0x00;
			}break;

			case 1:
			{
				Time_delay[i] = TxRx_Start_Time;
				sample_time_flag.TXRX_sample_flag=0x00;

			}break;
			
			}
			//Sample_Time_Interval_Setting(i,value);
		}
	}

}

//各模块定时器重新计时加载
void Sample_Timer_Reload()
{
	unsigned char i;
	for(i=0; i<systick_count; i++)
	{
		if(Time_delay[i]==0)
		{
			switch(i)
			{
			case 0:
			{
				Time_delay[i] = RF_Delay_Time;
				sample_time_flag.RF_delay_flag=0x01;
			}break;

			case 1:
			{
				Time_delay[i] =TxRx_Start_Time;
				sample_time_flag.TXRX_sample_flag=0x01;

			}break;
			}
			//Sample_Time_Interval_Setting(i,value);
		}
	}

}



void HAL_SYSTICK_Callback(void)
{

	if (TimingDelay != 0x00000000)
	{
		TimingDelay--;
	}

	Sample_timer_dec();


}
