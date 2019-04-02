#include "lora_app.h"
#include "lora_ui.h"
#include "lora_cfg.h"
#include "usart1.h"
#include "string.h"
#include "systick_timer.h"
#include "stdio.h"
#include "usart.h"

/************************************************
 ALIENTEK 阿波罗STM32F429开发板
 ATK-LORA-01模块功能驱动
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com  
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

//设备参数初始化(具体设备参数见lora_cfg.h定义)
_LoRa_CFG LoRa_CFG=
{
	.addr = LORA_ADDR,       //设备地址
	.power = LORA_POWER,     //发射功率
	.chn = LORA_CHN,         //信道
	.wlrate = LORA_RATE,     //空中速率
	.wltime = LORA_WLTIME,   //睡眠时间
	.mode = LORA_MODE,       //工作模式
	.mode_sta = LORA_STA,    //发送状态
	.bps = LORA_TTLBPS ,     //波特率设置
	.parity = LORA_TTLPAR    //校验位设置
};


GPIO_InitTypeDef GPIO_Initure;

void LORA_AUX(unsigned char flag)
{

     if(flag==0)
     {
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
     }
     else
     {
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);

    }
}
uint32_t LORA_AUX_READ()
{

return HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_11);
}

void LORA_MD0(unsigned char flag)
{

     if(flag==0)
     {
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET);
     }
     else
     {
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);

    }
}
uint32_t LORA_MD0_READ()
{

return HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_11);
}

//设备工作模式(用于记录设备状态)
u8 Lora_mode=0;// 0:配置模式 1:接收模式 2:发送模式

//记录中断状态
static u8 Int_mode=0;//0：关闭 1:上升沿 2:下降沿

//AUX中断设置
//mode:配置的模式 0:关闭 1:上升沿 2:下降沿
void Aux_Int(u8 mode)
{
	
	if(!mode)
	{
		HAL_NVIC_DisableIRQ(EXTI4_IRQn);//失能中断线11					
	}else
	{
		if(mode==1)
		{
			 GPIO_Initure.Pin=GPIO_PIN_4; //PI11
             GPIO_Initure.Mode=GPIO_MODE_IT_RISING;//上升沿触发
		}
			
	    else if(mode==2)
		{
			 GPIO_Initure.Pin=GPIO_PIN_4; //PI11
             GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;//下降沿触发
		}
		
		 HAL_GPIO_Init(GPIOA,&GPIO_Initure);//重新初始化
	     __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);//清除中断标志位
		 HAL_NVIC_SetPriority(EXTI4_IRQn,2,0); //抢占优先级为2，子优先级为0
		 HAL_NVIC_EnableIRQ(EXTI4_IRQn);//使能
	 
	}
	Int_mode = mode;//记录中断模式
}

void EXTI4_IRQn_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);//调用中断处理公用函数	
}

////中断服务程序中需要做的事情
////在HAL库中所有的外部中断服务函数都会调用此函数
////GPIO_Pin:中断引脚号
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==GPIO_PIN_4)
	{
	   if(Int_mode==1)//上升沿(发送:开始发送数据 接收:数据开始输出)     
	   {
		  if(Lora_mode==1)//接收模式
		  {
			 USART1_RX_STA=0;//数据计数清0
		  }
		  Int_mode=2;//设置下降沿
		  //LED0=0;//DS0亮
	   }
       else if(Int_mode==2)//下降沿(发送:数据已发送完 接收:数据输出结束)	
	   {
		  if(Lora_mode==1)//接收模式
		  {
			 USART1_RX_STA|=1<<15;//数据计数标记完成
		  }else if(Lora_mode==2)//发送模式(串口数据发送完毕)
		  {
			 Lora_mode=1;//进入接收模式
		  }
		  Int_mode=1;//设置上升沿
         // LED0=1;//DS0灭		   
	   }
       Aux_Int(Int_mode);//重新设置中断边沿
 	
	}
	
}

//LoRa模块初始化
//返回值:0,检测成功
//       1,检测失败
u8 LoRa_Init(void)
{
	 u8 retry=0;
	 u8 temp=1;
	
	 __HAL_RCC_GPIOA_CLK_ENABLE();          //开启GPIOA时钟
	//__HAL_RCC_GPIOI_CLK_ENABLE();           //开启GPIOI时钟
	 
	GPIO_Initure.Pin=GPIO_PIN_5;           //PA4
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP; //推挽输出
	GPIO_Initure.Pull=GPIO_PULLDOWN;       //下拉
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);

	GPIO_Initure.Pin=GPIO_PIN_4;          //PI11
	GPIO_Initure.Mode=GPIO_MODE_IT_RISING; //上升沿触发
	GPIO_Initure.Pull=GPIO_PULLDOWN;       //下拉
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	//LORA_MD0(0);
	LORA_AUX(0);
	
	while(LORA_AUX_READ())//确保LORA模块在空闲状态下(LORA_AUX(0))
	{
		 //Show_Str(40+30,50+20,200,16,"模块正忙,请稍等!!",16,0); 	
		 delay_ms(500);
		// Show_Str(40+30,50+20,200,16,"                    ",16,0);
         delay_ms(100);		 
	}
	usart1_init(115200);//初始化串口3
	 
	LORA_MD0(1);//进入AT模式
	delay_ms(40);
	retry=3;
	while(retry--)
	  {
		 if(!lora_send_cmd("AT","OK",70))
		 {
			 temp=0;//检测成功
			 break;
		 }	
	  }
	  if(retry==0) temp=1;//检测失败
	  return temp;
}

//Lora模块参数配置
void LoRa_Set(void)
{
	u8 sendbuf[20];
	u8 lora_addrh,lora_addrl=0;
	
	usart1_set(LORA_TTLBPS_115200,LORA_TTLPAR_8N1);//进入配置模式前设置通信波特率和校验位(115200 8位数据 1位停止 无数据校验）
	usart1_rx(1);//开启串口3接收
	
	while(LORA_AUX_READ());//等待模块空闲
	LORA_MD0(1); //进入配置模式
	delay_ms(40);
	Lora_mode=0;//标记"配置模式"
	
	lora_addrh =  (LoRa_CFG.addr>>8)&0xff;
	lora_addrl = LoRa_CFG.addr&0xff;
	sprintf((char*)sendbuf,"AT+ADDR=%02x,%02x",lora_addrh,lora_addrl);//设置设备地址
	lora_send_cmd(sendbuf,"OK",50);
	sprintf((char*)sendbuf,"AT+WLRATE=%d,%d",LoRa_CFG.chn,LoRa_CFG.wlrate);//设置信道和空中速率
	lora_send_cmd(sendbuf,"OK",50);
	sprintf((char*)sendbuf,"AT+TPOWER=%d",LoRa_CFG.power);//设置发射功率
	lora_send_cmd(sendbuf,"OK",50);
	sprintf((char*)sendbuf,"AT+CWMODE=%d",LoRa_CFG.mode);//设置工作模式
	lora_send_cmd(sendbuf,"OK",50);
	sprintf((char*)sendbuf,"AT+TMODE=%d",LoRa_CFG.mode_sta);//设置发送状态
	lora_send_cmd(sendbuf,"OK",50);
	sprintf((char*)sendbuf,"AT+WLTIME=%d",LoRa_CFG.wltime);//设置睡眠时间
	lora_send_cmd(sendbuf,"OK",50);
	sprintf((char*)sendbuf,"AT+UART=%d,%d",LoRa_CFG.bps,LoRa_CFG.parity);//设置串口波特率、数据校验位
	lora_send_cmd(sendbuf,"OK",50);

	LORA_MD0(0);//退出配置,进入通信
	delay_ms(40);
	while(LORA_AUX_READ());//判断是否空闲(模块会重新配置参数)
	USART1_RX_STA=0;
	Lora_mode=1;//标记"接收模式"
	usart1_set(LoRa_CFG.bps,LoRa_CFG.parity);//返回通信,更新通信串口配置(波特率、数据校验位)
	Aux_Int(1);//设置LORA_AUX上升沿中断
	
	
}


u8 Dire_Date[]={0x11,0x22,0x33,0x44,0x55};//定向传输数据
u8 date[30]={0};//定向数组
u8 Tran_Data[30]={0};//透传数组

#define Dire_DateLen sizeof(Dire_Date)/sizeof(Dire_Date[0])
extern u32 obj_addr;//记录用户输入目标地址
extern u8 obj_chn;//记录用户输入目标信道

u8 wlcd_buff[10]={0}; //LCD显示字符串缓冲区
//Lora模块发送数据
void LoRa_SendData(void)
{      
	static u8 num=0;
    u16 addr;
	u8 chn;
	u16 i=0; 
		
	if(LoRa_CFG.mode_sta == LORA_STA_Tran)//透明传输
	{
		sprintf((char*)Tran_Data,"ATK-LORA-01 TEST %d",num);
		u1_printf("%s\r\n",Tran_Data);
		//LCD_Fill(0,195,240,220,WHITE);//清除显示
	//	Show_Str_Mid(10,195,Tran_Data,16,240);//显示发送的数据	
		
		num++;
		if(num==255) num=0;
		
	}else if(LoRa_CFG.mode_sta == LORA_STA_Dire)//定向传输
	{
		
		addr = (u16)obj_addr;//目标地址
		chn = obj_chn;//目标信道
		
		date[i++] =(addr>>8)&0xff;//高位地址
		date[i++] = addr&0xff;//低位地址
		date[i] = chn;  //无线信道
		for(i=0;i<Dire_DateLen;i++)//数据写到发送BUFF
		{
			date[3+i] = Dire_Date[i];
		}
		for(i=0;i<(Dire_DateLen+3);i++)
		{
			while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TXE)== RESET);//循环发送,直到发送完毕   
			HAL_UART_Transmit(&UART1_Handler,&date[i],1,1000);	
		}	
		
        //将十六进制的数据转化为字符串打印在lcd_buff数组
		sprintf((char*)wlcd_buff,"%x %x %x %x %x %x %x %x",
				date[0],date[1],date[2],date[3],date[4],date[5],date[6],date[7]);	
		//LCD_Fill(0,200,240,230,WHITE);//清除显示
	//	Show_Str_Mid(10,200,wlcd_buff,16,240);//显示发送的数据		
		
	    Dire_Date[4]++;//Dire_Date[4]数据更新	
	}
			
}

u8 rlcd_buff[10]={0}; //LCD显示字符串缓冲区
//Lora模块接收数据
void LoRa_ReceData(void)
{
    u16 i=0;
    u16 len=0;
   
	//有数据来了
	if(USART1_RX_STA&0x8000)
	{
		len = USART1_RX_STA&0X7FFF;
		USART1_RX_BUF[len]=0;//添加结束符
		USART1_RX_STA=0;

		for(i=0;i<len;i++)
		{
			while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TXE)== RESET);//循环发送,直到发送完毕   
			HAL_UART_Transmit(&UART1_Handler,&USART1_RX_BUF[i],1,1000);	
		}
		// LCD_Fill(10,260,240,320,WHITE);
		if(LoRa_CFG.mode_sta==LORA_STA_Tran)//透明传输
		{	
		//	Show_Str_Mid(10,270,USART1_RX_BUF,16,240);//显示接收到的数据

		}else if(LoRa_CFG.mode_sta==LORA_STA_Dire)//定向传输
		{
			//将十六进制的数据转化为字符串打印在lcd_buff数组
			sprintf((char*)rlcd_buff,"%x %x %x %x %x",
			USART1_RX_BUF[0],USART1_RX_BUF[1],USART1_RX_BUF[2],USART1_RX_BUF[3],USART1_RX_BUF[4]);
				
		//	Show_Str_Mid(10,270,rlcd_buff,16,240);//显示接收到的数据	
		}
		memset((char*)USART1_RX_BUF,0x00,len);//串口接收缓冲区清0

	}

}

//发送和接收处理
void LoRa_Process(void)
{
   u8 key=0;
   u8 t=0;
 DATA:
	Process_ui();//界面显示
	LoRa_Set();//LoRa配置(进入配置需设置串口波特率为115200,) 
	while(1)
	{
		
		//key = KEY_Scan(0);
		
		if(key==KEY0_PRES)
		{
			if(LoRa_CFG.mode_sta==LORA_STA_Dire)//若是定向传输,则进入配置目标地址和信道界面
			{
				usart1_rx(0);//关闭串口接收
				Aux_Int(0);//关闭中断
				Dire_Set();//进入设置目标地址和信道
				goto DATA;
			}
		}else if(key==WKUP_PRES)//返回主菜单页面
		{
			usart1_rx(0);//关闭串口接收
			Aux_Int(0);//关闭中断
			break;
		}
		else if(key==KEY1_PRES)//发送数据
		{
			  if(!LORA_AUX_READ())//若空闲
			  {
				  Lora_mode=2;//标记"发送状态"
				  LoRa_SendData();//发送数据    
			  }
		}
			
		//数据接收
		LoRa_ReceData();
		
		t++;
		if(t==20)
		{
			t=0;
			//LED1=~LED1;
		}			
		delay_ms(10);		
   }
	
}


//主测试函数
void Lora_Test(void)
{
	u8 t=0;
	u8 key=0;
	u8 netpro=0;
	
	//LCD_Clear(WHITE);
	//POINT_COLOR=RED;
	//Show_Str_Mid(0,30,"ATK-LORA-01 测试程序",16,240); 
	
	while(LoRa_Init())//初始化ATK-LORA-01模块
	{
		//Show_Str(40+30,50+20,200,16,"未检测到模块!!!",16,0); 	
		delay_ms(300);
		//Show_Str(40+30,50+20,200,16,"                ",16,0);
	}
	//Show_Str(40+30,50+20,200,16,"检测到模块!!!",16,0);
    delay_ms(500); 	
	//Menu_ui();//菜单
	
	while(1)
	{

	 LoRa_Process();//开始数据测试
	Menu_cfg(netpro);//参数配置	
		/*key = KEY_Scan(0);
		key=1;
		if(key)
		{
			//Show_Str(30+10,95+45+netpro*25,200,16,"  ",16,0);//清空之前的显示
			
			 if(key==KEY0_PRES)//KEY0按下
			{
				if(netpro<6)netpro++;
				else netpro=0;
			}else if(key==KEY1_PRES)//KEY1按下
			{
				if(netpro>0)netpro--;
				else netpro=6; 
			}else if(key==WKUP_PRES)//KEY_UP按下
			{
				if(netpro==0)//进入通信选项
				{
				  LoRa_Process();//开始数据测试
				  netpro=0;//索引返回第0
				  //Menu_ui();

				}else
				{
					//Show_Str(30+40,95+45+netpro*25+2,200,16,"________",16,1);//显示下划线,表示选中
					//Show_Str(30+10,95+45+netpro*25,200,16,"→",16,0);//指向新条目
					Menu_cfg(netpro);//参数配置
					//LCD_Fill(30+40,95+45+netpro*25+2+15,30+40+100,95+45+netpro*25+2+18,WHITE);//清除下划线显示
				}
			}
			//Show_Str(30+10,95+45+netpro*25,200,16,"→",16,0);//指向新条目
		}
		
		t++;
		if(t==30)
		{
			t=0;
			LED1=~LED1;
		}
		delay_ms(10);*/
	}	
	
}


