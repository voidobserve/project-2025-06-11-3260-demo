#ifndef __ADC_H
#define __ADC_H

#include "include.h"   // 使用芯片官方提供的头文件
#include "my_config.h" // 包含自定义的头文件

// extern volatile u16 adc_val;       // adc值，0~4095

void adc_config(void);
// void adc_showval(void);
void adc_sel_pin(u8 adc_pin);
u16 adc_getval(void);

#endif
