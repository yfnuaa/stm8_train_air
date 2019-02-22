/**
  ******************************************************************************
  * @file    : pm25.h
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 

#ifndef  _PM25_H___
#define  _PM25_H___

#include "stm8s.h"

#define PM25_ALARM_VALUE  	250

extern uint8_t g_pm25_is_power_on;
extern uint8_t g_pm25_is_led_on;

extern uint8_t g_pm25_need_detect;

extern uint16_t g_pm25_dust_density;

void pm25_calculate_density(uint16_t mv);

void pm25_power_off(void);
void pm25_power_on(void);
void pm25_set_detect_begin(void);
void pm25_set_detect_end(void);
void pm25_init(void);

#endif
