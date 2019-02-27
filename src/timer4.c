/**
  ******************************************************************************
  * @file    : timer4.c
  * @brief   : used for pm2.5 delay 280us
  * @author  : 
  * @version :
  * @date    :
  */ 

#include "debug.h"
#include "stm8s_tim4.h"
#define TIMER4_C    1
#include "timer4.h"

volatile uint8_t g_time4_280us_ok = 0;
volatile uint8_t g_time4_40us_ok = 0;

enum time4_start_mode g_time4_start_mode = e_mode_null;

void timer4_isr(void)
{
    if( g_time4_start_mode == e_mode_280_us )
    {
        g_time4_280us_ok = 1;
        g_time4_start_mode = e_mode_null;
        //print("timer4_isr_280");
    }
    else
    {
        g_time4_40us_ok = 1;
        g_time4_start_mode = e_mode_null;
        //print("timer4_isr_40");
    }
    //GPIO_WriteReverse(GPIOB, GPIO_PIN_5);
    TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
    TIM4_Cmd(DISABLE);  
}


void timer4_start_280us(void)         
{   
    print("timer4_start_280us");
    
    g_time4_start_mode = e_mode_280_us;
    g_time4_280us_ok = 0;

    //TIM4_Cmd(DISABLE);  
    TIM4_DeInit();  
		#ifdef USE_DEFAULT_CLK_2M
    TIM4_TimeBaseInit(TIM4_PRESCALER_4, 140);      //2M     140
		#else
		TIM4_TimeBaseInit(TIM4_PRESCALER_32,140);      //16 M
		#endif
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);    
    //TIM4_ARRPreloadConfig(ENABLE);    
    TIM4_Cmd(ENABLE);  
}


void timer4_start_40us(void)         
{   
    print("timer4_start_40us");
    
    g_time4_start_mode = e_mode_40_us;
    g_time4_40us_ok = 0;

    TIM4_Cmd(DISABLE);  
    TIM4_DeInit();  
	  #ifdef USE_DEFAULT_CLK_2M
    TIM4_TimeBaseInit(TIM4_PRESCALER_2, 40);     //2M   
		#else
		TIM4_TimeBaseInit(TIM4_PRESCALER_16,40);   //???4,????16??.
		#endif
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);    
    //TIM4_ARRPreloadConfig(ENABLE);    
    TIM4_Cmd(ENABLE);  
}

