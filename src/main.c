/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */
#define  SYSTEM_C   1
#include "system.h"
#include "pm25.h"
#include "co2.h"
#include "touch_key.h"
#include "beep.h"
#include "lcd.h"
#include "pwm.h"
#include "adc1.h"
#include "debug.h"
#include "timer1.h"
#include "timer4.h"
#include "utility.h"
#include "beep.h"


#define SYSTEM_SENSOR_DETECT_INTERVAL           2   // 2S
#define SYSTEM_LCD_REFLUSH_INTERVAL             10   //10S

enum system_mode g_system_mode = e_auto_mode;




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
    
    lcd_init();
    lcd_test();
    touch_key_Init();
    uart1_init();   
    adc1_init();
    enableInterrupts();
    timer1_init();
    beep_init(); 
    beep_on_ms(POWER_ON_BEEP_ON_TIME);
 
 
    
    pwm_init();
    pm25_init();
    
 
 
    print("System Power Onx");
 

    #if 1
    while( 1 )
    {
      //  adc1_start();x
 
        pm25_led_on();
        pm25_power_off();
        delay_280us();
        
        delay_40us();
        pm25_led_off();
        pm25_power_on();
        delay_ms(100);
    }
    #endif
    
    while(1)
    {
        
        //print("RE ...");
        //pm25_set_detect_begin()
        print_u8("Test --------------------------->  ", i++);
        if(SET == g_touch_power_long_pressed)print("power ON long pressed");
        #if 1
        pm25_set_detect_begin();
        //timer4_start_280us();
        delay_280us();
        //while(g_time4_280us_ok != 0);
        adc1_start();
        
        //timer4_start_40us();
        //while(g_time4_40us_ok != 0);
        delay_40us();
        pm25_set_detect_end();
        
        print_u16("g_adc1_pm25_ad_value = ", g_adc1_pm25_ad_value);
        
        #endif
        
        delay_ms(10000);
        print_enter();
        print_enter();
        //delay_280us();
    }
    

    //上电自检
    //上电 LCD 全亮一下
    // ...
    // ...
    //显示默认值 如 ---  ---
    while(1)
    {
        if( (g_system_mode == e_auto_mode) || (g_system_mode == e_manual_mode) || (g_system_mode == e_sleep_mode))
        {
            //Delay
            delay_s(SYSTEM_SENSOR_DETECT_INTERVAL);
   
            if( g_pm25_need_detect )    
            {
                pm25_set_detect_begin();
            }
            //ADC start
            //adc1_start();
            while( g_adc1_is_ok == 1 );
            
            if( g_pm25_need_detect )    
            {
                pm25_set_detect_end();
            }

            //PM2.5 计算
            if( g_pm25_is_power_on && g_pm25_need_detect )
            {
                pm25_calculate_density(g_adc1_pm25_ad_value);
            }

            if( g_co2_is_power_on && g_co2_is_warm_up && g_co2_need_detect )
            {
                co2_calculate_density(g_adc1_co2_ad_value);
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
                break;
               
        } 
        g_touch_power_long_pressed = RESET;        
    }
}



