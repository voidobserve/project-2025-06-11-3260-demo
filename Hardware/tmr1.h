#ifndef __TMR1_H
#define __TMR1_H

#include "include.h"   // 使用芯片官方提供的头文件
#include "my_config.h" // 包含自定义的头文件

#define TIMER1_PEROID_VAL (SYSCLK / 1 / 62388 - 1) // 周期值=系统时钟/分频/频率 - 1  

void timer1_pwm_config(void);
void timer1_pwm_enable(void);
void timer1_pwm_disable(void);
 void timer1_set_pwm_duty_val(u8 pwm_duty_val);

#endif
