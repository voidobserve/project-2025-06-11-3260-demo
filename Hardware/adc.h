#ifndef __ADC_H
#define __ADC_H

#include "include.h"   // ʹ��оƬ�ٷ��ṩ��ͷ�ļ�
#include "my_config.h" // �����Զ����ͷ�ļ�

// extern volatile u16 adc_val;       // adcֵ��0~4095

void adc_config(void);
// void adc_showval(void);
void adc_sel_pin(u8 adc_pin);
u16 adc_getval(void);

#endif
