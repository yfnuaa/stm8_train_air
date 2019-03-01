/**
  ******************************************************************************
  * @file    : timer1.c
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 

#include "debug.h"
#include "stm8s_tim1.h"
#include "beep.h"
#include "touch_key.h"
#define LONG_PRESS_TIME_DEFINE 150 //means 1.5s
extern volatile u16 g_system_sensor_detect_timer_flag;
void timer1_isr(void)  // 10 10 ms timer
{
    GPIO_WriteReverse(GPIOC, GPIO_PIN_3);
    if(g_touch_long_press_count&&(BitStatus)(GPIOC->IDR & GPIO_PIN_5))
    {
        g_touch_long_press_count++;
    }
    else 
    {
        g_touch_long_press_count = 0;
    }
    if(g_touch_long_press_count >= LONG_PRESS_TIME_DEFINE)
    {
        print("power long pressed");
        g_touch_power_long_pressed = SET;
        g_touch_long_press_count = 0;
    }
    if( g_beep_need_on )
    {
        g_beep_timer_counter--;

        if( g_beep_timer_counter == 0 )
        {
            g_beep_need_on = 0;
            beep_derect_off();
        }
    }
    if(g_system_sensor_detect_timer_flag)g_system_sensor_detect_timer_flag--;
    TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
}

void timer1_start(void)
{
    
}

void timer1_init(void)
{  
    //TIM4CLK is set to 16 MHz,
    TIM1_DeInit();                      
    #ifdef USE_DEFAULT_CLK_2M
    TIM1_TimeBaseInit(15, TIM1_COUNTERMODE_UP, 1250, 0); // 2M   2000000/16 = 125000   125 means 1ms  so 1250 means 10ms 
    #else
    TIM1_TimeBaseInit(15,TIM1_COUNTERMODE_UP,10000,0); // 16M  16M/16 = 1M  so 1000 means 1ms so 10000 means 10ms
    #endif
    TIM1_ARRPreloadConfig(ENABLE); 
    TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
    TIM1_Cmd(ENABLE);                   //使能计时器  
}