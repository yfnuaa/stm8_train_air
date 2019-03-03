/**
  ******************************************************************************
  * @file    : PM25_1.c
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 

#include "stm8s.h"
#include "co2.h"
#include "adc1.h"
#include "debug.h"
#include "utility.h"
#include "lcd.h"

#define PM25_POWER_PORT         GPIOD
#define PM25_POWER_PIN          GPIO_PIN_1

#define PM25_LED_PORT           GPIOC       //Need Check
#define PM25_LED_PIN            GPIO_PIN_3
uint16_t g_adc1_pm25_ad_value;     //mv    --> Channel AIN3

uint8_t g_pm25_is_power_on = 0;
uint8_t g_pm25_is_led_on = 0;

uint8_t g_pm25_need_detect = 0;

uint16_t g_pm25_dust_density = 0;

uint8_t g_pm25_delay_480us_ok = 0;
uint8_t g_pm25_delay_40us_ok = 0;

void pm25_led_off(void)
{
    PM25_LED_PORT->ODR |= (uint8_t)( PM25_LED_PIN);
    g_pm25_is_led_on = 0;
}


void pm25_led_on(void)
{
    PM25_LED_PORT->ODR &= (uint8_t)(~PM25_LED_PIN);
    g_pm25_is_led_on = 1;
}
/*
X = M*C/22.4
C = 22.4*X/M
X = 44ug/m3
C = PPM
M = molecular weight  CO2 = 44
*/

void pm25_calculate_density(uint16_t mv)
{  
    /* 
    calcVoltage = voMeasured * (5.0 / 1024.0); //将模拟值转换为电压值
    g_pm25_dust_density =(uint16_t)( 0.17 * calcVoltage - 0.1); //将电压值转换为粉尘密度输出单位
    */
    //u16 calcVoltage =  g_adc1_pm25_ad_value * (5000 / 1023);
    u16 calcVoltage   =  (u16)(g_adc1_pm25_ad_value * 4.88759);      //unit  mv
    g_pm25_dust_density = (uint16_t)( 0.17 * calcVoltage - 0.1);   //将电压值转换为粉尘密度输出单位   //unit mg/m3
    //g_pm25_dust_density = (g_pm25_dust_density+(uint16_t)( 0.17 * calcVoltage - 100))>>1; //将电压值转换为粉尘密度输出单位     //unit  ug/m3
    print_u16("pm2.5 value", g_pm25_dust_density);
    lcd_display_pm25(g_pm25_dust_density);
}

void pm25_power_off(void)
{  
    PM25_POWER_PORT->ODR &= (uint8_t)(~PM25_POWER_PIN);
    g_pm25_is_power_on = 0;
    pm25_led_on();
}


void pm25_power_on(void)
{
    PM25_POWER_PORT->ODR |= (uint8_t)( PM25_POWER_PIN);
    g_pm25_is_power_on = 1;
    pm25_led_off();
}


void pm25_set_detect_begin(void)
{  
    //sensor vcc enable
    //pm25_power_off();

    //delay
    
    //sensor LED power
    //delay_280us();
    print("pm25_led_on");
    pm25_led_on();

    //start time4
}


void pm25_set_detect_end(void)
{  
    pm25_led_off();
    print("pm25_led_off");
}

//PD_DDR     CR1      CR2
//  0         0        0    float             input     pullup resistor is off
//  0         1        0    pullup            input     pullup resistor is on 
//  0         0        1    interrup float    input     pullup resistor is off 
//  0         1        1    interrup pullup   input     pullup resistor is on //  1         0        0    open-drain        output    pullup resistor is off 
//  1         1        0    push-pull         output    pullup resistor is off    
//  1         x        1    quick 10Mhz       output    pullup resistor is off 
void pm25_init(void)
{
    PM25_LED_PORT->DDR |= PM25_LED_PIN;    
    PM25_LED_PORT->CR1 |= PM25_LED_PIN;    
    PM25_LED_PORT->CR2 &=(u8)( ~(PM25_LED_PIN));  
    PM25_POWER_PORT->DDR |= PM25_POWER_PIN;    
    PM25_POWER_PORT->CR1 |= PM25_POWER_PIN;    
    PM25_POWER_PORT->CR2  &=(u8)( ~(PM25_POWER_PIN));  
    pm25_power_off();
    pm25_led_off();
}

