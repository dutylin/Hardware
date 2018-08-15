#include "main.h"
#include "systick_timer.h"
#include "stm32f4xx_hal.h"
#include "ds18b20.h"
#include "ds18b20_BSP.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//DS18B20驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/1/16
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//复位DS18B20
void DS18B20_Rst(void)	   
{   
   unsigned char 	presence;
	//DS18B20_IO_OUT();   //设置为输出
  DS18B20_DQ_OUT(1);  //拉低DQ
  delay_us(50);      //拉低750us
  DS18B20_DQ_OUT(0);  //DQ=1 
	delay_us(470);       //15US
	 DS18B20_DQ_OUT(1);  //拉低DQ

             //稍做延时
   delay_us(40);       //15US
    if(DS18B20_DQ_IN()==0)      //如果=0则初始化成功 =1则初始化失败
     presence = 1;
    else  
	 presence = 0;
          
    delay_us(450);           //  延时470us
     DS18B20_DQ_OUT(1);  //拉低DQ
	
}

//等待DS18B20的回应
//返回1:未检测到DS18B20的存在
//返回0:存在
unsigned char  DS18B20_Check(void) 	   
{   

}

//从DS18B20读取一个位
//返回值：1/0


//从DS18B20读取一个字节
//返回值：读到的数据
unsigned char  DS18B20_Read_Byte(void)   
{        
  unsigned char  i,dat;
  dat=0;
    for (i = 8; i > 0; i--) 
	{

      	DS18B20_DQ_OUT(0);
		   //delay_us(1);      //总线为低电平
      	dat >>= 1;
      	DS18B20_DQ_OUT(1);       //总线为高电平(释放总线)   
      	if(DS18B20_DQ_IN())	//读DQ
      		dat |= 0x80;
      	delay_us(80);
      	DS18B20_DQ_OUT(1);		
    }						    
  return dat;
}

//写一个字节到DS18B20
//dat：要写入的字节
void DS18B20_Write_Byte(unsigned char  dat)     
 {             
    unsigned char  j;
    unsigned char  testb;
  //  DS18B20_IO_OUT();     //设置为输出
for (j= 8; j > 0; j--)	 //写一字节 
	{DS18B20_DQ_OUT(0);
        testb=dat&0x01;
       
        if(testb)       // 写1
        {                           
            DS18B20_DQ_OUT(1);
           // delay_us(60);             
        }
        else            //写0
        {
            DS18B20_DQ_OUT(0);
					}
				delay_us(80);
        DS18B20_DQ_OUT(1);					
				dat=dat>>1;
                                                 
        
    }
}
 
//开始温度转换
void DS18B20_Start(void)
{   						               
    DS18B20_Rst();	   
   // DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
}

//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在    	 
unsigned char  DS18B20_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
	
    GPIO_Initure.Pin=GPIO_PIN_12;           //PB12
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //初始化
 
 	  DS18B20_Rst();
	 // return DS18B20_Check();
}
unsigned char a,b,c,d,e,f,g,h;
//从ds18b20得到温度值
//精度：0.1C
//返回值：温度值 （-550~1250） 
short DS18B20_Get_Temp(void)
{
    unsigned char  temp;
    unsigned char  TL,TH;
    short tem;
    DS18B20_Start ();           //开始转换
	DS18B20_Write_Byte(0x33);   // skip rom
	a=DS18B20_Read_Byte();
		b=DS18B20_Read_Byte();
		c=DS18B20_Read_Byte();
		d=DS18B20_Read_Byte();
		e=DS18B20_Read_Byte();
		f=DS18B20_Read_Byte();
		g=DS18B20_Read_Byte();
		h=DS18B20_Read_Byte();
		
   // DS18B20_Rst();
  //  DS18B20_Check();	
    DS18B20_Start (); 	
    DS18B20_Write_Byte(0xcc);   // skip rom
    DS18B20_Write_Byte(0xbe);   // convert	    
    TL=DS18B20_Read_Byte();     // LSB   
    TH=DS18B20_Read_Byte();     // MSB   
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//温度为负  
    }else temp=1;//温度为正	  	  
    tem=TH; //获得高八位
    tem<<=8;    
    tem+=TL;//获得底八位
    tem=(double)tem*0.625;//转换     
	if(temp)return tem; //返回温度值
	else return -tem;    
}
