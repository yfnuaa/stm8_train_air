/**
  ******************************************************************************
  * @file    : timer1.c
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 
#define TIMER1_C  1
#include "debug.h"
#include "stm8s_tim1.h"
#include "beep.h"
#include "touch_key.h"
#include "co2.h"


volatile uint32_t g_system_10ms_tick = 0; 
volatile uint32_t g_system_tick; 
void timer1_isr(void)  // 10 10 ms timer
{
    //���� Power �� ��������
    if(g_touch_key_power_long_press_count && (BitStatus)(GPIOC->IDR & GPIO_PIN_5))
    {
        g_touch_key_power_long_press_count++;
    }
    else 
    {
        g_touch_key_power_long_press_count = 0;
    }
    
    if(g_touch_key_power_long_press_count >= LONG_PRESS_TIME_DEFINE)
    {
        print("power long pressed");
        touch_key_power_long_press_handle();
        g_touch_key_power_long_press_count = 0;
    }

    //���� ���� ����
    if( g_beep_need_on )
    {
        g_beep_timer_counter--;

        if( g_beep_timer_counter == 0 )
        {
            g_beep_need_on = 0;
            beep_derect_off();
        }
    }

    //������ TODO ����Ч����Ҫ��������ļ���ֵ��
    /* ��. ��. ��������. ��. ��������. ��. ������ */
    if( g_need_beep_alarm == 1 )
    {
        g_beep_alarm_count++;

        if( g_beep_alarm_count == 1 )
        {
            beep_derect_on();
        }
        else if( g_beep_alarm_count == 20 )
        {
            beep_derect_off();
        }
        else if( g_beep_alarm_count == 40 )
        {
            beep_derect_on();
        }
        else if( g_beep_alarm_count == 60 )
        {
            beep_derect_off();
        }
        else if( g_beep_alarm_count == 200 )
        {
            g_beep_alarm_count = 0;
        }
    }

    //ϵͳʱ�ӣ��������ڶ�ʱ
    ++g_system_10ms_tick;

    if( g_system_tick > 0 )
    {
        --g_system_tick;
    }

    //��ʱ������
    if(g_system_detect_timer_count>0) 
    {
        g_system_detect_timer_count --;
    }

    if( g_system_beep_alarm_timer_count >0)
    {
        --g_system_beep_alarm_timer_count;
    }

    if( g_system_check_alarm_timer_count>0 )
    {
        --g_system_check_alarm_timer_count;
    }

    //CO2 ����
    if( g_co2_is_power_on )
    {
        ++g_co2_power_time_count;

        if( g_co2_is_warm_up == 0 )
        {   
            //TODO ���Ҳ���Զ�̬
            if( g_co2_power_time_count == CO2_WARM_UP_OK_TIME_2_MINUTES )
            {
                g_co2_is_warm_up = 1;
            }
        }
    }
    else
    {
        g_co2_power_time_count = 0;
        g_co2_is_warm_up = 0;
    }
    
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
    TIM1_Cmd(ENABLE);                   //ʹ�ܼ�ʱ��  
}