/**
  ******************************************************************************
  * @file    : system.h
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */

#ifndef __SYSTEM_H___
#define __SYSTEM_H___
#define USE_DEFAULT_CLK_2M
enum system_mode
{
    e_auto_mode = 0,
    e_manual_mode,
    e_sleep_mode,
    e_power_off_mode
};

#ifndef SYSTEM_C
extern enum system_mode g_system_mode;
#endif

#endif
