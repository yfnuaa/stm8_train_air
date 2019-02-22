/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */
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


enum system_mode
{
    e_auto_mode = 0,
    e_manual_mode,
    e_sleep_mode,
    e_power_off_mode
};

#define SYSTEM_SENSOR_DETECT_INTERVAL           2   // 2S
#define SYSTEM_LCD_REFLUSH_INTERVAL             10   //10S

uint8_t g_system_mode = e_auto_mode;

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
    
    g_pm25_dust_density =(u16) (serialno / 1000);
    g_pm25_dust_density += 10000; //show A01 234 //serial no 1234
    g_co2_value = (u16)(serialno % 1000);
    #endif
}

 
void main()
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1); // set system clock 2 div freq //system 8M speed running 

    touch_key_Init();
    
    adc1_init();
    
    beep_init();
    
    uart1_init(); 
    
    lcd_init();
     
    timer1_init();
    timer4_init();
    
    enableInterrupts(); 
    
    adc1_start();
    
    timer1_start();
    
    print("POW");

    //上电自检
    //上电 LCD 全亮一下
    // ...
    // ...
    //显示默认值 如 ---  ---
    while(1)
    {
        if( (g_system_mode == e_auto_mode) || 
                (g_system_mode == e_manual_mode))
        {
            //Delay
            delay_s(SYSTEM_SENSOR_DETECT_INTERVAL);
            g_system_lcd_flush_time += SYSTEM_SENSOR_DETECT_INTERVAL;
            
            //ADC 检测
            
            if( g_pm25_need_detect )    
            {
                pm25_set_detect_begin();
            }
            
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
                if( g_co2_density > 30 )
                {
                    pwm_set_duty(50);
                }
                else
                {
                    //...
                }
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



