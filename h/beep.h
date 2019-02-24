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

extern volatile uint8_t g_beep_is_on;
extern volatile uint8_t g_beep_timer_counter;
extern volatile uint8_t g_beep_need_on;

void beep_derect_on(void);
void beep_derect_off(void);
void beep_init(void);
void beep_on_ms(uint16_t ms);

#endif