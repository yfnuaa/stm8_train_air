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
#include "timer1.h"

#if 0
void delay_min(uint8_t min)
{
    uint32_t start = g_system_10ms_tick;
    uint32_t delt = min*600;
    uint32_t end = start + delt;
    enum system_mode old_mode = g_system_mode;
    enum system_mode new_mode = g_system_mode; 
        
    if( end < start )
    {
        end = delt - (2^32-1 - start);
    }
    
    while( end > g_system_10ms_tick )
    {
        new_mode = g_system_mode;

        //模式改变时，退出 长延时
        if( new_mode == old_mode )
        {
            break;
        }
    }
}


void delay_s(uint8_t second)
{
    uint32_t start = g_system_10ms_tick;
    uint32_t delt = second*100;
    uint32_t end = start + delt;
    
    #if 0
    if( end > start )
    {
        while( end > g_system_10ms_tick );
    }
    else
    {
        end = delt - (2^32-1 - start);
        while( end > g_system_10ms_tick );
    }
    #endif
    
    if( end < start )
    {
        end = delt - (2^32-1 - start);
    }
    
    while( end > g_system_10ms_tick );
}
#endif

void delay_280us(void)
{
    #ifdef USE_DEFAULT_CLK_2M
    int i=185;H
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



