/**
  ******************************************************************************
  * @file    : touch_1.c
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 
#include "stm8s.h"
#include "touch_key.h"
#include "beep.h"
#include "debug.h"
#include "system.h"
#include "pwm.h"
#include "co2.h"
#include "adc.h"
#include "lcd.h"
#include "timer4.h"


#define TOUCH_KEY_PORT          GPIOC   
#define TOUCH_KEY_EXTI_PORT     EXTI_PORT_GPIOC

#define TOUCH_KEY_PLUS_PIN      GPIO_PIN_4    //+
#define TOUCH_KEY_POWER_PIN     GPIO_PIN_5    //power
#define TOUCH_KEY_MINUS_PIN     GPIO_PIN_6    //-
#define TOUCH_KEY_MODE_PIN      GPIO_PIN_7    //mode

#define TOUCH_KEY_ALL_PIN   ( GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6| GPIO_PIN_7 )


volatile u16 g_touch_key_power_long_press_count = 0;


void touch_key_plus_press_handle(void)
{
    //报警时，按任意键 停止
    g_need_beep_alarm = 0;
    
    if( g_system_mode == e_manual_mode )
    {
        beep_on_ms(SHORT_PRESS_BEEP_ON_TIME);
        pwm_set_motor_speed_up();
    }
}

void touch_key_minus_press_handle(void)
{
    //报警时，按任意键 停止
    g_need_beep_alarm = 0;
    
    if( g_system_mode == e_manual_mode )
    {
        beep_on_ms(SHORT_PRESS_BEEP_ON_TIME);
        pwm_set_motor_speed_down();
    }
}

void touch_key_mode_press_handle(void)
{
    //报警时，按任意键 停止
    g_need_beep_alarm = 0;

    //TODO
    if(g_system_mode != e_power_off_mode)
    {
        beep_on_ms(SHORT_PRESS_BEEP_ON_TIME);
    }
    
    switch( g_system_mode )
    {
        case e_auto_mode:
            g_system_mode = e_manual_mode;
            
            print("Switch --> Manual Mode");
            //g_need_operate_after_mode_switch = 1;
            break;
            
        case e_manual_mode:
            g_system_mode = e_sleep_mode;
            
            print("Switch --> Sleep Mode");
            //g_need_operate_after_mode_switch = 1;
            break;

        case e_sleep_mode:
            g_system_mode = e_auto_mode;
            
            print("Switch --> Auto Mode");
            //g_need_operate_after_mode_switch = 1;
            break;
 
        default:
            break;
    }
}


void touch_key_power_long_press_handle(void)
{
    //报警时，按任意键 停止
    g_need_beep_alarm = 0;

    //TODO
    beep_on_ms(LONG_PRESS_BEEP_ON_TIME);
    
    switch( g_system_mode )
    {
        case e_auto_mode:
        case e_manual_mode: 
        case e_sleep_mode:
        case e_power_on_mode:
            g_system_mode = e_power_off_mode;
            print("Switch --> Off Mode");
            //g_need_operate_after_mode_switch = 1;
            break;
            
        case e_power_off_mode:
            g_system_mode = e_power_on_mode;
 			//g_need_operate_after_mode_switch = 1;
            print("Switch --> Power_On Mode");
            break;
                        
        default:
            break;
    }
}

extern volatile u16 g_10ms_delay_count; //不知道 这个的作用


#if RUIZHU_TEST
int8_t g_test_speed = 0;
#endif
void touch_key_gpio_isr(void)
{
    uint8_t value = GPIOC->IDR;
    print("Key isr");
    //touch_key_DeInit();// stop key interrupt

    //Power Key
    if( (value & TOUCH_KEY_POWER_PIN) != 0)
    {
        print("Key[ Power Power Power Power Power Power]");
        g_touch_key_power_long_press_count = 1;
    }

    //Mode Key
    if( (value & TOUCH_KEY_MODE_PIN) != 0)
    {
        print("Key[ModeModeModeModeModeModeModeModeModeModeModeMode Mode]");
        touch_key_mode_press_handle();
    }

    // --- Key
    if( (value & TOUCH_KEY_MINUS_PIN) != 0)
    {
        print("Key[-----------------------------------]");
   
        #if RUIZHU_TEST
        beep_on_ms(SHORT_PRESS_BEEP_ON_TIME);
        g_test_speed-=1;
        if(g_test_speed<0) 
        {
            g_test_speed = 0;
        }
        pwm_set_freq(g_test_speed*30);
        lcd_display_pm25(g_test_speed);
        
        #else 
       
        touch_key_minus_press_handle();
        #endif
    }

    // +++ Key
    if( (value & TOUCH_KEY_PLUS_PIN) != 0)
    {
        print("Key[++++++++++++++++++++++++++++++++++++++]");
 
        #if RUIZHU_TEST
        beep_on_ms(SHORT_PRESS_BEEP_ON_TIME);
        g_test_speed+=1;
        
        if(g_test_speed>100) 
        {
            g_test_speed = 100;
        }
        pwm_set_freq(g_test_speed*30);
        lcd_display_pm25(g_test_speed);
        
        #else 
       
        touch_key_plus_press_handle();
        #endif
    }
    
	//g_10ms_delay_count = 10;
   // timer4_start();
}



void touch_key_interrupt_able(enum e_touch_key key, uint8_t able)
{
    switch( key )
    {
        case e_touch_key_plus:
            break;
            
        case e_touch_key_minus:
            break;
            
        case e_touch_key_mode:
            break;
            
        case e_touch_key_power:
            break;
            
        case e_touch_key_all:
            break;
            
        default:
            break;
    }
}


void touch_key_Init(void)
{
 
    GPIO_Init(TOUCH_KEY_PORT, TOUCH_KEY_ALL_PIN, GPIO_MODE_IN_FL_IT);
    EXTI_SetExtIntSensitivity(TOUCH_KEY_EXTI_PORT, EXTI_SENSITIVITY_RISE_ONLY);
}
void touch_key_DeInit(void)
{
    GPIO_Init(TOUCH_KEY_PORT, TOUCH_KEY_ALL_PIN, GPIO_MODE_IN_FL_NO_IT);
    //EXTI_SetExtIntSensitivity(TOUCH_KEY_EXTI_PORT, EXTI_SENSITIVITY_RISE_ONLY);
}



