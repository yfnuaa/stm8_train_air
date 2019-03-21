/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */
#include "stm8s.h"
#define  SYSTEM_C   1
#include "system.h"
#include "pm25.h"
#include "co2.h"
#include "touch_key.h"
#include "beep.h"
#include "lcd.h"
#include "pwm.h"
#include "adc.h"
#include "debug.h"
#include "timer1.h"
#include "timer4.h"
#include "utility.h"
#include "beep.h"

u16 g_air_quality = 0;
volatile enum  system_mode g_system_mode = e_dummy_mode;
volatile enum  system_mode g_last_system_mode = e_dummy_mode;

volatile int8_t g_need_operate_after_mode_switch = 0;   //ģʽ�л�ʱ���Ƿ���Ҫ ���������л�ģʽʱ������

volatile int8_t g_system_need_detect = 0;               //��Ҫ ��⴫����


volatile int8_t g_system_need_check_alarm = 1;         // �Ƿ�Ҫȥ check �������������Ӷ� ȥ ���� ����
                                                       // �����ڱ�����30 ������ ����ȥ check �Ƿ�Ҫ����
                                                       /* ϵͳ��ʼ�� Ϊ 1�� ��Ҫȥ check */

#define SYSTEM_SENSOR_DETECT_5_MINUTES_INTERVAL         500//30000 // (5min * 60s * 100) 10ms
#define SYSTEM_SENSOR_DETECT_15_MINUTES_INTERVAL        500//90000 // (15min * 60s * 100) 10ms
volatile int32_t g_system_detect_timer_count = 0;                   //ϵͳ ��� ��ʱ��

#define SYSTEM_BEEP_ALARM_1_MINUTES_INTERVAL            6000 // (1min * 60s * 100) 10ms
#define SYSTEM_BEEP_ALARM_INTERVAL      SYSTEM_BEEP_ALARM_1_MINUTES_INTERVAL
volatile int32_t g_system_beep_alarm_timer_count = 0;        //ϵͳ ���� ��ʱ�� ��ʱ 1 ����


#define SYSTEM_CHECK_ALARM_30_MINUTES_INTERVAL          180000 // (30min * 60s * 100) 10ms
#define SYSTEM_CHECK_ALARM_INTERVAL     SYSTEM_CHECK_ALARM_30_MINUTES_INTERVAL
volatile int32_t g_system_check_alarm_timer_count = 0;   //ϵͳ ����Ƿ� Ҫ ���� ��ʱ���� ��ʱ 30 ����


//TODO
void start_motor_and_diaplay_quality_according_detect_value(void)
{
    #if 0//TODO
    if( g_pwm_motor_speed_step == e_speed_off )
    {
        if(g_co2_density_mg_m3<= )
        {
            pwm_set_motor_speed(e_speed_off);
        }
        else if(g_co2_density_mg_m3 <= )
        {
            pwm_set_motor_speed(e_speed_low);
        }
        else if(g_co2_density_mg_m3 <= )
        {
            pwm_set_motor_speed(e_speed_middle);
        }        
        else if(g_co2_density_mg_m3 <= )
        {
            pwm_set_motor_speed(e_speed_high);
        }
        else  
        {
            pwm_set_motor_speed(e_speed_high+10);
        }
    }

    #endif
    //���û����תʱ�����ж� PM2.5
    //if( g_pwm_motor_speed_step == e_speed_off )
    {
        if(g_pm25_dust_density_ug_m3 <= e_pm25_level_1)
        {
            pwm_set_motor_speed(e_speed_off);
            lcd_display_air_quality(  0 );
        }
        else if(g_pm25_dust_density_ug_m3 <= e_pm25_level_2)
        {
            pwm_set_motor_speed(e_speed_low);
            lcd_display_air_quality(  1 );
        }
        else if(g_pm25_dust_density_ug_m3 <= e_pm25_level_3)
        {
            pwm_set_motor_speed(e_speed_middle);
            lcd_display_air_quality(  2 );
        }        
        else if(g_pm25_dust_density_ug_m3 <= e_pm25_level_4)
        {
            pwm_set_motor_speed(e_speed_high);
            lcd_display_air_quality(  2 );
        }
        else  //> e_pm25_bad
        {
            pwm_set_motor_speed(e_speed_high+10);
            lcd_display_air_quality(  2 );
        }
    }
}


void alarm_config_after_detect(void)
{  
    //��û�б��� && ��Ҫ Check �Ƿ���Ҫ����ʱ
    if( (g_system_need_check_alarm == 1) && (g_need_beep_alarm == 0) )
    {  
        print("Main --> Check Alarm");
        
        /* �������ʹ�� LCD ����ʾֵ ��Ϊ �ж����� */
        if( (g_co2_density_mg_m3 > 600) || (g_pm25_dust_density_ug_m3 > 200) )   //TODO
        {   
            /* Sleep Mode �£�ֻ�� �� һ�� */
            if( g_system_mode == e_sleep_mode )
            {
                beep_on_ms(200);
            }
            else    //
            {
                g_need_beep_alarm = 1;
            }

            //�� Auto Mode �� �Զ� ��������

        }
        else
        {
            if( g_system_mode != e_sleep_mode )     //�� Sleep mode �£����ر� ����
            {
                //�رշ���
                pwm_set_motor_speed(e_speed_off);
            }
            
        }

        /* �������� beep ��ʼ ���� */
        /* ��ʼ ��ʱ 1 ���� */
        /* ��ʼ ��ʱ 30 ���� ���� ȥ check  */
        if( g_need_beep_alarm == 1 )
        {
            g_system_beep_alarm_timer_count = SYSTEM_BEEP_ALARM_INTERVAL;
            g_system_check_alarm_timer_count = SYSTEM_CHECK_ALARM_INTERVAL;
            g_system_need_check_alarm = 0;
        }  
    }

    if( g_need_beep_alarm == 1 )    
    {
        //�ڱ���ʱ��ȥ check ���� 1 �����Ƿ� ��
        if( g_system_beep_alarm_timer_count == 0 )
        {
            g_need_beep_alarm = 0;
        }
    }

    //�ڲ���Ҫ Check �Ƿ���Ҫ����ʱ���жϣ����� �ȴ� 30 ���� ��ʱ����
    if( g_system_need_check_alarm == 0 ) 
    {   
        if( g_system_check_alarm_timer_count == 0 )
        {
            g_system_need_check_alarm = 1;
        }
    }
}



void main()
{
    uint8_t i = 0;
    
    #ifndef USE_DEFAULT_CLK_2M
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);                           // default is 8 div 
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);    /*CLK_PRESCALER_CPUDIV128*/  // set system clock 2 div freq //system 8M speed running 
    #endif
    
    /* ================================  ϵͳ�ϵ� BEGIN ================================= 
        1.	�ϵ��ʼ��
        2.	������һ�£�����ʾ���������ʾһ��Ĭ�ϵ�ֵ��
        3.	ϵͳ���� ��power_off mode��ģʽ���ȴ����� power ������
        4.	������
    */	

    //TODO ��ʼ�� ˳��
    touch_key_Init(); 
    enableInterrupts();
    timer1_init(); 
    beep_init();
    uart1_init();   
    print("POooooooooooooooooxxxxxxxxxxxxxxxxxxyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy"); 

    //LCD
    lcd_init();
    
    lcd_back_light_on();
    
    //beep_on_ms(400);
    
    lcd_display_power_on_mode(); 
    pwm_init();
    
    pm25_init();
    co2_init();

    g_system_tick = 150;
    while( g_system_tick != 0 );
    lcd_back_light_off();
    
    //TODO
    
    /* �ϵ����� Power Off Mode */
    g_system_mode = e_power_off_mode;
    g_last_system_mode = e_dummy_mode;
    g_system_need_detect = 0;
    /* ================================  ϵͳ�ϵ� END ================================= */
    #if 0  
    pm25_power_on();co2_power_on();
    while(1)
    {//co2_power_on(); pm25_power_on();print("on");
        pm25_detect_process(); 
        delay_ms(1000); 
        ///co2_power_off(); pm25_power_off(); print("off");
         co2_detect_process();
        //pm25_led_off();pm25_power_off();
         delay_ms(1000);     
        //  pm25_led_on();pm25_power_on();
        //  delay_ms(5000);
    }
    #endif 

    while(1)
    {           
        /* ������ ��� */
        if(  (g_system_detect_timer_count == 0) && (g_system_need_detect == 1) )
        {
            if( g_system_mode == e_sleep_mode )
            {
                g_system_detect_timer_count = SYSTEM_SENSOR_DETECT_15_MINUTES_INTERVAL;
            }
            else
            {
                g_system_detect_timer_count = SYSTEM_SENSOR_DETECT_5_MINUTES_INTERVAL;
            }
               
            print("Main --> Detect");
            {
                static bool temp_flag = 0;
                temp_flag = !temp_flag;
                
                if(temp_flag)
                {
                    pm25_detect_process(); 
                    lcd_display_pm25(g_pm25_dust_density_ug_m3);
                    //pm25_detect_process(); 
                }
                else
                {          
                    co2_detect_process();
                    lcd_display_co2(g_co2_density_mg_m3);
                    //delay_ms(20);
                    //co2_detect_process();
                }
            }
            
            if( g_system_mode == e_auto_mode )
            {
                start_motor_and_diaplay_quality_according_detect_value(); 
            }
            
            //g_air_quality = 1;  //������ʾ LCD �ϵ� ��������
        }
        
        /* ÿ�μ���꣬�͸��� LCD */
        
        
        /* TODO  */

        /* ���ݼ��ֵ ���� ���� */
        switch( g_system_mode )
        {
            case e_auto_mode:
            case e_manual_mode:
            case e_sleep_mode:
               // alarm_config_after_detect();
            break;
                
            default:
            break;
        }

        //ģʽ�л� ����
        if( g_system_mode != g_last_system_mode )
        {
            g_need_operate_after_mode_switch = 1;
            g_last_system_mode = g_system_mode;
        }
		if(!g_need_operate_after_mode_switch)
			continue;
        switch(g_last_system_mode)
        {   
            // power_on mode
            case e_power_on_mode:
                if( g_need_operate_after_mode_switch == 1 )
                {   
                    g_need_operate_after_mode_switch = 0;
                    g_system_mode = e_auto_mode;
                    g_system_detect_timer_count = 0;    //�ϵ� �� ��Ҫ ��ʼ���

                    g_pwm_motor_speed_step = 0;
                    
                    print("Main --> Power_On Mode");
                    /* 
                        1.	��ʾ LCD ������ʾ����ʾ����Ч����
                        2.	PM2.5�� CO2�������ϵ�
                        3.	������
                        4.	ϵͳ���롾auto mode��
                    */
                    
                    co2_power_on(); 
                    pm25_power_on();
    
                    pwm_init();
                    /* 1. */
                    
                    //lcd_init();
                    //lcd_on();
                    lcd_back_light_on();
                    lcd_display_switch_on();
                    ht1621_light_up_always_on_seg();
                    
                    /* 3. */
                    /* 4. */
                    g_system_need_detect = 1;
                }
                break;
                
            //Auto mode
            case e_auto_mode: 
                if( g_need_operate_after_mode_switch == 1 )
                {
                    g_need_operate_after_mode_switch = 0;
                    print("Main --> Auto Mode");
                    lcd_light_mode_auto();
                    /* 
                        1.	ϵͳÿ�� 5 ���ӣ����һ��PM2.5 �� CO2
                        2.	���ݼ������ݵ��ڷ����ٶ�
                        3.	�����⵽ PM2.5 or CO2 �쳣����������������. ��. ��������. ��. ��������. ��. ��������
                             ���������� 1 ���Ӻ��Զ�ȡ������������ڼ����κΰ�������ȡ��������������
                            ��һ�α������Ϊ30���Ӻ� 
                        4.	Goto  1
                    */
                    g_system_need_detect = 1;
                    g_system_need_check_alarm = 1;
                }
                break;

            //Manual Mode
            case e_manual_mode:
                if( g_need_operate_after_mode_switch == 1 )
                {
                    g_need_operate_after_mode_switch = 0;
                    print("Main --> Manual Mode");
                    lcd_light_mode_manual();
                    /* 
                        1.	����Manual Mode �󣬲��ı䵱ǰ���ȵ�״̬������ǰ���������еģ��������С�
                        2.	ϵͳÿ�� 5 ���ӣ����һ��PM2.5 �� CO2��
                        3.	�����⵽ PM2.5 or CO2 �쳣����������������. ��. ��������. ��. ��������. ��. ��������
                            ���������� 1 ���Ӻ��Զ�ȡ������������ڼ����κΰ�������ȡ��������������
                            ��һ�α������Ϊ30���Ӻ�
                        4.	��������Ӽ������л���Ӧ����ת�١�
                        5.	Goto 2
                    */
                    
                    g_system_need_check_alarm = 1;   
                }
                break;
                
            //Sleep Mode
            case e_sleep_mode:
                if( g_need_operate_after_mode_switch == 1 )
                {   
                    g_need_operate_after_mode_switch = 0;
                    print("Main --> Sleep Mode");
                    lcd_light_mode_sleep();
                    /* 
                        1.	����Sleep Mode �󣬷����ٶ�����١�
                        2.	ϵͳÿ�� 15 ���ӣ����һ��PM2.5 �� CO2��
                        3.	�����⵽ PM2.5 or CO2 �쳣����������������һ�Σ�,��һ�α������Ϊ30���Ӻ�
                        4.	������յ��ػ�����ػ�����power off mode
                        5.	Goto 2
                    */
                    
                    pwm_set_motor_speed(e_speed_low);

                    //˯��ģʽ�£��Ƿ���Ҫ �೤ʱ��� ���� power off mode   //TODO
                }
                break;

            //Power Off Mode
            case e_power_off_mode: 
                if( g_need_operate_after_mode_switch == 1 )
                {
                    g_need_operate_after_mode_switch = 0;
                    print("Main --> Power_Off Mode");
                    /* 
                        ��ģʽֻ��Ӧ ��Power������
                        1.	�رշ���
                        2.	�رմ�������Դ
                        3.	�ر�LCD ��Ļ���� ����
                        4.	�ȴ� ��Power��������������
                    */
                    co2_power_off();  
                    pm25_power_off();
                    adc1_reset();
                    lcd_back_light_off();
                    //lcd_off();
                    beep_derect_off();
                    pwm_close();
                       
                    g_system_need_check_alarm = 0;
                        
                    g_system_detect_timer_count = 0;
                    g_system_beep_alarm_timer_count = 0;
                    g_system_check_alarm_timer_count = 0;   
                }  
                break;

            default:
                break;
                    
        } 
    }
}



