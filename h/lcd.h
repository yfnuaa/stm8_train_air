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
void lcd_display_fan_speed( u8 step );
void lcd_display_air_quality( uint16_t quality );


#endif