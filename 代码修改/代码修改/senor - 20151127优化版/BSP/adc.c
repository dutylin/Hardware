#include "global.h"
#include "adc.h"

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

unsigned int Alfter_filter[4];
/***************************传感器变量相关****************************/
extern uint32_t CV_Value[4]= {1,1,1,1};
/**************************************************************
函数：void ADC_Config(void)
功能：ADC
**************************************************************/
void ADC_Config(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC->AHBENR |= RCC_AHBPeriph_GPIOA;

#if (defined Liceng)||(defined Weiyan)||(defined Yali)
    /* 通道1、2对应PA4/PA5 */
    //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

#elif (defined Maogan)||(defined Maosuo)
    /* 通道1、2对应PA4/PA5 3、4通道对应PA6/PA7*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
    RCC_HSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
    {
    }
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    //ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Resolution=ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    ADC_PowerDownCmd(ADC1, ADC_PowerDown_Idle_Delay, ENABLE);
    ADC_DelaySelectionConfig(ADC1,ADC_DelayLength_Freeze);
    ADC_Cmd(ADC1,ENABLE);
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_ADONS) == RESET)
    {
    }
}
//uint32_t CV_Value[4];
/**************************************************************
函数：void Get_ADC(uint8_t channel)
功能：ADC转换获取
**************************************************************/
void Get_ADC(uint8_t channel)
{
    switch(channel)
    {
    case 0:
        ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_192Cycles);
		    //CV_Value[channel]=0x00;
        break;
    case 1:
        ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_192Cycles);
		    //CV_Value[channel]=0x00;
        break;
    case 2:
        ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_192Cycles);
		    //CV_Value[channel]=0x00;
        break;
    //case 3:
    //    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_192Cycles);
     //   break;
    }
    ADC_Cmd(ADC1,ENABLE);
    ADC_SoftwareStartConv(ADC1); //使能指定的ADC1的软件转换启动功能
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//等待转换结束
#ifdef Liceng  //基准电压3300mV 离层0-300mm    采集电压=AD值*3300/4096  0~2500mv->0~300
    //CV_Value[channel]=ADC_GetConversionValue(ADC1);
		//CV_Value[channel]=(unsigned int)((194061*(ADC_GetConversionValue(ADC1)-0x0140))>>19);
//CV_Value[channel]=(unsigned int)((6300*(ADC_GetConversionValue(ADC1)-0x0263))>>15)-0x02;
//		if(channel==1)
//		{
		CV_Value[channel]=(unsigned int)((9200*(ADC_GetConversionValue(ADC1)-0x1B9))>>15)+0x10;//为了测试数据暂时注释掉
		if(CV_Value[channel]>0x258) 
		{
			CV_Value[channel]=0;
		}
		else if(CV_Value[channel]>0x00&CV_Value[channel]<0x20) 
		{
			CV_Value[channel]=0;
		}
	
//		}
//		else
//		{
//		CV_Value[channel]=(unsigned int)((8813*(ADC_GetConversionValue(ADC1)-0x1B9))>>15)+0x76;
//		}
			////CV_Value[channel]=(unsigned int)((3150*(ADC_GetConversionValue(ADC1)-0x0131))>>15)-0x04;
		 //CV_Value[channel]=(unsigned int)((1645*(ADC_GetConversionValue(ADC1)+0x0F21))>>10);
		
		
#elif (defined Maogan)||(defined Weiyan)||(defined Yali)           //0.5~4.5->0~600
    CV_Value[channel]=(unsigned int)((825*ADC_GetConversionValue(ADC1))>>13-37.5);
#elif defined Maosuo
    CV_Value[channel]=(unsigned int)((825*ADC_GetConversionValue(ADC1))>>12-75);
#endif
    ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
    ADC_SoftwareStartConv(ADC1);
    ADC_Cmd(ADC1,DISABLE);
}

//300/2500*(AD*3300/4096)



