/**
  ******************************************************************************
  * @file    : touch_key.h
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 

#ifndef _TOUCH_1_H___
#define _TOUCH_1_H___
 
enum e_touch_key
{
    e_touch_key_plus = 0,
    e_touch_key_minus,
    e_touch_key_mode,
    e_touch_key_power,
    e_touch_key_all
};

#define LONG_PRESS_TIME_DEFINE 150 //means 1.5s

#ifndef TOUCH_KEY_C
extern volatile u16 g_touch_key_power_long_press_count;

#endif

void touch_key_power_long_press_handle(void);
void touch_key_Init(void);
void touch_key_DeInit(void);
#endif