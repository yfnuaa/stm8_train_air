/**
  ******************************************************************************
  * @file    : touch_1.h
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

void touch_key_Init(void);

#endif