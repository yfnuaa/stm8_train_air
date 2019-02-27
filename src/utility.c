/**
  ******************************************************************************
  * @file    : utility.c
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 
#include "stm8s.h"
#include "system.h"
void delay_s(uint8_t second)
{
}


void delay_280us(void)
{
    #ifdef USE_DEFAULT_CLK_2M
    int i=185;
    #else
    int i = 185*8;
    #endif
    while(--i);
}


void delay_40us(void)
{
    #ifdef USE_DEFAULT_CLK_2M
    int i=25;
    #else
    int i = 25*8;
    #endif
    while(--i);
}



