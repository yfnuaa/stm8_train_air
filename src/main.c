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

u16 g_air_quality = 0;
volatile enum  system_mode g_system_mode = e_dummy_mode;
volatile enum  system_mode g_last_system_mode = e_dummy_mode;

volatile int8_t g_need_operate_after_mode_switch = 0;   //模式切换时，是否需要 做操作，切换模式时，设置

volatile int8_t g_system_need_detect = 0;               //需要 检测传感器


volatile int8_t g_system_need_check_alarm = 1;         // 是否要去 check 空气的质量，从而 去 设置 报警
                                                       // 用于在报警后，30 分钟内 不用去 check 是否要报警
                                                       /* 系统初始化 为 1， 需要去 check */

#define SYSTEM_SENSOR_DETECT_5_MINUTES_INTERVAL         500//30000 // (5min * 60s * 100) 10ms
#define SYSTEM_SENSOR_DETECT_15_MINUTES_INTERVAL        500//90000 // (15min * 60s * 100) 10ms
volatile int32_t g_system_detect_timer_count = 0;                   //系统 检测 定时器

#define SYSTEM_BEEP_ALARM_1_MINUTES_INTERVAL            6000 // (1min * 60s * 100) 10ms
#define SYSTEM_BEEP_ALARM_INTERVAL      SYSTEM_BEEP_ALARM_1_MINUTES_INTERVAL
volatile int32_t g_system_beep_alarm_timer_count = 0;        //系统 报警 定时器 定时 1 分钟


#define SYSTEM_CHECK_ALARM_30_MINUTES_INTERVAL          180000 // (30min * 60s * 100) 10ms
#define SYSTEM_CHECK_ALARM_INTERVAL     SYSTEM_CHECK_ALARM_30_MINUTES_INTERVAL
volatile int32_t g_system_check_alarm_timer_count = 0;   //系统 检测是否 要 报警 定时器， 定时 30 分钟


//TODO
void start_motor_and_diaplay_quality_according_detect_value(void)
{
    #if 0//TODO
    if( g_pwm_motor_speed_step == e_speed_off )
    {
        if(g_co2_density_mg_m3<= )
        {
            pwm_set_motor_speed(e_speed_off);
        }
        else if(g_co2_density_mg_m3 <= )
        {
            pwm_set_motor_speed(e_speed_low);
        }
        else if(g_co2_density_mg_m3 <= )
        {
            pwm_set_motor_speed(e_speed_middle);
        }        
        else if(g_co2_density_mg_m3 <= )
        {
            pwm_set_motor_speed(e_speed_high);
        }
        else  
        {
            pwm_set_motor_speed(e_speed_high+10);
        }
    }

    #endif
    //电机没有运转时，再判断 PM2.5
    //if( g_pwm_motor_speed_step == e_speed_off )
    {
        if(g_pm25_dust_density_ug_m3 <= e_pm25_level_1)
        {
            pwm_set_motor_speed(e_speed_off);
            lcd_display_air_quality(  0 );
        }
        else if(g_pm25_dust_density_ug_m3 <= e_pm25_level_2)
        {
            pwm_set_motor_speed(e_speed_low);
            lcd_display_air_quality(  1 );
        }
        else if(g_pm25_dust_density_ug_m3 <= e_pm25_level_3)
        {
            pwm_set_motor_speed(e_speed_middle);
            lcd_display_air_quality(  2 );
        }        
        else if(g_pm25_dust_density_ug_m3 <= e_pm25_level_4)
        {
            pwm_set_motor_speed(e_speed_high);
            lcd_display_air_quality(  2 );
        }
        else  //> e_pm25_bad
        {
            pwm_set_motor_speed(e_speed_high+10);
            lcd_display_air_quality(  2 );
        }
    }
}


void alarm_config_after_detect(void)
{  
    //在没有报警 && 需要 Check 是否需要报警时
    if( (g_system_need_check_alarm == 1) && (g_need_beep_alarm == 0) )
    {  
        print("Main --> Check Alarm");
        
        /* 这里最好使用 LCD 的显示值 作为 判断条件 */
        if( (g_co2_density_mg_m3 > 600) || (g_pm25_dust_density_ug_m3 > 200) )   //TODO
        {   
            /* Sleep Mode 下，只是 滴 一下 */
            if( g_system_mode == e_sleep_mode )
            {
                beep_on_ms(200);
            }
            else    //
            {
                g_need_beep_alarm = 1;
            }

            //在 Auto Mode 下 自动 启动风扇

        }
        else
        {
            if( g_system_mode != e_sleep_mode )     //在 Sleep mode 下，不关闭 风扇
            {
                //关闭风扇
                pwm_set_motor_speed(e_speed_off);
            }
            
        }

        /* 这里设置 beep 开始 报警 */
        /* 开始 定时 1 分钟 */
        /* 开始 定时 30 分钟 后在 去 check  */
        if( g_need_beep_alarm == 1 )
        {
            g_system_beep_alarm_timer_count = SYSTEM_BEEP_ALARM_INTERVAL;
            g_system_check_alarm_timer_count = SYSTEM_CHECK_ALARM_INTERVAL;
            g_system_need_check_alarm = 0;
        }  
    }

    if( g_need_beep_alarm == 1 )    
    {
        //在报警时，去 check 报警 1 分钟是否 到
        if( g_system_beep_alarm_timer_count == 0 )
        {
            g_need_beep_alarm = 0;
        }
    }

    //在不需要 Check 是否需要报警时，判断，用于 等待 30 分钟 定时结束
    if( g_system_need_check_alarm == 0 ) 
    {   
        if( g_system_check_alarm_timer_count == 0 )
        {
            g_system_need_check_alarm = 1;
        }
    }
}



void main()
{
    uint8_t i = 0;
    
    #ifndef USE_DEFAULT_CLK_2M
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);                           // default is 8 div 
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);    /*CLK_PRESCALER_CPUDIV128*/  // set system clock 2 div freq //system 8M speed running 
    #endif
    
    /* ================================  系统上电 BEGIN ================================= 
        1.	上电初始化
        2.	背光亮一下（各显示区域可以显示一个默认的值）
        3.	系统进入 【power_off mode】模式，等待长按 power 键开机
        4.	背光灭
    */	

    //TODO 初始化 顺序
    touch_key_Init(); 
    enableInterrupts();
    timer1_init(); 
    beep_init();
    uart1_init();   
    print("POooooooooooooooooxxxxxxxxxxxxxxxxxxyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy"); 

    //LCD
    lcd_init();
    
    lcd_back_light_on();
    
    //beep_on_ms(400);
    
    lcd_display_power_on_mode(); 
    pwm_init();
    
    pm25_init();
    co2_init();

    g_system_tick = 150;
    while( g_system_tick != 0 );
    lcd_back_light_off();
    
    //TODO
    
    /* 上电后进入 Power Off Mode */
    g_system_mode = e_power_off_mode;
    g_last_system_mode = e_dummy_mode;
    g_system_need_detect = 0;
    /* ================================  系统上电 END ================================= */
    #if 0  
    pm25_power_on();co2_power_on();
    while(1)
    {//co2_power_on(); pm25_power_on();print("on");
        pm25_detect_process(); 
        delay_ms(1000); 
        ///co2_power_off(); pm25_power_off(); print("off");
         co2_detect_process();
        //pm25_led_off();pm25_power_off();
         delay_ms(1000);     
        //  pm25_led_on();pm25_power_on();
        //  delay_ms(5000);
    }
    #endif 

    while(1)
    {           
        /* 传感器 检测 */
        if(  (g_system_detect_timer_count == 0) && (g_system_need_detect == 1) )
        {
            if( g_system_mode == e_sleep_mode )
            {
                g_system_detect_timer_count = SYSTEM_SENSOR_DETECT_15_MINUTES_INTERVAL;
            }
            else
            {
                g_system_detect_timer_count = SYSTEM_SENSOR_DETECT_5_MINUTES_INTERVAL;
            }
               
            print("Main --> Detect");
            {
                static bool temp_flag = 0;
                temp_flag = !temp_flag;
                
                if(temp_flag)
                {
                    pm25_detect_process(); 
                    lcd_display_pm25(g_pm25_dust_density_ug_m3);
                    //pm25_detect_process(); 
                }
                else
                {          
                    co2_detect_process();
                    lcd_display_co2(g_co2_density_mg_m3);
                    //delay_ms(20);
                    //co2_detect_process();
                }
            }
            
            if( g_system_mode == e_auto_mode )
            {
                start_motor_and_diaplay_quality_according_detect_value(); 
            }
            
            //g_air_quality = 1;  //用于显示 LCD 上的 空气质量
        }
        
        /* 每次检测完，就更新 LCD */
        
        
        /* TODO  */

        /* 根据检测值 设置 报警 */
        switch( g_system_mode )
        {
            case e_auto_mode:
            case e_manual_mode:
            case e_sleep_mode:
               // alarm_config_after_detect();
            break;
                
            default:
            break;
        }

        //模式切换 处理
        if( g_system_mode != g_last_system_mode )
        {
            g_need_operate_after_mode_switch = 1;
            g_last_system_mode = g_system_mode;
        }
		if(!g_need_operate_after_mode_switch)
			continue;
        switch(g_last_system_mode)
        {   
            // power_on mode
            case e_power_on_mode:
                if( g_need_operate_after_mode_switch == 1 )
                {   
                    g_need_operate_after_mode_switch = 0;
                    g_system_mode = e_auto_mode;
                    g_system_detect_timer_count = 0;    //上电 后 就要 开始检测

                    g_pwm_motor_speed_step = 0;
                    
                    print("Main --> Power_On Mode");
                    /* 
                        1.	显示 LCD 启动显示（显示动画效果）
                        2.	PM2.5， CO2传感器上电
                        3.	背光亮
                        4.	系统进入【auto mode】
                    */
                    
                    co2_power_on(); 
                    pm25_power_on();
    
                    pwm_init();
                    /* 1. */
                    
                    //lcd_init();
                    //lcd_on();
                    lcd_back_light_on();
                    lcd_display_switch_on();
                    ht1621_light_up_always_on_seg();
                    
                    /* 3. */
                    /* 4. */
                    g_system_need_detect = 1;
                }
                break;
                
            //Auto mode
            case e_auto_mode: 
                if( g_need_operate_after_mode_switch == 1 )
                {
                    g_need_operate_after_mode_switch = 0;
                    print("Main --> Auto Mode");
                    lcd_light_mode_auto();
                    /* 
                        1.	系统每隔 5 分钟，检测一次PM2.5 和 CO2
                        2.	根据检测的数据调节风扇速度
                        3.	如果检测到 PM2.5 or CO2 异常，蜂鸣器报警（滴. 滴. ………滴. 滴. ………滴. 滴. ………）
                             蜂鸣器报警 1 分钟后，自动取消报警；如果期间有任何按键，将取消蜂鸣器报警。
                            下一次报警检测为30分钟后 
                        4.	Goto  1
                    */
                    g_system_need_detect = 1;
                    g_system_need_check_alarm = 1;
                }
                break;

            //Manual Mode
            case e_manual_mode:
                if( g_need_operate_after_mode_switch == 1 )
                {
                    g_need_operate_after_mode_switch = 0;
                    print("Main --> Manual Mode");
                    lcd_light_mode_manual();
                    /* 
                        1.	进入Manual Mode 后，不改变当前风扇的状态，及当前风扇是运行的，还是运行。
                        2.	系统每隔 5 分钟，检测一次PM2.5 和 CO2。
                        3.	如果检测到 PM2.5 or CO2 异常，蜂鸣器报警（滴. 滴. ………滴. 滴. ………滴. 滴. ………）
                            蜂鸣器报警 1 分钟后，自动取消报警；如果期间有任何按键，将取消蜂鸣器报警。
                            下一次报警检测为30分钟后
                        4.	如果按键加减，则切换相应风扇转速。
                        5.	Goto 2
                    */
                    
                    g_system_need_check_alarm = 1;   
                }
                break;
                
            //Sleep Mode
            case e_sleep_mode:
                if( g_need_operate_after_mode_switch == 1 )
                {   
                    g_need_operate_after_mode_switch = 0;
                    print("Main --> Sleep Mode");
                    lcd_light_mode_sleep();
                    /* 
                        1.	进入Sleep Mode 后，风扇速度最低速。
                        2.	系统每隔 15 分钟，检测一次PM2.5 和 CO2。
                        3.	如果检测到 PM2.5 or CO2 异常，蜂鸣器报警（滴一次）,下一次报警检测为30分钟后
                        4.	如果接收到关机，则关机进入power off mode
                        5.	Goto 2
                    */
                    
                    pwm_set_motor_speed(e_speed_low);

                    //睡眠模式下，是否需要 多长时间后 进入 power off mode   //TODO
                }
                break;

            //Power Off Mode
            case e_power_off_mode: 
                if( g_need_operate_after_mode_switch == 1 )
                {
                    g_need_operate_after_mode_switch = 0;
                    print("Main --> Power_Off Mode");
                    /* 
                        此模式只响应 “Power”按键
                        1.	关闭风扇
                        2.	关闭传感器电源
                        3.	关闭LCD 屏幕，及 背光
                        4.	等待 “Power”键按键开机。
                    */
                    co2_power_off();  
                    pm25_power_off();
                    adc1_reset();
                    lcd_back_light_off();
                    //lcd_off();
                    beep_derect_off();
                    pwm_close();
                       
                    g_system_need_check_alarm = 0;
                        
                    g_system_detect_timer_count = 0;
                    g_system_beep_alarm_timer_count = 0;
                    g_system_check_alarm_timer_count = 0;   
                }  
                break;

            default:
                break;
                    
        } 
    }
}



