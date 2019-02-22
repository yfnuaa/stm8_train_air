/**
  ******************************************************************************
  * @file    : timer1.c
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 

#include "debug.h"
#include "stm8s_tim4.h"

void timer4_isr(void)
{
    TIM1_ClearITPendingBit(TIM4_IT_UPDATE);
  
}


void timer4_start(void)
{
}

void timer4_init(void)
{
    #if 0
    //assert_param(IS_TIM1TIMERTIME_OK(10000));     
    TIM4_DeInit();//复位TIM1所有参数  
    TIM4_TimeBaseInit(15,TIM1_COUNTERMODE_UP,10000,0); 
    TIM4_ARRPreloadConfig(ENABLE); 
    TIM4_Cmd(ENABLE);//使能计时器  
    TIM4_ITConfig(TIM1_IT_UPDATE,ENABLE);
    #endif
}
