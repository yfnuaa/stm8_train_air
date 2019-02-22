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
#include "stm8s_tim2.h" 


void pwm_init(void)
{ 
    //void TIM1_SetAutoreload(uint16_t Autoreload);
    //GPIO_Init(GPIOA,GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_SLOW);
    //TIM2_DeInit();
    TIM2_TimeBaseInit(TIM2_PRESCALER_1,800);//  f=20k
   
    TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 00, TIM2_OCPOLARITY_HIGH);  //0 duty pwm init        
    //pwm_set_duty(0);
    TIM2_OC3PreloadConfig(ENABLE);
   // TIM2_ARRPreloadConfig(ENABLE);
    TIM2_Cmd(ENABLE); 
}
void pwm_set_freq(u8 frqKZH)
{
    // void TIM2_SetCompare3(uint16_t Compare3);
}
void pwm_set_duty(u8 duty)
{
 
    if(duty>100) return;
    
    //send_string("[duty]");
    
    TIM2_SetCompare3(8*duty); 
    //TIM2_SetCompare3(duty);    
}
void pwm_stop(void)
{
    TIM2_Cmd(DISABLE);  
    TIM2_ForcedOC3Config(TIM2_FORCEDACTION_INACTIVE);
}
 
