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


#define TOUCH_KEY_PORT          GPIOC
#define TOUCH_KEY_DDR           PA_DDR        
#define TOUCH_KEY_CR1           PA_CR1
#define TOUCH_KEY_CR2           PA_CR2


#define TOUCH_KEY_PLUS          //+
#define TOUCH_KEY_MINUS         //=
#define TOUCH_KEY_MODE          //mode
#define TOUCH_KEY_POWER         //power


void touch_key_plus_short_press()
{
}

void touch_key_minus_short_press()
{
}

void touch_key_mode_short_press()
{
}

void touch_key_power_short_press()
{
}


void touch_key_power_long_press()
{
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
   // GPIO_Init(GPIOC,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7,GPIO_MODE_OUT_OD_LOW_FAST);

}



