// 定时器TMR2的驱动头文件
#ifndef __TMR2_H
#define __TMR2_H

#include "include.h" // 包含芯片官方提供的头文件
#include "my_config.h" // 包含自定义的头文件

// extern volatile unsigned char tmr2_flag; // tmr2中断服务函数中会置位的标志位
extern volatile u32 tmr2_cnt; // 定时器TMR2的计数值（每次在中断服务函数中会加一）

void tmr2_config(void); // 定时器tmr2的初始化
void tmr2_pwm_config(void); 

void tmr2_enable(void); // 开启定时器TMR2，开始计时
void tmr2_disable(void); // 关闭定时器TMR2，清空计数值

#endif
