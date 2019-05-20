#include <CMT2300A_EasyDriver.h> 
#include <HopeDuino_UART.h>
#include <HopeDuino_Buzz.h> 
#include <HopeDuino_Disp.h> 

#define  ActiveMode    1

#define LEN  25
#define LEN1 26

int KEY1  = 4;
int KEY2  = 5;
int KEY3  = 18;
int KEY4  = 19;

byte disp0[] = {"C2300A Active-Tx"};
byte disp1[] = {"C2300A PassiveRx"};

byte strTx[]= {"TxCount:        "};
byte strRx[]= {"RxCount:        "};

byte music[2] = {
                    SM_4, 
                      10, 
                    };


byte  str[LEN] = {'H','o','p','e','R','F',' ','C','O','B','-','S','e','r','i','e','s',' ','R','F','M','3','0','0','A'};

byte str1[LEN1] = {'C','m','o','s','t','e','k',' ','N','e','x','t','G','e','n','R','F',' ','C','M','T','2','3','0','0','A'};

byte strcnt[18];
byte getstr[32];

cmt2300aEasy radio;
uartClass    uart;
dispClass    Lcd;
buzzClass    Buzz;

bool change;
word TxCount;
word RxCount;

word CMTBank[12] = {
					0x0000,
					0x0166,
					0x026C,
					0x034D,
					0x04F0,
					0x0580,
					0x0614,
					0x0708,
					0x08B1,
					0x0903,
					0x0A00,
					0x0B78     //用于校准RSSI误差
				   };
				   
word SystemBank[12] = {
				    0x0CA5,
				    0x0DE0,
				    0x0E30,
				    0x0F00,
				    0x1000,
				    0x11F4,
				    0x1210,
				    0x13E2,
				    0x1442,
				    0x1520,
				    0x1600,
				    0x1781				  
				    };

word FrequencyBank[8] = {       //CMT2300 频率设置
					0x1842,
					0x1971,
					0x1ACE,
					0x1B1C,
					0x1C42,
					0x1D5B,
					0x1E1C,
					0x1F1C				    
					     };
					     
word DataRateBank[24] = {    
					0x204C,
					0x2193,
					0x2211,
					0x2310,
					0x24A2,
					0x2512,
					0x260B,
					0x270A,
					0x289F,
					0x295A,
					0x2A29,
					0x2B28,
					0x2CC0,
					0x2D8A,
					0x2E02,
					0x2F53,
					0x3018,
					0x3100,
					0x32B4,
					0x3300,
					0x3400,
					0x3501,
					0x3600,
					0x3700
						};     

word BasebandBank[29] = {
					0x3812,  // 1byte RX_preamble  单位8bit
					0x3908,  // 8byte TX_preamble  
					0x3A00,
					0x3BAA,  //报文头内容为AA
					0x3C04,  //同步字设置为3byte
					0x3D00,  //缺省位
					0x3E00,  //缺省位
					0x3F00,  //缺省位
					0x4000,  //缺省位
					0x4100,  //缺省位
					0x42D4,  //同步字D4
					0x432D,  //同步字2D
					0x44AA,  //同步字AA
					0x4501,  //可变包长
					0x461F,  //31byte的负载
					0x4700,  //Node ID 检测电路设置
					0x4800,  //Node ID 缺省位
					0x4900,  //Node ID 缺省位
					0x4A00,  //Node ID 缺省位
					0x4B00,  //Node ID 缺省位
					0x4C11,  //CRC 使能
					0x4D0F,  //CRC多项式初始值
					0x4E1D,  //CRC多项式初始值
					0x4F00,  //CRC大小顺序
					0x5000,  //白化多项式使能
					0x5102,  //PA打开 定义预发射内容，发1。
					0x5200,  //重复发包的次数
					0x5320,　//包与包的间隔 单位symbol
					0x5410	 //发包完自动清除fifo
						};	

word TXBank[11] = {
					0x5550,
					0x5657,
					0x570B,
					0x5800,
					0x5903,
					0x5A50,
					0x5B00,
					0x5C7F,
					0x5D00,
					0x5E3F,
					0x5F7F															
				};						

void setup()
{

 vInitRadio();
 if(ActiveMode)
 	radio.bGoStandby();
 else
 	{
 	radio.vIntSrcCfg(INT_FIFO_WBYTE_RX, INT_CRC_PASS);	
	//radio.vIntSrcCfg(INT_FIFO_WBYTE_RX, INT_PKT_DONE);
 	radio.bGoRx();
 	}
 uart.vUartInit(9600, _8N1);
 change = false;
 
 pinMode(KEY1, INPUT);  
 pinMode(KEY2, INPUT);
 pinMode(KEY3, INPUT);
 pinMode(KEY4, INPUT);
 
 Lcd.vDispInit();
 Buzz.vBuzzInit();
 Lcd.vDispLight(1);
 Lcd.vDispClear(); 
 _delay_ms(50);
 if(ActiveMode==1)
     Lcd.vDispString8x8(1, 1, disp0);
 else
     Lcd.vDispString8x8(1, 1, disp1);
  
 Lcd.vDispString8x8(2, 1, strTx);
 Lcd.vDispString8x8(5, 1, strRx);
     
 TxCount = 0;
 RxCount = 0;
  
}

void loop()
{
 byte tmp;
 byte i;
 byte j;
 byte noise;
 
 if(digitalRead(KEY4)==LOW)
     {
     RxCount = 0;
     TxCount = 0;
     }
 
 if(ActiveMode)
 	{
 	vInitRadio();
 	radio.vIntSrcCfg(INT_FIFO_NMTY_TX, INT_TX_DONE);   //INT2 for TxDone
	if(change) 
		{
		radio.bSendMessage(str1, LEN1);
		change = false;
		}
	else
		{
		radio.bSendMessage(str, LEN);
		change = true;
		}
	while(GPO3_L());	
	radio.bIntSrcFlagClr();
 	radio.vClearFIFO(); 
 	radio.bGoStandby();
 	radio.bReadStatus();

	radio.vIntSrcCfg(INT_FIFO_WBYTE_RX, INT_CRC_PASS);	
	//radio.vIntSrcCfg(INT_FIFO_WBYTE_RX, INT_PKT_DONE);
	radio.bGoRx();
	
	TxCount++;
	ShowCount(TxCount);
	Lcd.vDispString8x8(3, 1, strcnt);
    _delay_ms(10);
    noise = radio.bReadRssi(false);
    noise = noise/2;
    ShowNoise(noise);    
	Lcd.vDispString8x8(8, 1, strcnt);	
    
    for(i=0;i<100;i++)
         {
         delay(2);
         tmp = radio.bGetMessageByFlag(getstr);
         if(tmp!=0)
            {
            radio.bGoStandby();   
       		radio.bIntSrcFlagClr();
       		radio.vClearFIFO(); 
       		radio.bReadStatus();
            if(change)
            	{
                for(j=0;j<tmp;j++)  
                	{
                    if(getstr[j]!=str[j])
                    	break;
                    }
               	}
            else
               	{
                for(j=0;j<tmp;j++)  
                	{
                    if(getstr[j]!=str1[j])
                    	break;
                   	}
               	}      
           	if(j==tmp)
           		{
                RxCount++;
                ShowCount(RxCount);
                Lcd.vDispString8x8(6, 1, strcnt);
                ShowRssi(radio.PktRssi/2);
                Lcd.vDispString8x8(7, 1, strcnt);
                Buzz.vBuzzPlay(music, 1);
                uart.vUartPutNByte(getstr, tmp);
                uart.vUartNewLine();
                }
           	for(i=0;i<tmp;i++)
            	getstr[i] = 0x00; 
           	break;             
            }
		}
	
	radio.bGoSleep();   
    delay(1000);	
 	}
 else
 	{
 	tmp = radio.bGetMessageByFlag(getstr);
 	if(tmp!=0)
 		{
 		radio.bGoStandby();   
       	radio.bIntSrcFlagClr();
       	radio.vClearFIFO(); 
       	radio.bReadStatus();
 		
        ShowRssi(radio.PktRssi/2);
        Lcd.vDispString8x8(7, 1, strcnt);
        RxCount++;
        ShowCount(RxCount);
        Lcd.vDispString8x8(6, 1, strcnt); 

        Buzz.vBuzzPlay(music, 1);  
        uart.vUartPutNByte(getstr, tmp);
        uart.vUartNewLine();

        delay(100);
        vInitRadio();
        radio.vIntSrcCfg(INT_FIFO_NMTY_TX, INT_TX_DONE);   //INT2 for TxDone
        radio.bSendMessage(getstr, tmp);
		while(GPO3_L());	
		radio.bIntSrcFlagClr();
 		radio.vClearFIFO(); 
 		radio.bGoStandby();
 		radio.bReadStatus();        
        
        TxCount++;
        ShowCount(TxCount);
        Lcd.vDispString8x8(3, 1, strcnt);        
      
        for(i=0;i<tmp;i++)
            getstr[i] = 0x00;
        delay(100);
		
		radio.vIntSrcCfg(INT_FIFO_WBYTE_RX, INT_CRC_PASS);	
		//radio.vIntSrcCfg(INT_FIFO_WBYTE_RX, INT_PKT_DONE);        
        radio.bGoRx();
        delay(10);
        
        noise = radio.bReadRssi(false);
        noise = noise/2;
        ShowNoise(noise);
        Lcd.vDispString8x8(8, 1, strcnt); 		
 		
 		}
	}	
}
					
					
void vInitRadio(void)					
{
 radio.FixedPktLength    = false;				
 radio.PayloadLength     = LEN;	
 radio.vInit();
 radio.vCfgBank(CMTBank, 12);
 radio.vCfgBank(SystemBank, 12);
 radio.vCfgBank(FrequencyBank, 8);
 radio.vCfgBank(DataRateBank, 24);
 radio.vCfgBank(BasebandBank, 29);
 radio.vCfgBank(TXBank, 11);

 radio.vEnableAntSwitch(1);
 radio.vGpioFuncCfg(GPIO1_INT1+GPIO2_INT2+GPIO3_INT2+GPIO4_DOUT);
 //radio.vGpioFuncCfg(GPIO1_INT1+GPIO2_INT2+GPIO3_DOUT+GPIO4_DOUT);   //for test GPO3=Dout
 
 radio.vIntSrcEnable(PKT_DONE_EN+CRC_PASS_EN+PREAMBLE_PASS_EN+SYNC_PASS_EN+TX_DONE_EN); 
 radio.bIntSrcFlagClr();
 radio.vClearFIFO();
 radio.bGoSleep();
}

void ShowCount(word count)
{
 byte i=0;
 strcnt[i++] = '0'+(count/10000);
 strcnt[i++] = '0'+(count%10000/1000);
 strcnt[i++] = '0'+(count%1000/100);
 strcnt[i++] = '0'+(count%100/10);
 strcnt[i++] = '0'+(count%10);
 strcnt[i++] = '\0';
}

void ShowRssi(byte rs)
{
 byte i=0;
 strcnt[i++] = 'R';
 strcnt[i++] = 's';
 strcnt[i++] = 's';
 strcnt[i++] = 'i';
 strcnt[i++] = ':';
 strcnt[i++] = ' ';
 strcnt[i++] = '-';
 strcnt[i++] = '0'+(rs/100);
 strcnt[i++] = '0'+(rs%100/10);
 strcnt[i++] = '0'+(rs%10);
 strcnt[i++] = 'd';
 strcnt[i++] = 'B';
 strcnt[i++] = 'm';
 strcnt[i++] = '\0';
} 

void ShowNoise(byte noise)
{
 byte i=0;
 strcnt[i++] = 'N';
 strcnt[i++] = 'o';
 strcnt[i++] = 'i';
 strcnt[i++] = 's';
 strcnt[i++] = 'e';
 strcnt[i++] = ':';
 strcnt[i++] = '-';
 strcnt[i++] = '0'+(noise/100);
 strcnt[i++] = '0'+(noise%100/10);
 strcnt[i++] = '0'+(noise%10);
 strcnt[i++] = 'd';
 strcnt[i++] = 'B';
 strcnt[i++] = 'm';
 strcnt[i++] = '\0';
} 
