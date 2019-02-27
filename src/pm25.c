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

#define PM25_POWER_PORT         GPIOD
#define PM25_POWER_PIN          GPIO_PIN_1

#define PM25_LED_PORT           GPIOC       //Need Check
#define PM25_LED_PIN            GPIO_PIN_3

uint8_t g_pm25_is_power_on = 0;
uint8_t g_pm25_is_led_on = 0;

uint8_t g_pm25_need_detect = 0;

uint16_t g_pm25_dust_density = 0;

uint8_t g_pm25_delay_480us_ok = 0;
uint8_t g_pm25_delay_40us_ok = 0;

void pm25_led_off(void)
{
    PM25_LED_PORT->ODR |= (uint8_t)( PM25_LED_PIN);
    g_pm25_is_led_on = 1;
}


void pm25_led_on(void)
{
    PM25_LED_PORT->ODR &= (uint8_t)(~PM25_LED_PIN);
    g_pm25_is_led_on = 0;
}


void pm25_calculate_density(uint16_t mv)
{  
    /* 
    calcVoltage = voMeasured * (5.0 / 1024.0); //将模拟值转换为电压值
    g_pm25_dust_density =(uint16_t)( 0.17 * calcVoltage - 0.1); //将电压值转换为粉尘密度输出单位
    */
    g_pm25_dust_density = 0;
    
}

void pm25_power_off(void)
{
    PM25_POWER_PORT->ODR &= (uint8_t)(~PM25_POWER_PIN);
    g_pm25_is_power_on = 0;
}


void pm25_power_on(void)
{
    PM25_POWER_PORT->ODR |= (uint8_t)( PM25_POWER_PIN);
    g_pm25_is_power_on = 1;
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


void pm25_init(void)
{
    PM25_LED_PORT->DDR |= PM25_LED_PIN;    
    PM25_LED_PORT->CR1 |= PM25_LED_PIN;    
    PM25_LED_PORT->CR2 &=(u8)( ~(PM25_LED_PIN));  

    pm25_led_off();
}

