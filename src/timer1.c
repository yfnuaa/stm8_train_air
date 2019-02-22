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

void timer1_isr(void)
{
    
}

void timer1_start(void)
{
    
}

void timer1_init(void)
{
    //assert_param(IS_TIM1TIMERTIME_OK(10000));     
    TIM1_DeInit();//复位TIM1所有参数  
    TIM1_TimeBaseInit(15,TIM1_COUNTERMODE_UP,10000,0); 
    TIM1_ARRPreloadConfig(ENABLE); 
    TIM1_Cmd(ENABLE);//使能计时器  
    TIM1_ITConfig(TIM1_IT_UPDATE,ENABLE);
}