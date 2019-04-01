#ifndef _USER_H_
#define _USER_H_

typedef struct 
{
 enum 
 {
  KEY1_BACK=1,
  KEY2_OK,
  KEY3_SCROLL,
 
  KEY_Press,
  KEY_Release,
  

 }Key_enum;

 uint8_t  Trg;
 uint8_t  Cont;
 uint8_t  Release;
 uint8_t  KeyReadPin;
}Key_EventsTypeDef;

typedef void (*pDisplay)(void);


extern void Key_Events_Scanning(Key_EventsTypeDef * KeyGT);
extern void TIM4_Config(void);
extern void Key_Task(void);
extern void page_update(void);

extern void diplay_aadr_1(void);
extern void diplay_aadr_2(void);
extern void diplay_aadr_3(void);
extern void diplay_aadr_4(void);
extern void diplay_aadr_5(void);
extern void diplay_aadr_6(void);
extern void diplay_aadr_7(void);
extern void diplay_aadr_8(void);
extern void diplay_aadr_9(void);
extern void diplay_aadr_10(void);
extern void diplay_aadr_11(void);
extern void diplay_aadr_12(void);
extern void diplay_aadr_13(void);
extern void diplay_aadr_14(void);
extern void diplay_aadr_15(void);
extern void diplay_aadr_16(void);
extern void diplay_aadr_17(void);
extern void diplay_aadr_18(void);
extern void diplay_aadr_19(void);

extern void display_clear(uint8_t x,uint8_t y,uint8_t pos);

#endif