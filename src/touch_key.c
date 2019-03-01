/**
  ******************************************************************************
  * @file    : touch_1.c
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 
#include "stm8s.h"
#include "touch_key.h"
#include "beep.h"
#include "debug.h"
#include "system.h"
#include "pwm.h"
#include "co2.h"
#define TOUCH_KEY_PORT          GPIOC   
#define TOUCH_KEY_EXTI_PORT     EXTI_PORT_GPIOC

#define TOUCH_KEY_PLUS_PIN      GPIO_PIN_4    //+
#define TOUCH_KEY_POWER_PIN     GPIO_PIN_5    //power
#define TOUCH_KEY_MINUS_PIN     GPIO_PIN_6    //-
#define TOUCH_KEY_MODE_PIN      GPIO_PIN_7    //mode

#define TOUCH_KEY_ALL_PIN   ( GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6| GPIO_PIN_7 )

uint8_t g_touch_key_power_pressed = 0;
volatile BitStatus g_touch_power_long_pressed = RESET;
volatile u16 g_touch_long_press_count = 0;
void touch_key_plus_press(void)
{
    if( g_system_mode == e_manual_mode )
    {
        beep_on_ms(SHORT_PRESS_BEEP_ON_TIME);
        pwm_set_motor_speed_up();
    }
    
}

void touch_key_minus_press(void)
{
    if( g_system_mode == e_manual_mode )
    {
        beep_on_ms(SHORT_PRESS_BEEP_ON_TIME);
        pwm_set_motor_speed_down();
    }
}

void touch_key_mode_press(void)
{
    beep_on_ms(SHORT_PRESS_BEEP_ON_TIME);
    
    switch( g_system_mode )
    {
        case e_auto_mode:
            g_system_mode = e_manual_mode;
            print("Switch --> Manual Mode");
            break;
            
        case e_manual_mode:
            g_system_mode = e_sleep_mode;
            print("Switch --> Sleep Mode");
            break;

        case e_sleep_mode:
            g_system_mode = e_auto_mode;
            print("Switch --> Auto Mode");
            break;

        default:
            break;
    }
}


void touch_key_power_long_press(void)
{
    beep_on_ms(LONG_PRESS_BEEP_ON_TIME);
    
    switch( g_system_mode )
    {
        case e_auto_mode:
        case e_manual_mode: 
				case e_sleep_mode:
				    co2_power_off();
            g_system_mode = e_power_off_mode;
            print("Switch --> Off Mode");
            break;
            
        case e_power_off_mode:
            g_system_mode = e_auto_mode;
						co2_power_on();
            print("Switch --> Auto Mode");
            break;
        default:
            break;
    }
}


void touch_key_gpio_isr(void)
{
    uint8_t value = GPIOC->IDR;
    print("Key isr");
		touch_key_DeInit();
		
    if( (value & TOUCH_KEY_POWER_PIN) != 0)
    {
        print("Key[ Power]");
         
        g_touch_key_power_pressed = 1;
        g_touch_long_press_count=1;
        
    }

    if( (value & TOUCH_KEY_MODE_PIN) != 0)
    {
        print("Key[ Mode]");
        print_enter();
        touch_key_mode_press();
    }

    if( (value & TOUCH_KEY_MINUS_PIN) != 0)
    {
        print("Key[----]");
        print_enter();
        touch_key_minus_press();
    }

    if( (value & TOUCH_KEY_PLUS_PIN) != 0)
    {
        print("Key[++++]");
        print_enter();
        touch_key_plus_press();
    }
    
}



void touch_key_interrupt_able(enum e_touch_key key, uint8_t able)
{
    switch( key )
    {
        case e_touch_key_plus:
            break;
            
        case e_touch_key_minus:
            break;
            
        case e_touch_key_mode:
            break;
            
        case e_touch_key_power:
            break;
            
        case e_touch_key_all:
            break;
            
        default:
            break;
    }
}


void touch_key_Init(void)
{
 
    GPIO_Init(TOUCH_KEY_PORT, TOUCH_KEY_ALL_PIN, GPIO_MODE_IN_FL_IT);
    EXTI_SetExtIntSensitivity(TOUCH_KEY_EXTI_PORT, EXTI_SENSITIVITY_RISE_ONLY);
}
void touch_key_DeInit(void)
{
    GPIO_Init(TOUCH_KEY_PORT, TOUCH_KEY_ALL_PIN, GPIO_MODE_IN_FL_NO_IT);
    //EXTI_SetExtIntSensitivity(TOUCH_KEY_EXTI_PORT, EXTI_SENSITIVITY_RISE_ONLY);
}



