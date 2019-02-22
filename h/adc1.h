#ifndef ___ADC1_H___
#define ___ADC1_H___

extern uint16_t g_adc1_pm25_ad_value;     //mv    --> Channel AIN3
extern uint16_t g_adc1_co2_ad_value;      //mv    --> Channel AIN4    (MG812输出: 200mv~400mv, 这里放大 10 倍)
extern uint8_t  g_adc1_is_ok;

uint8_t adc1_start(void);
void adc1_init(void);


#endif