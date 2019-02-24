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

enum pwm_motor_speed_step g_pwm_motor_speed_step = e_speed_off;


void pwm_set_freq(u8 frqKZH)
{
    // void TIM2_SetCompare3(uint16_t Compare3);
}

void pwm_set_duty(enum pwm_motor_speed_step step)
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


void pwm_set_motor_speed(enum pwm_motor_speed_step step)
{
    pwm_set_duty(step);
    g_pwm_motor_speed_step = step;
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
            //TODO
            break;
            
        default:
            break;
    }

    pwm_set_duty(step);
    g_pwm_motor_speed_step = step;
}


void pwm_set_motor_speed_down(void)
{
    enum pwm_motor_speed_step step;

    print("PWM Speed  --> Down");
    
    switch( g_pwm_motor_speed_step )
    {
        case e_speed_off:
            //TODO
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
            
        default:
            break;
    }

    pwm_set_duty(step);
    g_pwm_motor_speed_step = step;
}


void pwm_init(void)
{ 
    TIM2_TimeBaseInit(TIM2_PRESCALER_1, 100);//  f=20k
    TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 0, TIM2_OCPOLARITY_HIGH);  //0 duty pwm init        
    TIM2_OC3PreloadConfig(ENABLE);
    TIM2_Cmd(ENABLE);  
}
