/**
  ******************************************************************************
  * @file    : lcd.H
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 

#ifndef __LCD_H__
#define __LCD_H__

#include "stm8s.h"
#include "pwm.h"

void lcd_init(void);
void lcd_tone_off(void);
void lcd_tone_on(void);
void lcd_display_pm25( uint16_t pm25 );
void lcd_display_co2( uint16_t pm25 );
void lcd_display_fan_speed( enum pwm_motor_speed_step step );
void lcd_display_air_quality( uint16_t quality );
void lcd_display_power_on_mode(void);
void lcd_back_light_off(void);
void lcd_back_light_on(void);
void lcd_on(void);
void lcd_off(void);
void lcd_display_switch_on(void);
void lcd_light_mode_manual(void);
void ht1621_light_up_always_on_seg(void);
void lcd_light_mode_auto(void);

void lcd_light_mode_sleep(void);

#endif