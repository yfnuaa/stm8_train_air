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
    e_pm25_level_1 = 70,
    e_pm25_level_2  = 100,
    e_pm25_level_3  = 400,
    e_pm25_level_4  = 500,
    e_pm25_level_5  = 700
}; 

#define PM25_ALARM_VALUE      250

#ifndef PM25
extern uint16_t g_adc1_pm25_ad_value;     //mv    --> Channel AIN3

extern uint8_t g_pm25_is_power_on;
extern uint8_t g_pm25_is_led_on;

extern uint8_t g_pm25_need_detect;

extern uint16_t g_pm25_dust_density_ug_m3;

extern uint8_t g_pm25_delay_480us_ok;
extern uint8_t g_pm25_delay_40us_ok;
#endif

void pm25_calculate_density(uint16_t mv);

void pm25_led_on(void);
void pm25_led_off(void);
void pm25_power_off(void);
void pm25_power_on(void);
void pm25_set_detect_begin(void);
void pm25_set_detect_end(void);
void pm25_detect_process(void);
void pm25_init(void);

#endif
