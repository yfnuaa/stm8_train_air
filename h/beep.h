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

#define POWER_ON_BEEP_ON_TIME    	300       //ms
#define SHORT_PRESS_BEEP_ON_TIME    30       //ms
#define LONG_PRESS_BEEP_ON_TIME    	100       //ms

#ifndef BEEP_C
extern volatile uint8_t g_beep_is_on;
extern volatile uint8_t g_beep_timer_counter;
extern volatile uint8_t g_beep_need_on;
extern volatile uint8_t g_beep_alarm_count;
extern volatile uint8_t g_need_beep_alarm;       //需要开启 beep 报警,用于 启动 beep
#endif

void beep_derect_on(void);
void beep_derect_off(void);

void beep_on_ms(uint16_t ms);
void beep_derect_off(void);
void beep_init(void);

#endif