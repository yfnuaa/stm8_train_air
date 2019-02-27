/**
  ******************************************************************************
  * @file    : beep.h
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 

#ifndef __BEEP__H__
#define __BEEP__H__

#include "stm8s.h"
#define GPIO_BUZZER     0
extern volatile uint8_t g_beep_is_on;
extern volatile uint8_t g_beep_timer_counter;
extern volatile uint8_t g_beep_need_on;
#define POWER_ON_BEEP_ON_TIME    500       //ms

#define SHORT_PRESS_BEEP_ON_TIME    50       //ms

#define LONG_PRESS_BEEP_ON_TIME    100       //ms

void beep_derect_on(void);
void beep_derect_off(void);
#if GPIO_BUZZER
void beep_init(void);
#else
#define beep_init() /\
/
#endif
void beep_on_ms(uint16_t ms);

#endif