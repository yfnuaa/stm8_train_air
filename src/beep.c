/**
  ******************************************************************************
  * @file    : beep.c
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 

#include "stm8s.h"
#include "lcd.h"
#include "debug.h"



#define BEEP_GPIO_PORT       GPIOD
#define BEEP_GPIO_PIN        GPIO_PIN_4

#define GPIO_BUZZER     1

uint8_t g_beep_is_on = 1;


#if 0
void beep_stop(void)
{   
    g_timerE_100ms = e_timer_stop;
    g_timerF_300ms = e_timer_stop;
    g_timerG_2s    = e_timer_stop;
    Buzzer_OFF;
    g_buzzer_enable = RESET;
}

void beep_100ms(void)
{
    if(!g_buzzer_enable)return;
    Buzzer_ON;
    g_timerE_100ms = e_timer_start;
}
void beep_300ms(void)
{
    if(!g_buzzer_enable)return;
    Buzzer_ON;
    g_timerF_300ms = e_timer_start; 
}
void beep_2s(void)
{
    if(!g_buzzer_enable)return;
    Buzzer_ON;
    g_timerG_2s = e_timer_start; 
}
 
#endif


void beep_on(void)
{
    #if GPIO_BUZZER
    BEEP_GPIO_PORT->ODR |= (uint8_t)( BEEP_GPIO_PIN);
    #else 
    lcd_tone_on();
    #endif

    g_beep_is_on = 0;   
}


void beep_off(void)
{
    #if GPIO_BUZZER
    BEEP_GPIO_PORT->ODR &= (uint8_t)(~BEEP_GPIO_PIN);
    #else
    lcd_tone_on();
    #endif

    g_beep_is_on = 0;   
}


void beep_init(void)
{
    //GPIO_Init(GPIOD,GPIO_PIN_4,GPIO_MODE_OUT_OD_LOW_FAST);
    //GPIO_Init(GPIOD,GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_SLOW); 
    #if GPIO_BUZZER
    BEEP_GPIO_PORT->DDR |= GPIO_PIN_4;
    BEEP_GPIO_PORT->CR1 |= GPIO_PIN_4;
    BEEP_GPIO_PORT->CR2 &=(u8)( ~(GPIO_PIN_4));
    #endif
}
