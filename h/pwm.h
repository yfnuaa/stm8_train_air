#ifndef ___PWM_H____
#define ___PWM_H____


enum pwm_motor_speed_step
{
    e_speed_off      = 0,
    e_speed_low      = 24,  // 1.2v is supposed the 600 speed 
    e_speed_middle   = 38,  // 1.9v is supposed the 950 speed 
    e_speed_high     = 52,  // 2.7v is the hightest speed allowed, 1200r/min
    e_speed_full     = 80   // 2.7v is the hightest speed allowed, 1200r/min
}; 

#ifndef PWM_C
extern enum pwm_motor_speed_step g_pwm_motor_speed_step;
#endif

void pwm_init(void);
void pwm_set_freq(u16 freq);
void pwm_set_duty(u8 duty);//0-100
void pwm_stop(void);
void pwm_set_motor_speed_up(void);
void pwm_set_motor_speed_down(void);
void pwm_set_motor_speed(u8 step);
 
#endif