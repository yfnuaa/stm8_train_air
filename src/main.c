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


#define SYSTEM_SENSOR_DETECT_INTERVAL           2   // 2S
#define SYSTEM_LCD_REFLUSH_INTERVAL             10   //10S

enum system_mode g_system_mode = e_auto_mode;

uint8_t g_system_lcd_flush_time = 0;
uint8_t g_system_lcd_need_flush = 1;


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

void main()
{
    uint8_t i = 0;
		#ifndef USE_DEFAULT_CLK_2M
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);                           // default is 8 div 
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);    /*CLK_PRESCALER_CPUDIV128*/   // set system clock 2 div freq //system 8M speed running 
		#endif

    touch_key_Init();
    uart1_init(); 
    adc1_init();

    #if 0
    while(1)
    {
        print_u8("TTest --------------------------->  ", i++);
        delay_ms(8000);
    }
    
    #endif
    beep_init(); 
    lcd_init();
    lcd_test();
    #if 0
    enableInterrupts(); 
    timer4_start_280us();
    while(1)
    {
    }
    #endif
    
    pwm_init();
    pm25_init();
    
    
    //timer1_init();
    //timer4_start_40us();
    
    enableInterrupts(); 
     
    adc1_start();
    
 
    print("System Power On");
 

    #if 0
    while( 1 )
    {
        pm25_led_off();
        delay_ms(100);
        pm25_led_on();
        delay_ms(100);
    }
    #endif
    
    while( 1 )
    {
        
        //print("RE ...");
        //pm25_set_detect_begin()
        print_u8("Test --------------------------->  ", i++);
        
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
            g_system_lcd_flush_time += SYSTEM_SENSOR_DETECT_INTERVAL;
            
            if( g_pm25_need_detect )    
            {
                pm25_set_detect_begin();
            }
            //ADC start
            adc1_start();
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

            //LCD 更新
            if( (g_system_lcd_need_flush != 0) &&
                    (g_system_lcd_flush_time == SYSTEM_LCD_REFLUSH_INTERVAL) )
            {
                g_system_lcd_flush_time = 0;
                
                if( 1 ) //其他条件
                {
                    //LCD 更新
                    //lcd_update_indicate_progress(g_pm25_dust_density, g_co2_value, fan_setp, air_setp);
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
                break;
               
        }        
    }
}



