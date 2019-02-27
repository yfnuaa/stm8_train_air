/**
  ******************************************************************************
  * @file    : time4.h
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 

#ifndef __TIME4_H__
#define __TIME4_H__

#include "stm8s.h"

enum time4_start_mode
{
    e_mode_280_us = 0,
    e_mode_40_us,
    e_mode_null
};

#ifndef TIMER4_C
extern volatile uint8_t g_time4_280us_ok;
extern volatile uint8_t g_time4_40us_ok;
extern enum time4_start_mode g_time4_start_mode;
#endif

void timer4_start_280us(void);    
void timer4_start_40us(void);    
void timer4_init(void);

#endif