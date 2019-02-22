/**
  ******************************************************************************
  * @file    : adc1.c
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 
  
#include "stm8s.h"

#define ADC_CONVERSION_SPEED            ADC1_PRESSEL_FCPU_D4

#define SYSTEM_ADC_RESOLUTION_RATIO     1023.0
#define VOLTAGE_REF                     5000.0  //mv
#define VOLTAGE_MG812_REF               500.0   //mv


uint16_t g_adc1_pm25_ad_value;     //mv    --> Channel AIN3
uint16_t g_adc1_co2_ad_value;      //mv    --> Channel AIN4    (MG812输出: 200mv~400mv, 这里放大 10 倍)
uint8_t  g_adc1_is_ok;

void adc1_isr(void)
{
    uint16_t temp = 0;
    uint8_t tempL = ADC1->DB3RL;
    uint8_t tempH = ADC1->DB3RH;

    //PM2.5
    tempL = ADC1->DB3RL;
    tempH = ADC1->DB3RH;
    temp = tempH<<8 + tempL;
    g_adc1_pm25_ad_value = (uint16_t)(VOLTAGE_REF*temp/SYSTEM_ADC_RESOLUTION_RATIO);
          
    //MG812
    tempL = ADC1->DB4RL;
    tempH = ADC1->DB4RH;
    temp = tempH<<8 + tempL;
    g_adc1_co2_ad_value = (uint16_t)(VOLTAGE_MG812_REF*temp/SYSTEM_ADC_RESOLUTION_RATIO);
    
    ADC1->CSR = ADC1_IT_EOCIE | ADC1_CHANNEL_4;

    g_adc1_is_ok = 1;
}


void adc1_start(void)
{
    g_adc1_is_ok = 0;
    ADC1->CR1 |= ADC1_CR1_ADON;
    
}


void adc1_init(void)
{
  GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT);
  
  ADC1->CSR = ADC1_IT_EOCIE | ADC1_CHANNEL_4;
  ADC1->CR1 = ADC_CONVERSION_SPEED | ADC1_CONVERSIONMODE_CONTINUOUS; //ADC1_CR1_CONT | 
  ADC1->CR2 = ADC1_ALIGN_RIGHT;
  
  ADC1->TDRL = ADC1_CHANNEL_3 | ADC1_CHANNEL_4;
  ADC1->CR2 |= ADC1_CR2_SCAN;
  ADC1->CR3 = ADC1_CR3_DBUF;
  
  ADC1->CR1 |= ADC1_CR1_ADON;
}


