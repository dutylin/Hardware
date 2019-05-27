#include "common.h"
#include <string.h>

//#include "gpio_defs.h"
//#include "stm8l15x.h"


//void set_u16_to_buf(u8 buf[], u16 dat16)
//{
//    buf[0] = (u8)dat16;
//    buf[1] = (u8)(dat16 >> 8);
//}
//
//u16 get_u16_from_buf(const u8 buf[])
//{
//    u16 dat16 = 0;
//    dat16  = buf[0];
//    dat16 |= ((u16)buf[1]) << 8;
//    return dat16;
//}
//
//void set_u32_to_buf(u8 buf[], u32 dat32)
//{
//    buf[0] = (u8)dat32;
//    buf[1] = (u8)(dat32 >> 8);
//    buf[2] = (u8)(dat32 >> 16);
//    buf[3] = (u8)(dat32 >> 24);
//}
//
//u32 get_u32_from_buf(const u8 buf[])
//{
//    u32 dat32 = 0;
//    dat32  = buf[0];
//    dat32 |= ((u32)buf[1]) << 8;
//    dat32 |= ((u32)buf[2]) << 16;
//    dat32 |= ((u32)buf[3]) << 24;
//    return dat32;
//}

void views_print_line(u8 nLine, const char* str)
{
  //static u8 buf[32];
  
  // memset(buf, ' ', sizeof(buf));
  // memcpy(buf, str, strlen(str));
  //  buf[21] = 0;
  
  //lcd12864_update_data(0);
  //lcd12864_display_string_6x8(nLine, 0, buf);
  //lcd12864_update_data(1);
}

