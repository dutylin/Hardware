#define __UTIL_CRC16L_C__
#include "hac_crc16l.h"
//#include "std_int.h"

//左移
//多项式=X16+X12+X5+X0
//CRC16L_POLY=0x1021
//1 0001 0000 0010 0001
//CRC16L_IN_INIT = 0x0000


#define CRC16L_IN_INIT           0x0000U
#define CRC16L_POLY              0x1021U
#define CRC16L_DIS_POLY          ((CRC16L_POLY>>1)|CRC16L_FIRST_BIT_MASK)

#define CRC16L_FIRST_BIT_MASK    (1U<<15)
#define CRC16L_LAST_BIT_MASK     (1U<<0)


const uint16  crc16l_const_table[] = 
{
    0x0000,0x1021,0x2042,0x3063,0x4084,0x50A5,0x60C6,0x70E7,0x8108,0x9129,0xA14A,0xB16B,0xC18C,0xD1AD,0xE1CE,0xF1EF,
};

const uint16  crc16l_dis_const_table[] = 
{
    0x0000,0x1102,0x2204,0x3306,0x4408,0x550A,0x660C,0x770E,0x8810,0x9912,0xAA14,0xBB16,0xCC18,0xDD1A,0xEE1C,0xFF1E,
};

//表格生成
/*
for(uint16 i=0;i<16;i++)
{
crc16l_const_table[i]=crc16l_update_nibble(0,i);
}

for(uint16 i=0;i<16;i++)
{
crc16l_dis_const_table[i]=crc16l_dis_update_nibble(i,0);
}
*/

/*
uint16 crc16l_update_nibble(uint16 crc_in,uint8 data)
{
uint8 i;

data &=0x0f;

crc_in ^= (((uint16)data)<<12);

for (i = 0; i < 4; i++)
{
if (crc_in & CRC16L_FIRST_BIT_MASK)
{
crc_in <<= 1;
crc_in ^=  CRC16L_POLY;
            }
	    else
{
crc_in <<= 1;
            }
	}

return crc_in;
}
*/

/*
uint16 crc16l_dis_update_nibble(uint16 crc_out,uint8 data)
{
uint8 i; 

data &=0x0f;

for(i=0;i<4;i++)
{        
if(crc_out & CRC16L_LAST_BIT_MASK)
{                               
crc_out>>=1;              
crc_out ^=CRC16L_DIS_POLY;
          }
         else
{
crc_out>>=1; 
         }
       }      
crc_out^=(data<<12);
return crc_out;
}
*/

uint16 crc16l_update_nibble(uint16 crc_in,uint8 data)
{ 	              
    crc_in = (crc_in << 4) ^ crc16l_const_table[(crc_in>>(16-4)) ^ data];            
    
    return crc_in;
}

uint16 crc16l_dis_update_nibble(uint16 crc_out,uint8 data)
{
    
    crc_out=(crc_out>>4) ^crc16l_dis_const_table[crc_out&0x0F] ^ (data<<(16-4));
    
    return crc_out;
}

uint16 crc16l_update_byte(uint16 crc_in,uint8 data)
{
    crc_in=crc16l_update_nibble(crc_in,data/16);
    crc_in=crc16l_update_nibble(crc_in,data%16);
    return crc_in;
}

uint16 crc16l_dis_update_byte(uint16 crc_out,uint8 data)
{
    crc_out=crc16l_dis_update_nibble(crc_out,data%16);
    crc_out=crc16l_dis_update_nibble(crc_out,data/16);
    
    return crc_out;
}

uint16 crc16l_update_word(uint16 crc_in, uint16 data)
{
    crc_in=crc16l_update_byte(crc_in,data/256);
    crc_in=crc16l_update_byte(crc_in,data%256);
    return crc_in;
}


uint16 crc16l_dis_update_word(uint16 crc_out,uint16 data)
{
    crc_out=crc16l_dis_update_byte(crc_out,data%256);
    crc_out=crc16l_dis_update_byte(crc_out,data/256);
    return crc_out;
}

uint16 crc16l_update_bit(uint16 crc_in,uint8 data)
{
    uint8 i;
    
    data &=0x01;
   
    crc_in ^= (data<<15);
    for (i = 0; i < 1; i++)
    {
        if (crc_in & CRC16L_FIRST_BIT_MASK)
        {
            crc_in <<= 1;
            crc_in ^=  CRC16L_POLY;
        }
        else
        {
            crc_in <<= 1;
        }
    }
    
    return crc_in;
}

uint16 crc16l_dis_update_bit(uint16 crc_out,uint8 data)
{
    uint8 i; 
    
    data &=0x01;
    
    for(i=0;i<1;i++)
    {        
        if(crc_out & CRC16L_LAST_BIT_MASK)
        {                               
            crc_out >>=1;             
            crc_out ^=CRC16L_DIS_POLY;
        }
        else
        {
            crc_out>>=1; 
        }
    }      
    crc_out^=(data<<15);
    
    return crc_out;
}

uint16 crc16l_calculate(uint16 crc_in,const uint8 *data,uint16 size)
{
    uint16 i;
    
    for (i = 0; i < size; i++)
    {
        crc_in = crc16l_update_byte(crc_in, data[i]);
    }
    
    return crc_in; 
}

uint16 crc16l_check(const uint8 *data,uint16 size)
{
    return crc16l_calculate(CRC16L_IN_INIT,data,size);
}
