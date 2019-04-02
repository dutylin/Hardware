#ifndef AT_CMD_H_
#define AT_CMD_H_

//const unsigned char Test2[]='\0';
const unsigned char Test[]="AT\r";
const unsigned char Respond_OK[]="OK\r\n";
const unsigned char Respond_Start[]="PB DONE\r\n";
const unsigned char Respond_ERROR[]="ERROR\r\n";
const unsigned char Respond_IP[]="+IPADDR: ";
const unsigned char Respond_Connection_ERROR[]="CLOSED";
const unsigned char Respond_Connection_OK[]="CONNECT 115200\r";
const unsigned char Respond_CPIN[]="+CPIN: READY\r\n";
const unsigned char Respond_Network_Intensity[]="+CSQ: 0,0\r";//检查网络信号强度
const unsigned char Respond_Network_Intensity2[]=",99\r";//检查网络信号强度
const unsigned char Respond_Network_Intensity_eff[]="+CSQ: ";//检查网络信号强度

const unsigned char Respond_Attached_Fail[]="+CGATT: 0";//
const unsigned char Respond_Network_Open[]="Network is already opened\r";//
const unsigned char Respond_TCP_Connect[]="CONNECT 115200\r";//
const unsigned char Respond_Network_Open2[]="+NETOPEN: 0";//
const unsigned char Respond_No_Carrier[]="+NO CARRIER";//




const unsigned char TIMEOUT[]="AT+CIPTIMEOUT?\r\n";
const unsigned char TO_DATA_MODE[]="ATO\r";//enter data transparent
const unsigned char Quit_transparent[]="+++";//quit from data transparent
const unsigned char TCP_ERROR[]="+CIPOPEN: 0,4\r\n";//
const unsigned char TCP_Closed[]="CLOSED\r\n";//
const unsigned char TCP_Connect[]="CONNECT\r\n";//
const unsigned char CPU_ID[]="Get CPU ID\r\n";//
const unsigned char Echo_Dis[]="ATE0\r";//回显关闭命令，并保存
const unsigned char Read_SIM_Num[]="AT+CICCID\r";
const unsigned char Check_SIM[]="AT+CPIN?\r";//检查登入移动设备(ME)的密码
const unsigned char Normal_Mode[]="AT+CFUN=1\r";
const unsigned char Mnimum_Mode[]="AT+CFUN=0\r";
const unsigned char Network_Intensity[]="AT+CSQ\r";//检查网络信号强度
const unsigned char GPRS_Register[]="AT+CGREG?\r";//检查 GPRS 注册状态
const unsigned char Network_Register[]="AT+CREG?\r";//检查网络注册状态
const unsigned char GPRS_Attached_State[]="AT+CGATT?\r";//检查 GPRS 附着状态
const unsigned char Sing_Link[]="AT+CIPMUX=0\r";//单链接模式
const unsigned char Pass_Through[]="AT+CIPMODE=1\r";//设置为透传模式
const unsigned char Net_Open[]="AT+NETOPEN\r";//设置为透传模式
const unsigned char Net_Close[]="AT+NETCLOSE\r";//设置为透传模式
const unsigned char Reversion[]="AT+GMR\r";
const unsigned char CIP_Close[]="AT+CIPCLOSE=0\r";//设置为透传模式

const unsigned char APN[]="AT+CGDCONT=1,\"IP\",\"jsczaiot02.clfu.njm2mapn\"\r";//查询运营商选择状态
//const unsigned char Establish_TCP_Connection[]="AT+CIPSTART=\"TCP\",\"15k801n729.iok.la\",25718\r";
 
//创建连接\"TCP\",\"15k801n729.iok.la\",25718\r";
//\"TCP\",\"139.199.223.136\",12345\r";
const unsigned char Airplane_Mode[]="AT+CFUN=0\r";//飞行模式，关闭GPRS
const unsigned char China_Mobile[]="AT+COPS?\r";//查询运营商选择状态
const unsigned char AT_SHUNT[]="AT+CIPSHUT\r";//查询运营商选择状态
const unsigned char FLY_Mode[]="AT+CFUN=0\r";
const unsigned char M_RESET[]="AT+CRESET\r";

//**********************************GPS Infor********************************************

const unsigned char Start_GPS[]="AT+CGPSAUTO=1\r";//start gps
const unsigned char Stop_GPS[]="AT+CGPS=0\r";
const unsigned char AT_GPS_info[]="AT+CGPSINFO\r";

#endif
