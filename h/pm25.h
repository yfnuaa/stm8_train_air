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
enum pm25_dusty_servrity
{
    e_pm25_good = 140,
    e_pm25_mid  = 380,
    e_pm25_bad  = 580
};
#define PM25_ALARM_VALUE  	250
extern uint16_t g_adc1_pm25_ad_value;     //mv    --> Channel AIN3

extern uint8_t g_pm25_is_power_on;
extern uint8_t g_pm25_is_led_on;

extern uint8_t g_pm25_need_detect;

extern uint16_t g_pm25_dust_density;

extern uint8_t g_pm25_delay_480us_ok;
extern uint8_t g_pm25_delay_40us_ok;

void pm25_calculate_density(uint16_t mv);

void pm25_led_on(void);
void pm25_led_off(void);
void pm25_power_off(void);
void pm25_power_on(void);
void pm25_set_detect_begin(void);
void pm25_set_detect_end(void);
void pm25_init(void);

#endif
