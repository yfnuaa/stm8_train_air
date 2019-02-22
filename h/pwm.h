#ifndef ___PWM_H____
#define ___PWM_H____


void pwm_init(void);
void pwm_set_freq(u8 frqKZH);
void pwm_set_duty(u8 duty);//0-100
void pwm_stop(void);
 
#endif