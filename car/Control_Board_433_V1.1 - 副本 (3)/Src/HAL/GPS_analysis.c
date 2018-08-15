#include "stm32f4xx_hal.h"
#include "GPS_analysis.h"
#include "BSP.h"
#include "string.h"
//GPS_INFO  gps_info;

int GetComma(int num,unsigned char *str)
{
	int i,j=0,k=0;
	int len=strlen((const char *)str);
	for(i=0; i<len; i++)
	{
		if(str[i]!=',')
		{
			j++;
		}
		else
			k++;

		if(k==num)
			return i+1;
	}
	return 0;
}
int GetColon(int num,char *str)
{
	int i,j=0,k=0;
	int len=strlen(str);
	for(i=0; i<len; i++)
	{
		if(str[i]!=':')
		{
			j++;
		}
		else
			k++;

		if(k==num)
			return i+1;
	}
	return 0;
}

//====================================================================//
// 语法格式: static float Str_To_Float(char *buf)
// 实现功能： 把一个字符串转化成浮点数
// 参    数：字符串
// 返 回 值：转化后单精度值
//====================================================================//
static float Str_To_Float(char *buf)
{
	float rev = 0;
	float dat;
	int integer = 1;
	char *str = buf;
	int i;
	while(*str != '\0')
	{
		switch(*str)
		{
		case '0':
			dat = 0;
			break;
		case '1':
			dat = 1;
			break;
		case '2':
			dat = 2;
			break;
		case '3':
			dat = 3;
			break;
		case '4':
			dat = 4;
			break;
		case '5':
			dat = 5;
			break;
		case '6':
			dat = 6;
			break;
		case '7':
			dat = 7;
			break;
		case '8':
			dat = 8;
			break;
		case '9':
			dat = 9;
			break;
		case '.':
			dat = '.';
			break;
		}
		if(dat == '.')
		{
			integer = 0;
			i = 1;
			str ++;
			continue;
		}
		if( integer == 1 )
		{
			rev = rev * 10 + dat;
		}
		else
		{
			rev = rev + dat / (10 * i);
			i = i * 10 ;
		}
		str ++;
	}
	return rev;

}
//====================================================================//
// 语法格式: static double Str_To_Double(char *buf)
// 实现功能： 把一个字符串转化成浮点数
// 参    数：字符串
// 返 回 值：转化后双精度值
//====================================================================//
static double Str_To_Double(char *buf)
{
	float rev = 0;
	float dat;
	int integer = 1;
	char *str = buf;
	int i;
	while(*str != '\0')
	{
		switch(*str)
		{
		case '0':
			dat = 0;
			break;
		case '1':
			dat = 1;
			break;
		case '2':
			dat = 2;
			break;
		case '3':
			dat = 3;
			break;
		case '4':
			dat = 4;
			break;
		case '5':
			dat = 5;
			break;
		case '6':
			dat = 6;
			break;
		case '7':
			dat = 7;
			break;
		case '8':
			dat = 8;
			break;
		case '9':
			dat = 9;
			break;
		case '.':
			dat = '.';
			break;
		}
		if(dat == '.')
		{
			integer = 0;
			i = 1;
			str ++;
			continue;
		}
		if( integer == 1 )
		{
			rev = rev * 10 + dat;
		}
		else
		{
			rev = rev + dat / (10 * i);
			i = i * 10 ;
		}
		str ++;
	}
	return rev;
}
//====================================================================//
// 语法格式: static float Get_Float_Number(char *s)
// 实现功能： 把给定字符串第一个逗号之前的字符转化成单精度型
// 参    数：字符串
// 返 回 值：转化后单精度值
//====================================================================//
static float Get_Float_Number(char *s)
{
	char buf[10];
	unsigned char i;
	float rev;
	i=GetComma(1, (unsigned char *)s);
	i = i - 1;
	strncpy(buf, s, i);
	buf[i] = 0;
	rev=Str_To_Float(buf);
	return rev;
}
static float Get_Float_Number2(char *s)
{
	char buf[10];
//	unsigned char i;
	float rev;
//    i=GetComma(1, (unsigned char *)s);
//	if(i>0)
//    i = i - 1;
	strncpy(buf, s, 1);
	// buf[i] = 0;
	rev=Str_To_Float(buf);
	return rev;
}
//====================================================================//
// 语法格式: static double Get_Double_Number(char *s)
// 实现功能：把给定字符串第一个逗号之前的字符转化成双精度型
// 参    数：字符串
// 返 回 值：转化后双精度值
//====================================================================//
static double Get_Double_Number(unsigned char *s)
{
	char buf[10];
	unsigned char i;
	double rev;
	i=GetComma(1, s);
	i = i - 1;
	strncpy(buf, (const char *)s, i);
	buf[i] = 0;
	rev=Str_To_Double(buf);
	return rev;
}
//====================================================================//
// 语法格式：void UTC2BTC(DATE_TIME *GPS)
// 实现功能：转化时间为北京时区的时间
// 参    数：存放时间的结构体
// 返 回 值：无
//====================================================================//
static void UTC2BTC(DATE_TIME *GPS)
{
	unsigned int tmp;
	GPS->second ++;
	if(GPS->second > 59)
	{
		GPS->second = 0;
		GPS->minute ++;
		if(GPS->minute > 59)
		{
			GPS->minute = 0;
			GPS->hour ++;
		}
	}

	GPS->hour = GPS->hour + 8;
	if(GPS->hour > 23)
	{
		GPS->hour -= 24;
		GPS->day += 1;
		if(GPS->month == 2 ||
		        GPS->month == 4 ||
		        GPS->month == 6 ||
		        GPS->month == 9 ||
		        GPS->month == 11 )
		{
			if(GPS->day > 30)
			{
				GPS->day = 1;
				GPS->month++;
			}
		}
		else
		{
			if(GPS->day > 31)
			{
				GPS->day = 1;
				GPS->month ++;
			}
		}
		if((GPS->year_Hbyte*256+GPS->year_Lbyte) % 4 == 0 )
		{
			if(GPS->day > 29 && GPS->month == 2)
			{
				GPS->day = 1;
				GPS->month ++;
			}
		}
		else
		{
			if(GPS->day > 28 &&GPS->month == 2)
			{
				GPS->day = 1;
				GPS->month ++;
			}
		}
		if(GPS->month > 12)
		{
			GPS->month -= 12;
			tmp=GPS->year_Hbyte*256+GPS->year_Lbyte;
			tmp++;
			GPS->year_Hbyte=tmp/256;
			GPS->year_Lbyte=tmp%256;
		}
	}
}

unsigned char *buf ;
int GPS_RMC_Parse(unsigned char *line, GPS_INFO *GPS,char gps_format)
{
	unsigned char ch, status, tmp;
	float lati_cent_tmp, lati_second_tmp;
	float long_cent_tmp, long_second_tmp;
	float speed_tmp,tmp2;
	double latitude,longitude,speed,direction;
	unsigned int year;
	buf = line;
#ifdef debug_mode
	unsigned char gps_buff[]="+CGPSINFO:3113.343286,N,12121.234064,E,250311,072809.3,44.1,0.1,2";
	buf = gps_buff;
#endif

	if(gps_format==0)
	{
		ch = buf[5];
		status = buf[GetComma(2, buf)];
		if (ch == 'C') //如果第五个字符是C，($GPRMC)
		{
			if (status == 'A') //如果数据有效，则分析
			{
				GPS->NS = buf[GetComma(4, buf)];
				GPS->EW = buf[GetComma(6, buf)];

				latitude = Get_Double_Number(&buf[GetComma(3, buf)]);
				longitude = Get_Double_Number(&buf[GetComma(5, buf)]);

				GPS->latitude_Degree = (int)latitude / 100; //分离纬度
				lati_cent_tmp = (latitude - GPS->latitude_Degree * 100);
				GPS->latitude_Cent = (int)lati_cent_tmp;
				lati_second_tmp = (lati_cent_tmp - GPS->latitude_Cent) * 60;
				GPS->latitude_Second = (int)lati_second_tmp;

				GPS->longitude_Degree = (int)longitude / 100;    //分离经度
				long_cent_tmp = (longitude - GPS->longitude_Degree * 100);
				GPS->longitude_Cent = (int)long_cent_tmp;
				long_second_tmp = (long_cent_tmp - GPS->longitude_Cent) * 60;
				GPS->longitude_Second = (int)long_second_tmp;

				speed_tmp = Get_Float_Number(&buf[GetComma(7, buf)]); //速度(单位：海里/时)
				speed = speed_tmp * 1.85; //1海里=1.85公里
				speed=speed*100;
				GPS->speed_Hbyte=(unsigned char)((speed/100)/256);
				GPS->speed_Lbyte=(unsigned char)((long int)(speed/100)%256);
				GPS->speed_Dex=(unsigned char)(((long int)speed%100));

				direction = Get_Float_Number(&buf[GetComma(8, buf)]); //角度
				
				GPS->direction_Degree_H=(unsigned char)(direction/256);//度高字节
				tmp2=((long int)direction)%256;
				GPS->direction_Degree_L=(unsigned char)(tmp2);//度高字节
				tmp2=(long int)(direction*100)%100;
				tmp2=(float)tmp2/100;
				GPS->direction_Cent=(unsigned char)(tmp2*60);
				tmp2=(long int)((float)tmp2*60*100)%100;
				tmp2=tmp2/100.0;
				GPS->direction_Second=(unsigned char)(tmp2*60);

				GPS->D.hour = (buf[7] - '0') * 10 + (buf[8] - '0');        //时间
				GPS->D.minute = (buf[9] - '0') * 10 + (buf[10] - '0');
				GPS->D.second = (buf[11] - '0') * 10 + (buf[12] - '0');
				tmp = GetComma(9, buf);
				GPS->D.day = (buf[tmp + 0] - '0') * 10 + (buf[tmp + 1] - '0'); //日期
				GPS->D.month = (buf[tmp + 2] - '0') * 10 + (buf[tmp + 3] - '0');
				year = (buf[tmp + 4] - '0') * 10 + (buf[tmp + 5] - '0') + 2000;

				UTC2BTC(&GPS->D);
				GPS->len_Hbyte=(unsigned char)(sizeof(*GPS)/256);
				GPS->len_Lbyte=(unsigned char)(sizeof(*GPS)%256);			

				return 1;
			}
		}
	}
	else
	{
		//status = buf[GetComma(2, buf)];
		 //if (buf[3]=='C'&&buf[11]==':') //如果第9个字符是:，(+CGPSINFO:)

		if (buf[0]=='+'&&buf[9]==':') //如果第9个字符是:，(+CGPSINFO:)
		{
			// if (status == 'A') //如果数据有效，则分析
			//+CGPSINFO:3113.343286,N,12121.234064,E,250311,072809.3,44.1,0.0,0
			{
				tmp=GetComma(1, buf);
				GPS->len_Hbyte=0;
				GPS->len_Lbyte=0;
				
			
				GPS->NS= buf[tmp];
			        if((GPS->NS)!='N'&&(GPS->NS)!='S')
						return 0;
				tmp=GetComma(2, buf);
				longitude = Get_Double_Number(&buf[tmp]);
				tmp=GetComma(3, buf);
				GPS->EW = buf[tmp];
				
				tmp=GetColon(1, buf);
				latitude = Get_Double_Number(&buf[tmp]);
				//tmp=GetComma(2, buf);
				//  GPS->longitude = Get_Double_Number(&buf[GetComma(2, buf)]);

				GPS->latitude_Degree = (int)latitude / 100; //分离纬度
				lati_cent_tmp = (latitude - GPS->latitude_Degree * 100);
				GPS->latitude_Cent = (int)lati_cent_tmp;
				lati_second_tmp = (lati_cent_tmp - GPS->latitude_Cent) * 60;
				GPS->latitude_Second = (int)lati_second_tmp;

				GPS->longitude_Degree = (int)longitude / 100;    //分离经度
				long_cent_tmp = (longitude - GPS->longitude_Degree * 100);
				GPS->longitude_Cent = (int)long_cent_tmp;
				long_second_tmp = (long_cent_tmp - GPS->longitude_Cent) * 60;
				GPS->longitude_Second = (int)long_second_tmp;

				
				tmp = GetComma(7, buf);
				speed_tmp = Get_Float_Number(&buf[tmp]); //速度(单位：海里/时)
				speed_tmp = speed_tmp * 1.85; //1海里=1.85公里
				speed=speed_tmp*100;			
				GPS->speed_Hbyte=(unsigned char)((speed/100)/256);
				GPS->speed_Lbyte=(unsigned char)((long int)(speed/100)%256);
				GPS->speed_Dex=(unsigned char)((long int)speed%100);

				
				tmp = GetComma(8, buf);
				tmp2=buf[tmp];
				direction = Get_Float_Number2(&buf[tmp]); //角度
				GPS->direction_Degree_H=(unsigned char)(direction/256);//度高字节
				tmp2=((long int)direction)%256;
				GPS->direction_Degree_L=(unsigned char)(tmp2);//度高字节
				tmp2=(long int)(direction*100)%100;
				tmp2=(float)tmp2/100;
				GPS->direction_Cent=(unsigned char)(tmp2*60);
				tmp2=(long int)((float)tmp2*60*100)%100;
				tmp2=tmp2/100.0;
				GPS->direction_Second=(unsigned char)(tmp2*60);


				tmp = GetComma(5, buf);
				GPS->D.hour = (buf[tmp + 0] - '0') * 10 + (buf[tmp + 1] - '0');        //时间
				GPS->D.minute = (buf[tmp + 2] - '0') * 10 + (buf[tmp + 3] - '0');
				GPS->D.second = (buf[tmp + 4] - '0') * 10 + (buf[tmp + 5] - '0');
				
				tmp = GetComma(4, buf);
				GPS->D.day = (buf[tmp + 0] - '0') * 10 + (buf[tmp + 1] - '0'); //日期
				GPS->D.month = (buf[tmp + 2] - '0') * 10 + (buf[tmp + 3] - '0');
				year = (buf[tmp + 4] - '0') * 10 + (buf[tmp + 5] - '0') + 2000;
				GPS->D.year_Hbyte=(unsigned char)(year/256);
				GPS->D.year_Lbyte=(unsigned char)(year%256);

				UTC2BTC(&GPS->D);
				GPS->len_Hbyte=(unsigned char)(sizeof(*GPS)/256);
				GPS->len_Lbyte=(unsigned char)(sizeof(*GPS)%256);
				return 1;
			}
		}

	}

	return 0;
}
