#define	DATA_COMMAND	0X40
#define	DISP_COMMAND	0x80
#define	ADDR_COMMAND	0XC0

//TM1638模块引脚定义
int DIO = 2;
int CLK = 1;
int STB = 0; //这里定义了那三个脚
//共阴数码管显示代码
unsigned char tab[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
                           0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
unsigned char num[8];		//各个数码管显示的值                      
 void setup ()
{
  pinMode(STB,OUTPUT);
  pinMode(CLK,OUTPUT);
  pinMode(DIO,OUTPUT); //让三个脚都是输出状态
}
 void loop()
    {
	unsigned char i;
	init_TM1638();	                           //初始化TM1638
	for(i=0;i<8;i++)
	Write_DATA(i<<1,tab[0]);	               //初始化寄存器	
	while(1)
	{
		i=Read_key();                          //读按键值
		if(i<8)
		{
		  num[i]++;
		  while(i==Read_key());		       //等待按键释放
		  if(num[i]>15)
		  num[i]=0;
		  Write_DATA(i*2,tab[num[i]]);
		  Write_allLED(1<<i);
		}
	}
}



void TM1638_Write(unsigned char	DATA)			//写数据函数
{
	unsigned char i;
        pinMode(DIO,OUTPUT);
	for(i=0;i<8;i++)
	{
            digitalWrite(CLK,LOW);
	    if(DATA&0X01)
               digitalWrite(DIO,HIGH);
	    else
               digitalWrite(DIO,LOW);
	    DATA>>=1;
            digitalWrite(CLK,HIGH);
	}
}
unsigned char TM1638_Read(void)					//读数据函数
{
	unsigned char i;
	unsigned char temp=0;;	
        pinMode(DIO,INPUT);//设置为输入
	for(i=0;i<8;i++)
	{
  	    temp>>=1;
            digitalWrite(CLK,LOW);
  	    if(digitalRead(DIO)==HIGH)
  	      temp|=0x80;
            digitalWrite(CLK,HIGH);
               
	}
	return temp;
}
void Write_COM(unsigned char cmd)		//发送命令字
{
        digitalWrite(STB,LOW);
	TM1638_Write(cmd);
        digitalWrite(STB,HIGH);
}
unsigned char Read_key(void)
{
	unsigned char c[4],i,key_value=0;
        digitalWrite(STB,LOW);
	TM1638_Write(0x42);		           //读键扫数据 命令
	for(i=0;i<4;i++)		
  	{
            c[i]=TM1638_Read();
        }
        digitalWrite(STB,HIGH);					           //4个字节数据合成一个字节
	for(i=0;i<4;i++)
	{
            key_value|=c[i]<<i;
        }	
        for(i=0;i<8;i++)
        {
            if((0x01<<i)==key_value)
            break;
        }
	return i;
}
void Write_DATA(unsigned char add,unsigned char DATA)		//指定地址写入数据
{
	Write_COM(0x44);
        digitalWrite(STB,LOW);
	TM1638_Write(0xc0|add);
	TM1638_Write(DATA);
        digitalWrite(STB,HIGH);
}
void Write_allLED(unsigned char LED_flag)					//控制全部LED函数，LED_flag表示各个LED状态
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
	    if(LED_flag&(1<<i))
	      Write_DATA(2*i+1,1);
	    else
	      Write_DATA(2*i+1,0);
        }
}

//TM1638初始化函数
void init_TM1638(void)
{
	unsigned char i;
	Write_COM(0x8b);       //亮度 (0x88-0x8f)8级亮度可调
	Write_COM(0x40);       //采用地址自动加1
        digitalWrite(STB,LOW);		           //
	TM1638_Write(0xc0);    //设置起始地址
	for(i=0;i<16;i++)	   //传送16个字节的数据
	  TM1638_Write(0x00);
        digitalWrite(STB,HIGH);
}

