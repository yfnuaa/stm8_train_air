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
#define RUIZHU_TEST		0	//���� 0 or 1 To disable or enable

enum system_mode
{
	e_power_on_mode = 2,
    e_auto_mode,
    e_manual_mode,
    e_sleep_mode,
    e_power_off_mode,
    e_dummy_mode
};

#define SYSTEM_ALARM_STEP_HOLD_TIME		15 //һ�α������ٹ� ����ʱ�� �ټ�⣬�ٱ���

#ifndef SYSTEM_C
extern volatile enum system_mode g_system_mode;
extern volatile int8_t g_need_operate_after_mode_switch;   //ģʽ�л�ʱ���Ƿ���Ҫ ���������л�ģʽʱ������

extern volatile int8_t g_system_need_detect;           //��Ҫ ��⴫����

extern volatile int8_t g_system_need_check_alarm;  // �Ƿ�Ҫȥ check �������������Ӷ� ȥ ���� ����
                                                       // �����ڱ�����30 ������ ����ȥ check �Ƿ�Ҫ����
                                                       /* ϵͳ��ʼ�� Ϊ 1�� ��Ҫȥ check */

extern volatile uint32_t g_system_detect_timer_count;			 //ϵͳ ��� ��ʱ��
extern volatile int32_t g_system_beep_alarm_timer_count;        //ϵͳ ���� ��ʱ�� ��ʱ 1 ����
extern volatile int32_t g_system_check_alarm_timer_count;         //ϵͳ ����Ƿ� Ҫ ���� ��ʱ���� ��ʱ 30 ����
#endif 

#endif
