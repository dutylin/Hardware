//#include "oled.h"
#ifndef __OLED_H
#define __OLED_H

/* includes */
#include "stm32l1xx.h"
#include "stm32l1xx_conf.h"



#define     OLED_RST_L                      GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define     OLED_RST_H                      GPIO_SetBits(GPIOB, GPIO_Pin_12)


#define	    OLED_DC_L                       GPIO_ResetBits(GPIOC, GPIO_Pin_6);
#define     OLED_DC_H                       GPIO_SetBits(GPIOC, GPIO_Pin_6);

#define  	OLED_SCK_PIN					GPIO_Pin_13
#define		OLED_SDA_PIN					GPIO_Pin_15

#define Select_OLED       GPIO_ResetBits(GPIOB, GPIO_Pin_2);
#define NotSelect_OLED    GPIO_SetBits(GPIOB, GPIO_Pin_2);
   
/* OLED_WriteByte */
void OLED_WB(uint8_t data);
//void LCD_Init(void);
//void LCD_CLS(void);
void LCD_Fill(unsigned char dat);

#endif

#include "stm32l1xx.h"
//#include "stm32f10x_spi.h"
#include "codetab.h"

#define     XLevelL		    0x00
#define     XLevelH		    0x10
#define     XLevel		    ((XLevelH&0x0F)*16+XLevelL)
#define     Max_Column	    128
#define     Max_Row		    64
#define	    Brightness	    0xCF



#define     X_WIDTH         128
#define     Y_WIDTH         64
#define		Page			8

void Draw_BMP(unsigned char x0, unsigned char y0,unsigned char x1,unsigned char y1,const unsigned char * BMP);
void LCD_DLY_ms(unsigned int ms);
void LCD_P6x8Str(unsigned char x, unsigned char y,unsigned char ch[]);
void LCD_P8x16Str(unsigned char x, unsigned char y,unsigned char ch[]);
void LCD_P16x16Ch(unsigned char x, unsigned char y, unsigned char N, const unsigned char * ch);  
void LCD_P32x32Ch(unsigned char x, unsigned char y, unsigned char N, const unsigned char * ch);  
//void Delay(__IO uint32_t nTime);
//////////////////////////////////////////////////////
void OLED_WB(uint8_t data){
    /* Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    /* Send byte through the SPI2 peripheral */
    SPI_I2S_SendData(SPI2, data);
}
/*******************一个字节数据写入***********************/
void LCD_WrDat(unsigned char dat)
{
    OLED_DC_H;		
	OLED_WB(dat);
}

/********************一条指令写入**********************/
void LCD_WrCmd(unsigned char cmd)
{	
	OLED_DC_L;	  	
    OLED_WB(cmd);
}

/**********************设置显示位置**********************/
void LCD_Set_Pos(unsigned char x, unsigned char y)
{
	LCD_WrCmd(0x21);//-Set Column Address
	LCD_WrCmd(0x00+x);
	LCD_WrCmd(0x7f);

	LCD_WrCmd(0x22);//-Set Page Address
	LCD_WrCmd(0x00+y);
	LCD_WrCmd(0x07);
	//Delay(1);/* 等待内部稳定   */
	delay_ms(1);
}



/*********************清屏函数***********************/
void LCD_CLS(void)
{
	unsigned char y,x;
	LCD_WrCmd(0x21);//-Set Column Address
	LCD_WrCmd(0x00);
	LCD_WrCmd(0x7f);   
	//Delay(1);/* 等待内部稳定   */
	delay_ms(1);
	for(y=0;y<8;y++)
	{
		LCD_WrCmd(0x22);//-Set Page Address
		LCD_WrCmd(0x00+y);
		LCD_WrCmd(0x07);
		//Delay(1);/* 等待内部稳定   */
		delay_ms(1);
		for(x=0;x<X_WIDTH;x++) LCD_WrDat(0x00);
		//Delay(1);
		delay_ms(1);
	}
}



/*********************12864初始化***********************/
void LCD_Init(void)
{
	OLED_RST_L;
	//Delay(100);
	delay_ms(100);
	//delay_ms(200);
	OLED_RST_H;
	//从上电到下面开始初始化要有足够的时间，即等待RC复位完毕

    LCD_WrCmd(0xae);//--turn off oled panel

    LCD_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
    LCD_WrCmd(0x3f);//--1/64 duty
    LCD_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    LCD_WrCmd(0x00);//-not offset
    LCD_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    //LCD_WrCmd(0xa0);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    //LCD_WrCmd(0xc0);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常

	LCD_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    LCD_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常


    LCD_WrCmd(0xda);//--set com pins hardware configuration
    LCD_WrCmd(0x12);
    LCD_WrCmd(0x81);//--set contrast control register
    LCD_WrCmd(0xcf); // Set SEG Output Current Brightness
    LCD_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
    LCD_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
    LCD_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
    LCD_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    LCD_WrCmd(0x8d);//--set Charge Pump enable/disable
    LCD_WrCmd(0x14);//--set(0x10) disable
    LCD_WrCmd(0xaf);//--turn on oled panel

    LCD_WrCmd(0xd9);//--set pre-charge period
    LCD_WrCmd(0xf8);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock

    LCD_WrCmd(0xdb);//--set vcomh
    LCD_WrCmd(0x40);//Set VCOM Deselect Level

						
}

/***************显示6*8 ASCII码****************/
void LCD_P6x8Str(unsigned char x, unsigned char y,unsigned char ch[])
{
unsigned char c=0,i=0,j=0;      
	while (ch[j]!='\0'){    
		c =ch[j]-32;
		if(x>126){x=0;y++;}
		LCD_Set_Pos(x,y);    
		for(i=0;i<6;i++) {LCD_WrDat(F6x8[c][i]);delay_ms(1);/*Delay(1); 等待内部稳定 */} 
		x+=6;
		j++;
	}
}
/*******************显示8*16 ASCII码****************/
void LCD_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[])
{
unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0'){    
		c =ch[j]-32;
		if(x>120){x=0;y++;}			    //因为是横向8像素一个字符，所以大于120  要换行显示。 
		LCD_Set_Pos(x,y);    
		for(i=0;i<8;i++) {LCD_WrDat(F8X16[c*16+i]);/*Delay(1);*/delay_ms(1);}	     
		LCD_Set_Pos(x,y+1);    
		for(i=0;i<8;i++){LCD_WrDat(F8X16[c*16+i+8]);  /*Delay(1);*/ delay_ms(1);}
		x+=8;
		j++;
	}
	
}


/*****************显示16*16字符****************************/
void LCD_P16x16Ch(unsigned char x, unsigned char y, unsigned char N, const unsigned char * ch)
{
	unsigned char wm=0,i;
	unsigned int adder=0;
	for(i=0; i<N; i++){	
		LCD_Set_Pos(x+16*i , y);
		for(wm = 0;wm < 16;wm++){
			LCD_WrDat(ch[adder]);	
			//Delay(1);
			delay_ms(1);
			adder += 1;
		}      
		LCD_Set_Pos(x+16*i,y + 1); 
		for(wm = 0;wm < 16;wm++){
			LCD_WrDat(ch[adder]);
 			//Delay(1);
			delay_ms(1);
			adder += 1;
		} 	
	}  	
}
void LCD_P32x32Ch(unsigned char x, unsigned char y, unsigned char N, const unsigned char * ch)
{
	unsigned char wm=0,i;
	unsigned int adder=0;
	for(i=0; i<N; i++){	
		LCD_Set_Pos(x+32*i , y);
		for(wm = 0;wm < 32;wm++){
			LCD_WrDat(ch[adder]);	
			//Delay(1);
			delay_ms(1);
			adder += 1;
		}      
		LCD_Set_Pos(x+32*i,y + 1); 
		for(wm = 0;wm < 32;wm++){
			LCD_WrDat(ch[adder]);
 			//Delay(1);
			delay_ms(1);
			adder += 1;
		} 	
	}  	
}
/***********显示图片*****************/
void Draw_BMP(unsigned char x0, unsigned char y0,unsigned char x1,unsigned char y1,const unsigned char * BMP)
{ 	
	unsigned int j=0;
  	unsigned char x,y;
  	if(y1%8==0) y=y1/8;      
  	else y=y1/8+1;
 	for(y=y0;y<y1;y++){
		LCD_Set_Pos(x0,y);				
   	    for(x=x0;x<x1;x++){      
	    	LCD_WrDat(BMP[j++]);
			//Delay(1);  
        delay_ms(1);					
	    }
	}
} 
