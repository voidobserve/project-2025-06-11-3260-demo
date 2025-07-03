#ifndef __TMR1_H
#define __TMR1_H

#include "include.h"   // ʹ��оƬ�ٷ��ṩ��ͷ�ļ�
#include "my_config.h" // �����Զ����ͷ�ļ�

#define TIMER1_PEROID_VAL (SYSCLK / 1 / 62388 - 1) // ����ֵ=ϵͳʱ��/��Ƶ/Ƶ�� - 1  

void timer1_pwm_config(void);
void timer1_pwm_enable(void);
void timer1_pwm_disable(void);
 void timer1_set_pwm_duty_val(u8 pwm_duty_val);

#endif
