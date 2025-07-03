#ifndef __TMR0_H
#define __TMR0_H

#include "include.h" // 使用芯片官方提供的头文件

#include "my_config.h" // 包含自定义的头文件

#define TIMER0_PERIOD_VAL (SYSCLK / 128 / 1000 - 1) // 周期值=系统时钟/分频/频率 - 1



void timer0_config(void);

// void tmr0_enable(void);
// void tmr0_disable(void);


#endif
