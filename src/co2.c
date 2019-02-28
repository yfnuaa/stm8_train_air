/**
  ******************************************************************************
  * @file    : co2.c
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 
#include "stm8s.h"
#include "debug.h"

#define CO2_GPIO_PORT       GPIOB
#define CO2_GPIO_PIN        GPIO_PIN_4
uint16_t g_adc1_co2_ad_value =0 ;      //mv    --> Channel AIN4    (MG812输出: 200mv~400mv, 这里放大 10 倍)

uint8_t g_co2_is_power_on = 0;
uint8_t g_co2_is_warm_up = 0;
uint8_t g_co2_need_detect = 0;

uint16_t g_co2_density = 0;

void co2_calculate_density(uint16_t mv)
{  
    /* 
    calcVoltage = voMeasured * (5.0 / 1024.0); //灏嗘ā鎷熷�艰浆鎹负鐢靛帇鍊�
    g_pm25_dust_density =(uint16_t)( 0.17 * calcVoltage - 0.1); //灏嗙數鍘嬪�艰浆鎹负绮夊皹瀵嗗害杈撳嚭鍗曚綅
    */
    g_co2_density = 0;
    
}

void co2_power_on()
{
    CO2_GPIO_PORT->ODR |= (uint8_t)( CO2_GPIO_PIN);
    g_co2_is_power_on = 1;
}


void co2_power_off()
{
    CO2_GPIO_PORT->ODR &= (uint8_t)(~CO2_GPIO_PIN);
    g_co2_is_power_on = 0;
    g_co2_is_warm_up = 0;
}
//PD_DDR     CR1      CR2
//  0         0        0    float             input     pullup resistor is off
//  0         1        0    pullup            input     pullup resistor is on 
//  0         0        1    interrup float    input     pullup resistor is off 
//  0         1        1    interrup pullup   input     pullup resistor is on //  1         0        0    open-drain        output    pullup resistor is off 
//  1         1        0    push-pull         output    pullup resistor is off    
//  1         x        1    quick 10Mhz       output    pullup resistor is off 
void co2_init()
{
    CO2_GPIO_PORT->DDR |= CO2_GPIO_PIN;    
    CO2_GPIO_PORT->CR1 |= CO2_GPIO_PIN;    
    CO2_GPIO_PORT->CR2  &=(u8)( ~(CO2_GPIO_PIN));  
}