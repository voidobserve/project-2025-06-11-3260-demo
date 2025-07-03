#include "tmr1.h"

void timer1_pwm_config(void)
{
    //  ���� P25 Ϊ PWM ����˿�
    P2_MD1 &= ~GPIO_P25_MODE_SEL(0x3);
    P2_MD1 |= GPIO_P25_MODE_SEL(0x1); // ���ģʽ
    FOUT_S25 = GPIO_FOUT_TMR1_PWMOUT;

    TMR_ALLCON = TMR1_CNT_CLR(0x1);                               // �������ֵ
    TMR1_PRH = TMR_PERIOD_VAL_H((TIMER1_PEROID_VAL >> 8) & 0xFF); // ����ֵ
    TMR1_PRL = TMR_PERIOD_VAL_L((TIMER1_PEROID_VAL >> 0) & 0xFF);
    TMR1_PWMH = TMR_PWM_VAL_H((((u32)TIMER1_PEROID_VAL * 0 / 100) >> 8) & 0xFF); // ռ�ձ�����ֵ
    TMR1_PWML = TMR_PWM_VAL_L((((u32)TIMER1_PEROID_VAL * 0 / 100) >> 0) & 0xFF);
    TMR1_CONH = TMR_PRD_PND(0x1) | TMR_PRD_IRQ_EN(0x1);  // ʹ�ܼ����ж�
    TMR1_CONL = TMR_SOURCE_SEL(0x7) | TMR_MODE_SEL(0x2); // ѡ��ϵͳʱ�ӣ�ʱ��Դ����Ƶ��PWMģʽ
}

void timer1_pwm_enable(void)
{
    FOUT_S25 = GPIO_FOUT_TMR1_PWMOUT; // ���ָ��ù������
    TMR1_CONL |= (0x02 << 0);         // PWMģʽ
}

// �ر�pwm����������͵�ƽ
void timer1_pwm_disable(void)
{
    TMR1_CONL &= ~(0x03 << 0);    // ��ʹ�ܶ�ʱ������
    FOUT_S25 = GPIO_FOUT_AF_FUNC; // ���ָ��ù������
    P25 = 0;
}

void timer1_set_pwm_duty_val(u8 pwm_duty_val)
{
    TMR1_PWMH = TMR_PWM_VAL_H((((u32)TIMER1_PEROID_VAL * pwm_duty_val / 255) >> 8) & 0xFF); // ռ�ձ�����ֵ
    TMR1_PWML = TMR_PWM_VAL_L((((u32)TIMER1_PEROID_VAL * pwm_duty_val / 255) >> 0) & 0xFF);
}
