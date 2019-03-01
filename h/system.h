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
//#define USE_DEFAULT_CLK_2M    //open this to enable system to 2M default clock
#define DEBUG   1               //enable this will send uart message 
#define RUIZHU_TEST             //this is use for ruizhu test ,only funciton is adjust fan speed   0% 5% 10% 15% ... 80%
enum system_mode
{
    e_auto_mode = 0,
    e_manual_mode  ,
    e_sleep_mode   ,
    e_power_off_mode
};
 
extern volatile enum system_mode g_system_mode;
#endif
