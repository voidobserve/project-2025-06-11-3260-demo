// ��ʱ��TMR2������ͷ�ļ�
#ifndef __TMR2_H
#define __TMR2_H

#include "include.h" // ����оƬ�ٷ��ṩ��ͷ�ļ�
#include "my_config.h" // �����Զ����ͷ�ļ�

// extern volatile unsigned char tmr2_flag; // tmr2�жϷ������л���λ�ı�־λ
extern volatile u32 tmr2_cnt; // ��ʱ��TMR2�ļ���ֵ��ÿ�����жϷ������л��һ��

void tmr2_config(void); // ��ʱ��tmr2�ĳ�ʼ��
void tmr2_pwm_config(void); 

void tmr2_enable(void); // ������ʱ��TMR2����ʼ��ʱ
void tmr2_disable(void); // �رն�ʱ��TMR2����ռ���ֵ

#endif
