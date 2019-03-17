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
#include "adc.h"
#include "debug.h"
#include "utility.h"
#include "lcd.h"

#define PM25_POWER_PORT         GPIOD
#define PM25_POWER_PIN          GPIO_PIN_4

#define PM25_LED_PORT           GPIOC       //Need Check
#define PM25_LED_PIN            GPIO_PIN_3
uint16_t g_adc1_pm25_ad_value;     //mv    --> Channel AIN3

uint8_t g_pm25_is_power_on = 0;
uint8_t g_pm25_is_led_on = 0;

uint8_t g_pm25_need_detect = 0;

uint16_t g_pm25_dust_density_ug_m3 = 0;   /* 2 ~ 35 ug/m3 */

 

void pm25_led_off(void)
{
    PM25_LED_PORT->ODR |= (uint8_t)( PM25_LED_PIN);
 
}


void pm25_led_on(void)
{
    PM25_LED_PORT->ODR &= (uint8_t)(~PM25_LED_PIN);
  
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
    float calcVoltage;

//lcd_display_pm25(mv);
    /* 
    calcVoltage = voMeasured * (5.0 / 1024.0); //??????????????
    g_pm25_dust_density_ug_m3 =(uint16_t)( 0.17 * calcVoltage - 0.1); //??????????????????????
    */
    //u16 calcVoltage =  g_adc1_pm25_ad_value * (5000 / 1023);
    calcVoltage =  g_adc1_pm25_ad_value;
    print_u16("pm25 ad mv ======================= ", g_adc1_pm25_ad_value);
    calcVoltage   =  calcVoltage *   4.88759;      //unit  mv
    calcVoltage = 0.17 * calcVoltage ;
    calcVoltage = calcVoltage - 0.1;
    
    g_pm25_dust_density_ug_m3 = (uint16_t)(calcVoltage);   //??????????????????????   //unit ug/m3

    //g_pm25_dust_density_ug_m3 = (g_pm25_dust_density_ug_m3+(uint16_t)( 0.17 * calcVoltage - 100))>>1; //??????????????????????     //unit  ug/m3

    print_u16("pm25 ug/m3 ======================= ", g_pm25_dust_density_ug_m3);
    //lcd_display_pm25(g_pm25_dust_density_ug_m3);
}

void pm25_power_off(void)
{print("pm25 powen off");
    PM25_POWER_PORT->ODR &= (uint8_t)(~PM25_POWER_PIN);
    //g_pm25_is_power_on = 0;
    pm25_led_on();
}


void pm25_power_on(void)
{  print("pm25 powen on");
    PM25_POWER_PORT->ODR |= (uint8_t)( PM25_POWER_PIN);
    // g_pm25_is_power_on = 1;pm25_led_off();
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


//TODO ??????????????????
void pm25_detect_process(void)
{
    //begint PM25-------------
    ADC1_C3_Init();   
    pm25_led_on(); 
    delay_280us(); 

    ADC1_StartConversion();
    #if 1
 
    while(!ADC1_GetFlagStatus(ADC1_FLAG_EOC)){nop();nop();nop();nop();};
  
      ADC1_ClearFlag(ADC1_FLAG_EOC);

    g_adc1_pm25_ad_value = ADC1_GetConversionValue(); 
    #else
       while( RESET == g_adc_finished){ nop();}//wait adc finished          
       g_adc1_pm25_ad_value = g_ad_value;
    #endif 
    
    delay_40us();
    pm25_led_off();
    
    //lcd_display_pm25(g_adc1_pm25_ad_value);
    pm25_calculate_density(g_adc1_pm25_ad_value);
    
    delay_ms(9);
}


//PD_DDR     CR1      CR2
//  0         0        0    float             input     pullup resistor is off
//  0         1        0    pullup            input     pullup resistor is on 
//  0         0        1    interrup float    input     pullup resistor is off 
//  0         1        1    interrup pullup   input     pullup resistor is on 
//  1         0        0    open-drain        output    pullup resistor is off 
//  1         1        0    push-pull         output    pullup resistor is off    
//  1         x        1    quick 10Mhz       output    pullup resistor is off 
void pm25_init(void)
{
    PM25_LED_PORT->DDR |= PM25_LED_PIN;    
    PM25_LED_PORT->CR1 &=(u8)( ~(PM25_LED_PIN));  //|= PM25_LED_PIN;    
    PM25_LED_PORT->CR2 &=(u8)( ~(PM25_LED_PIN));  
    
    PM25_POWER_PORT->DDR |= PM25_POWER_PIN;    
    PM25_POWER_PORT->CR1 |= PM25_POWER_PIN;    
    PM25_POWER_PORT->CR2  &=(u8)( ~(PM25_POWER_PIN));  
    pm25_power_off();
    pm25_led_on();
}

