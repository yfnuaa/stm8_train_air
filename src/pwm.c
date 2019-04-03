/**
  ******************************************************************************
  * @file    : pwm.h
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 
#include "stm8s.h"
#include "debug.h"
#define   PWM_C   1
#include "pwm.h"
#include "stm8s_tim2.h" 
#include "lcd.h"

u8 g_pwm_motor_speed_step = e_speed_off;


void print_pwm_step(enum pwm_motor_speed_step step)
{
    switch( step )
    {
        case e_speed_off:
            print("PWM Set --> Off");
            break;
            
        case e_speed_low:
            print("PWM Set --> Low");
            break;
            
        case e_speed_middle:
            print("PWM Set --> Mid");
            break;
            
        case e_speed_high:
            print("PWM Set --> High");
            break;
            
        case e_speed_full:
            print("PWM Set --> Full");
            break;
            
        default:
            print_u8("PWM Set --> ", (uint8_t)step);
            break;
    }
}


void pwm_set_duty(u8  step)
{
    if(step>=e_speed_full) 
    {
        return;
    }
    
    print_u8("PWM Duty = ", (uint8_t)step);
    
    TIM2_SetCompare3(step); 
}


void pwm_stop(void)
{
    TIM2_Cmd(DISABLE);  
    TIM2_ForcedOC3Config(TIM2_FORCEDACTION_INACTIVE);
}


void pwm_set_freq(u16 freq)
{ 
  
    if(freq>3000) return;
    //freq = 333+((3000-freq)*5.5);print_u16("freq_COUNTRR",freq);
    if(0==freq){ pwm_stop();return;};
    freq =(u16)( 1000000/freq);
    print_u16("freq",freq);
    //TIM2_DeInit();
    #ifdef USE_DEFAULT_CLK_2M
    TIM2_TimeBaseInit(TIM2_PRESCALER_2, freq);//  f=20k  T = 50us
    #else
    TIM2_TimeBaseInit(TIM2_PRESCALER_16, freq);//  f=20k  T = 50us     800*(1/16000000) = 50 us
    #endif
    TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, freq>>1, TIM2_OCPOLARITY_HIGH);  //0 duty pwm init        
    TIM2_OC3PreloadConfig(ENABLE);
    TIM2_Cmd(ENABLE); 
    
}

void pwm_set_motor_speed(enum pwm_motor_speed_step step)
{
    if(step>e_speed_full)  
    {
        step = e_speed_full-3;
    }

    print_pwm_step(step);
    if(g_pwm_motor_speed_step == step) return; 
    
    if(step > g_pwm_motor_speed_step)
    {
        while(step>g_pwm_motor_speed_step)
        {
            g_pwm_motor_speed_step++;
            if(g_pwm_motor_speed_step>step)
               g_pwm_motor_speed_step = step;
           
            pwm_set_freq(g_pwm_motor_speed_step*30);
            lcd_display_fan_speed(g_pwm_motor_speed_step);
            delay_ms(80);
        }
    }
    else
    {
        while(step < g_pwm_motor_speed_step)
        {
            g_pwm_motor_speed_step--;
            if(step > g_pwm_motor_speed_step)
               g_pwm_motor_speed_step = step;
           
            pwm_set_freq(g_pwm_motor_speed_step*30);
            lcd_display_fan_speed(g_pwm_motor_speed_step);
            delay_ms(80);
        }
        
        
    }
}


void pwm_set_motor_speed_up(void)
{
    enum pwm_motor_speed_step step;

    print("PWM Speed  --> Up");
    
    switch( g_pwm_motor_speed_step )
    {
        case e_speed_off:
            step = e_speed_low;
            break;
            
        case e_speed_low:
            step = e_speed_middle;
            break;
            
        case e_speed_middle:
            step = e_speed_high;
            break;
            
        case e_speed_high:
            return;
            //step = e_speed_high;
            break;
            
        default:
            break;
    }

    print_pwm_step(step);
    
    pwm_set_motor_speed(step);
    //lcd_display_fan_speed(g_pwm_motor_speed_step);
}


void pwm_set_motor_speed_down(void)
{
    enum pwm_motor_speed_step step;

    print("PWM Speed  --> Down");
    
    switch( g_pwm_motor_speed_step )
    {
        case e_speed_off:
            return;
            break;
            
        case e_speed_low:
            step = e_speed_off;
            break;
            
        case e_speed_middle:
            step = e_speed_low;
            break;
            
        case e_speed_high:
            step = e_speed_middle;
            break;
            
       // case e_speed_full:
       //     step = e_speed_high;
        //    break;
            
        default:
            break;
    }

    print_pwm_step(step);
    
    pwm_set_motor_speed(step);
    //lcd_display_fan_speed(g_pwm_motor_speed_step);
}


void pwm_init(void)
{ 
        #ifdef USE_DEFAULT_CLK_2M
    TIM2_TimeBaseInit(TIM2_PRESCALER_1, 100);//  f=20k  T = 50us
        #else
    TIM2_TimeBaseInit(TIM2_PRESCALER_1, 800);//  f=20k  T = 50us     800*(1/16000000) = 50 us
        #endif
        
    TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 0, TIM2_OCPOLARITY_HIGH);  //0 duty pwm init        
    TIM2_OC3PreloadConfig(ENABLE);
    TIM2_Cmd(ENABLE);  
}
void pwm_close(void)
{
    TIM2_Cmd(DISABLE);
}
