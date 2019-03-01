/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */
 #include "stm8s.h"
#define  SYSTEM_C   1
#include "system.h"
#include "pm25.h"
#include "co2.h"
#include "touch_key.h"
#include "beep.h"
#include "lcd.h"
#include "pwm.h"
#include "adc.h"
#include "debug.h"
#include "timer1.h"
#include "timer4.h"
#include "utility.h"
#include "beep.h"

#define DEFAULT_SENSOR_DETECT_TIMER   60000 //60*100;//one 1min one time
volatile u32 g_system_sensor_detect_timer_flag; // 2S
u8 g_one_time_collect_timers = 0;
//#define SYSTEM_LCD_REFLUSH_INTERVAL             10   //10S

volatile enum  system_mode g_system_mode = e_auto_mode;




void read_serial_no(void)
{
    #if 0
    u32 serialno = 0;
    serialno  = (u32)( FLASH_ReadByte(SERIALNO   )) <<24; 
    serialno |= (u32)( FLASH_ReadByte(SERIALNO + 1))<<16;  
    serialno |= (u32)( FLASH_ReadByte(SERIALNO + 2))<<8; 
    serialno |=        FLASH_ReadByte(SERIALNO + 3);
    
    g_pm25_dust_density =(uint16_t) (serialno / 1000);
    g_pm25_dust_density += 10000; //show A01 234 //serial no 1234
    g_co2_value = (uint16_t)(serialno % 1000);
    #endif
}

//Def
//PD_DDR     CR1      CR2
//  0         0        0    float             input     pullup resistor is off
//  0         1        0    pullup            input     pullup resistor is on 
//  0         0        1    interrup float    input     pullup resistor is off 
//  0         1        1    interrup pullup   input     pullup resistor is on 

//  1         0        0    open-drain        output    pullup resistor is off 
//  1         1        0    push-pull         output    pullup resistor is off    
//  1         x        1    quick 10Mhz       output    pullup resistor is off  

void main()
{
    uint8_t i = 0;
    #ifndef USE_DEFAULT_CLK_2M
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);                           // default is 8 div 
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);    /*CLK_PRESCALER_CPUDIV128*/   // set system clock 2 div freq //system 8M speed running 
    #endif
    g_system_sensor_detect_timer_flag = 0; 
	
	// beep and lcd
    beep_init(); lcd_tone_off();
    uart1_init();    
    lcd_init();
    lcd_test();
	
	// touch key
    touch_key_Init();
   
 
    enableInterrupts();
    timer1_init();

    beep_on_ms(POWER_ON_BEEP_ON_TIME);
 
    //senor and  motor
    pwm_init();
    pm25_init();
    co2_init();
 
 
    print("PO");
    #if 1
    while(1)
    {
    }
    #endif        
        

    #if 1   //test ad lcd and touch
    while( 1 )
    {
        if(g_touch_power_long_pressed)
        {
            touch_key_power_long_press();
            g_touch_power_long_pressed = RESET;
        }
        pm25_power_on();
        pm25_led_on();
        delay_ms(2);// wait sensor stable 
        adc1_reset();
        //begint c02-------------
        //ADC start
        ADC1_C4_Init();
        //adc1_start();
        while( RESET == g_adc_finished){ nop();nop();}//wait adc finished
        g_adc1_co2_ad_value = g_ad_value; 
        co2_calculate_density(g_adc1_co2_ad_value);
          
        //begint PM25-------------
        //ADC start
        ADC1_C3_Init();
        //adc1_start();
        while( RESET == g_adc_finished){ nop();nop();}//wait adc finished          
 
        g_adc1_pm25_ad_value = g_ad_value;
        pm25_calculate_density(g_adc1_pm25_ad_value);
        delay_ms(5000);
    }
    #endif
     
 
    while(1)
    {
        if(g_touch_power_long_pressed)
        {
            touch_key_power_long_press();
            g_touch_power_long_pressed = RESET;
        }
        if( (g_system_mode == e_auto_mode) || (g_system_mode == e_manual_mode) || (g_system_mode == e_sleep_mode))
        {
            //Delay
            //delay_s(g_system_sensor_detect_timer_flag);
            if(0 == g_system_sensor_detect_timer_flag )//begin to collect ADC data
            { 
                if(g_one_time_collect_timers<=10)
                {
                    pm25_power_on();
                    pm25_led_on();
                    delay_ms(2);// wait sensor stable 
                    adc1_reset();
                    //begint c02-------------
                    //ADC start
                    ADC1_C4_Init();
                    //adc1_start();
                    while( RESET == g_adc_finished){ nop();nop();}//wait adc finished
                    g_adc1_co2_ad_value = g_ad_value; 
                    co2_calculate_density(g_adc1_co2_ad_value);
                      
                    //begint PM25-------------
                    //ADC start
                    ADC1_C3_Init();
                    //adc1_start();
                    while( RESET == g_adc_finished){ nop();nop();}//wait adc finished          
    
                    g_adc1_pm25_ad_value = g_ad_value;
                    pm25_calculate_density(g_adc1_pm25_ad_value);
                    g_one_time_collect_timers++;
                }
                else
                {
                    g_system_sensor_detect_timer_flag = DEFAULT_SENSOR_DETECT_TIMER;
                    g_one_time_collect_timers = 0;
                }
            }
        }
 
        
        switch(g_system_mode)
        {   
            //Auto mode
            case e_auto_mode: 
                if(g_pm25_dust_density <= e_pm25_good - 100)
                {
                    pwm_set_motor_speed(e_speed_off);
                }
                else if(g_pm25_dust_density <= e_pm25_good)
                {
                    pwm_set_motor_speed(e_speed_low);
                }
                else if(g_pm25_dust_density <= e_pm25_mid)
                {
                    pwm_set_motor_speed(e_speed_middle);
                }        
                else if(g_pm25_dust_density <= e_pm25_bad)
                {
                    pwm_set_motor_speed(e_speed_high);
                }
                else  //> e_pm25_bad
                {
                    pwm_set_motor_speed(e_speed_high+10);
                }
                break;
            case e_manual_mode:
            break;
            //Sleep Mode
            case e_sleep_mode:

            break;

            //Power Off Mode
            case e_power_off_mode:  
                co2_power_off();                        
            break;
               
        } 
        g_touch_power_long_pressed = RESET;        
    }
}



