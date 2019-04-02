#ifndef GPS_ANALISIS_H_
#define GPS_ANALISIS_H_
typedef struct
{
	unsigned char year_Hbyte;
	unsigned char year_Lbyte;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
} DATE_TIME;

/*typedef  struct{
    double latitude; //经度
    double longitude; //纬度
    int latitude_Degree;    //度
    int        latitude_Cent;   //分
    int     latitude_Second; //秒
    int longitude_Degree;    //度
    int        longitude_Cent;  //分
    int     longitude_Second; //秒
    float     speed; //速度
    float     direction; //航向
    float     height; //海拔高度
    int satellite;
    unsigned int len;
    unsigned char     NS;
    unsigned char     EW;
    DATE_TIME D;
}GPS_INFO;*/




typedef  struct
{
	//double latitude; //经度
	//double longitude; //纬度
	unsigned char latitude_Degree;    //度
	unsigned char        latitude_Cent;   //分
	unsigned char    latitude_Second; //秒
	unsigned char longitude_Degree;    //度
	unsigned char        longitude_Cent;  //分
	unsigned char     longitude_Second; //秒
	unsigned char     speed_Hbyte; //速度
	unsigned char     speed_Lbyte; //速度
	unsigned char     speed_Dex; //速度
         unsigned char    direction_Degree_H;    //度
        unsigned char    direction_Degree_L;    //度
	unsigned char    direction_Cent;   //分
	unsigned char    direction_Second; //秒	
	unsigned char     height_Hbyte; //海拔
	unsigned char     height_Lbyte; //海拔
	// float     height; //海拔高度
	unsigned char     satellite; //卫星
	unsigned char     len_Hbyte; //长度高字节
	unsigned char     len_Lbyte; //长度低字节
	unsigned char     NS;
	unsigned char     EW;
	DATE_TIME D;
}GPS_INFO;
int GPS_RMC_Parse(unsigned char *line, GPS_INFO *GPS,char gps_format);
#endif
