/**
  ******************************************************************************
  * @file    : co2.c
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 
#define  CO2_C  
#include "stm8s.h"
#include "debug.h"
#include "lcd.h"
#include "adc.h"


#define CO2_GPIO_PORT       GPIOB
#define CO2_GPIO_PIN        GPIO_PIN_4


uint16_t volatile g_adc1_co2_ad_value =0 ;      //mv    --> Channel AIN4    (MG812输出: 200mv~400mv, 这里放大 10 倍)

uint8_t volatile g_co2_is_power_on = 0;
uint8_t volatile g_co2_is_warm_up = 0;

volatile uint16_t g_co2_power_time_count = 0;

volatile uint16_t g_co2_density_mg_m3 = 0;     /* 0 ~ 900 mg/m3 */
/*
X = M*ppm/22.4
ppm = 22.4*X/M
X = 44mg/m3
 
M = molecular weight  CO2 = 44

ppm=（22.4×mg/m3）/m   分子量
*/


/*
340mv - 285mv 
ppm = (mv - 284)*();
284 - 257mv

256mv - 0


*/
u16 g_cal_zero_value_mv = 345;
void co2_calculate_density(uint16_t mv)
{  
    float co2ppm;
    float calc_raw_Voltage_mv;// = calc_raw_Voltage; 
    calc_raw_Voltage_mv =  mv;

    //u16 calcVoltage =  g_adc1_co2_ad_value * (5000  / 1023)/11;             // unit mv
    //  11 bei 0.444
    //  6 bei  0.814
    calc_raw_Voltage_mv = calc_raw_Voltage_mv *0.815;       // unit mv
    g_adc1_co2_ad_value= (u16)(calc_raw_Voltage_mv);

    print_u16("co2 ad mv ==================== ", g_adc1_co2_ad_value);
     
    /////////////////calc c02ppm/////////////////
    if(calc_raw_Voltage_mv >= g_cal_zero_value_mv)
    {
        co2ppm = 0;
    }
    else if(calc_raw_Voltage_mv <= g_cal_zero_value_mv - 85)   //260 = 345- 85
    {
        co2ppm = 507; //999/1.9643
    }
    else
    {
        co2ppm = (g_cal_zero_value_mv - calc_raw_Voltage_mv)*10;//  10 = 600/(345-285)
    }
    
   // print_u16("co2 value ppm[", co2ppm);print("]");
    if(g_co2_density_mg_m3>5)
    {   //g_co2_density_mg_m3 =(u16)( 44* co2ppm / 22.4);
        co2ppm = 1.9643 * co2ppm;
        g_co2_density_mg_m3 = (g_co2_density_mg_m3+(u16)(co2ppm))>>1;               //unit  mg/m3
    }
    else
    {
        g_co2_density_mg_m3 = (u16)(1.9643 * co2ppm);
    }
    
    print_u16("co2 value mg/m3  ================ ", g_co2_density_mg_m3);
    //lcd_display_co2(g_co2_density_mg_m3);
}

void co2_power_on()
{
    CO2_GPIO_PORT->ODR |= (uint8_t)( CO2_GPIO_PIN);
    g_co2_is_power_on = 1;print("co2 powen on");
}


void co2_power_off()
{
    CO2_GPIO_PORT->ODR &= (uint8_t)(~CO2_GPIO_PIN);
    g_co2_is_power_on = 0;
    g_co2_is_warm_up = 0;print("co2 powen off");
}


//TODO
void co2_detect_process(void)
{
    ADC1_C4_Init();
    ADC1_StartConversion();
    #if 1
    //{FlagStatus flag_status = RESET;
    //do
    //{
    //    flag_status = ADC1_GetFlagStatus(ADC1_FLAG_EOC);
    //} 
    //while(!flag_status);}
    while(!ADC1_GetFlagStatus(ADC1_FLAG_EOC)){nop();nop();};
    ADC1_ClearFlag(ADC1_FLAG_EOC);
    g_adc1_co2_ad_value = ADC1_GetConversionValue();
  
    print_u16("co2 = ",g_adc1_co2_ad_value);
    #else
    while( RESET == g_adc_finished){ nop();nop();}//wait adc finished          
    g_adc1_co2_ad_value = g_ad_value;
    #endif
    co2_calculate_density(g_adc1_co2_ad_value);
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
    CO2_GPIO_PORT->CR2 &=(u8)( ~(CO2_GPIO_PIN));//|= CO2_GPIO_PIN;   // &=(u8)( ~(CO2_GPIO_PIN));  
}
