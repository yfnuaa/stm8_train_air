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
#include "beep.h"



#define BEEP_GPIO_PORT       GPIOD
#define BEEP_GPIO_PIN        GPIO_PIN_4



volatile uint8_t g_beep_is_on = 0;

volatile uint8_t g_beep_timer_counter = 0;   // *10ms
volatile uint8_t g_beep_need_on = 0;

void beep_derect_on(void)
{
    #if GPIO_BUZZER
    BEEP_GPIO_PORT->ODR |= (uint8_t)( BEEP_GPIO_PIN);
    #else 
    lcd_tone_on();
    #endif

    g_beep_is_on = 0;   
}


void beep_derect_off(void)
{
    #if GPIO_BUZZER
    BEEP_GPIO_PORT->ODR &= (uint8_t)(~BEEP_GPIO_PIN);
    #else
    lcd_tone_off();
    #endif

    g_beep_is_on = 0;   
}

void beep_on_ms(uint16_t ms)
{
    g_beep_need_on = 1;
    g_beep_timer_counter = (uint8_t)(ms/10);
    beep_derect_on();
}
#if GPIO_BUZZER

void beep_init(void)
{
    //GPIO_Init(GPIOD,GPIO_PIN_4,GPIO_MODE_OUT_OD_LOW_FAST);
    //GPIO_Init(GPIOD,GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_SLOW); 
 
    BEEP_GPIO_PORT->DDR |= GPIO_PIN_4;
    BEEP_GPIO_PORT->CR1 |= GPIO_PIN_4;
    BEEP_GPIO_PORT->CR2 &=(u8)( ~(GPIO_PIN_4));
 
}
#endif

