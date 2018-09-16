#ifndef __LORA_UI_H__	
#define __LORA_UI_H__	 
#include "sys.h"
/************************************************
 ALIENTEK 阿波罗STM32F429开发板
 ATK-LORA-01模块UI和底层处理驱动
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com  
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

void lora_at_response(u8 mode);	
u8* lora_check_cmd(u8 *str);
u8 lora_send_cmd(u8 *cmd,u8 *ack,u16 waittime);

void Menu_ui(void);
void Menu_cfg(u8 num);
void Process_ui(void);
void Dire_Set(void);


#endif


