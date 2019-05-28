#ifndef UARTPARSE_H
#define UARTPARSE_H
typedef struct{
unsigned char head;
unsigned char payloadbuff[15];
unsigned char crc;


}CommandInfo_typedef;
#endif
