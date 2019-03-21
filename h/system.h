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

#include "stm8s.h"
#define SYS_VERSION 1030
//#define USE_DEFAULT_CLK_2M
#define DEBUG   		1
#define RUIZHU_TEST		0	//设置 0 or 1 To disable or enable

enum system_mode
{
	e_power_on_mode = 2,
    e_auto_mode,
    e_manual_mode,
    e_sleep_mode,
    e_power_off_mode,
    e_dummy_mode
};

#define SYSTEM_ALARM_STEP_HOLD_TIME		15 //一次报警后，再过 多少时间 再检测，再报警

#ifndef SYSTEM_C
extern volatile enum system_mode g_system_mode;
extern volatile int8_t g_need_operate_after_mode_switch;   //模式切换时，是否需要 做操作，切换模式时，设置

extern volatile int8_t g_system_need_detect;           //需要 检测传感器

extern volatile int8_t g_system_need_check_alarm;  // 是否要去 check 空气的质量，从而 去 设置 报警
                                                       // 用于在报警后，30 分钟内 不用去 check 是否要报警
                                                       /* 系统初始化 为 1， 需要去 check */

extern volatile uint32_t g_system_detect_timer_count;			 //系统 检测 定时器
extern volatile int32_t g_system_beep_alarm_timer_count;        //系统 报警 定时器 定时 1 分钟
extern volatile int32_t g_system_check_alarm_timer_count;         //系统 检测是否 要 报警 定时器， 定时 30 分钟
#endif 

#endif
