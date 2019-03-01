#ifndef ___ADC_H___
#define ___ADC_H___
#include "stm8s.h"
#include "stm8s_adc1.h"

#define CONVERSIONMODE_SINGLE 
//#define CONVERSIONMODE_CONTINUOUS  
extern volatile uint16_t g_ad_value;

extern volatile BitStatus  g_adc_finished;
void ADC1_C3_Init(void);
void ADC1_C4_Init(void);
void adc1_reset(void);
#define ADC1_C4_stop  ADC1_DeInit 

#define ADC1_C3_stop  ADC1_DeInit
#endif