#ifndef ___PWM_H____
#define ___PWM_H____


enum pwm_motor_speed_step
{
	e_speed_off = 0,
    e_speed_low = 30,
    e_speed_middle = 60,
    e_speed_high = 90,
    e_speed_full = 100
};

#ifndef PWM_C
extern enum pwm_motor_speed_step g_pwm_motor_speed_step;
#endif

void pwm_init(void);
void pwm_set_freq(u8 frqKZH);
void pwm_set_duty(u8 duty);//0-100
void pwm_stop(void);
void pwm_set_motor_speed_up(void);
void pwm_set_motor_speed_down(void);
void pwm_set_motor_speed(enum pwm_motor_speed_step step);
 
#endif