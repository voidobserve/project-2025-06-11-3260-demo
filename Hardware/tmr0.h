#ifndef __TMR0_H
#define __TMR0_H

#include "include.h" // ʹ��оƬ�ٷ��ṩ��ͷ�ļ�

#include "my_config.h" // �����Զ����ͷ�ļ�

#define TIMER0_PERIOD_VAL (SYSCLK / 128 / 1000 - 1) // ����ֵ=ϵͳʱ��/��Ƶ/Ƶ�� - 1



void timer0_config(void);

// void tmr0_enable(void);
// void tmr0_disable(void);


#endif
