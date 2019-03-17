/**
  ******************************************************************************
  * @file    : timer1.h
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 

#ifndef __TIME1_H__
#define __TIME1_H__

#ifndef TIMER1_C
extern volatile uint32_t g_system_10ms_tick; 
extern volatile uint32_t g_system_tick; 
#endif


void timer1_start(void);
void timer1_init(void);

#endif