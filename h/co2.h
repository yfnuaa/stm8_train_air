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

#define CO2_WARM_UP_OK_TIME_2_MINUTES    12000     // (2min * 60 * 100)* 10 ms
#define CO2_WARM_UP_OK_TIME_5_MINUTES    30000     // (5min * 60 * 100)* 10 ms

#ifndef CO2_C
extern volatile uint16_t g_adc1_co2_ad_value; 
extern volatile uint8_t g_co2_is_power_on;
extern volatile uint8_t g_co2_is_warm_up;
extern volatile uint16_t g_co2_density_mg_m3;
extern volatile uint16_t g_co2_power_time_count;
#endif

void co2_power_on(void);
void co2_power_off(void);
void co2_calculate_density(uint16_t mv);
void co2_detect_process(void);
void co2_init(void);

#endif