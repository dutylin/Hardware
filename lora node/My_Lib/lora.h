#ifndef _LORA_H
#define _LORA_H	
#include "stm8l15x.h"
#include "stm8_eval.h"

#include "stm8l15x.h"
u8 lora_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8* lora_check_cmd(u8 *str);
unsigned char  lora_init(void);

#endif


