/**
  ******************************************************************************
  * @file    : co2.h
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 

#ifndef __CO2_1_H___
#define __CO2_1_H___

#include "stm8s.h"

#define CO2_WARM_UP_TIME    500     // * 10 ms

#define CO2_ALARM_VALUE       300
extern uint16_t g_adc1_co2_ad_value; 
extern uint8_t g_co2_is_power_on;
extern uint8_t g_co2_is_warm_up;
extern uint8_t g_co2_need_detect;
extern uint16_t g_co2_density;


void co2_power_on(void);
void co2_power_off(void);
void co2_calculate_density(uint16_t mv);
void co2_init(void);

#endif