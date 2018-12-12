#include "I2C.h"
//#include "delay.h"
void delay_us(u32 nus)
{		
	u32 temp;	
	for(;nus>0;nus--)
	for(temp=0;temp<100;temp++)
		;
}

void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
      				
}
void i2c_init(void)
{
	/*GPIO_InitTypeDef GPIO_Init_Structure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_Init_Structure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
	GPIO_Init_Structure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Init_Structure);
	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);*/

	

	__HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};	
	/*Configure GPIO pin Output Level */
	/*Configure GPIO pins : scl_Pin sda_Pin */
	GPIO_InitStruct.Pin = scl_Pin|sda_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, scl_Pin|sda_Pin, GPIO_PIN_SET);	



	
}

void SDA_IN(void)
{
   /* GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);*/

	/*Configure GPIO pin Output Level */
	/*Configure GPIO pins : scl_Pin sda_Pin */
   GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = sda_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	
}
void SDA_OUT(void)
{
    /*GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);*/
     GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = sda_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	

	
}

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();   
	IIC_SDA_SET;	  	  
	IIC_SCL_SET;
	delay_us(5);
 	IIC_SDA_RESET;
	delay_us(5);
	IIC_SCL_RESET;
}

//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();
	//IIC_SCL_RESET;
	IIC_SDA_RESET;
	IIC_SCL_SET;
 	delay_us(5);
	//IIC_SCL_SET; 
	IIC_SDA_SET;
	delay_us(5);							   	
}

void IIC_Ack(void)
{
	IIC_SCL_RESET;
	SDA_OUT();
	IIC_SDA_RESET;
	delay_us(5);
	IIC_SCL_SET;
	delay_us(5);
	IIC_SCL_RESET;
}

void IIC_NAck(void)
{
	IIC_SCL_RESET;
	SDA_OUT();
	IIC_SDA_SET;
	delay_us(5);
	IIC_SCL_SET;
	delay_us(5);
	IIC_SCL_RESET;
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0; 
	IIC_SDA_SET;delay_us(5);
	SDA_IN();      //SDA设置为输入	   
	IIC_SCL_SET;delay_us(5); 	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			//IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_RESET;//时钟输出0 	   
	return 0;  
}


void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL_RESET;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
       // IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA_SET;
		else
			IIC_SDA_RESET;
		txd<<=1; 	  
		delay_us(5);   //对TEA5767这三个延时都是必须的
		IIC_SCL_SET;
		delay_us(5); 
		IIC_SCL_RESET;	
		delay_us(5);
    }	 
} 

u8 IIC_Read_Byte(void )
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL_RESET; 
        delay_us(5);
		IIC_SCL_SET;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(5); 
    }
	return receive;
}
