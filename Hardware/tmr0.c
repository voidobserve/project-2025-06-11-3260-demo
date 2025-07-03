#include "tmr0.h"

extern volatile bit flag_is_adjust_pwm_time_comes;

/**
 * @brief ���ö�ʱ��TMR0
 */
void timer0_config(void)
{
    __EnableIRQ(TMR0_IRQn); // ʹ��timer0�ж�
    IE_EA = 1;              // ʹ�����ж�

    // ����timer0�ļ������ܣ�����һ��Ƶ��Ϊ1kHz���ж�
    TMR_ALLCON = TMR0_CNT_CLR(0x1);                               // �������ֵ
    TMR0_PRH = TMR_PERIOD_VAL_H((TIMER0_PERIOD_VAL >> 8) & 0xFF); // ����ֵ
    TMR0_PRL = TMR_PERIOD_VAL_L((TIMER0_PERIOD_VAL >> 0) & 0xFF);
    TMR0_CONH = TMR_PRD_PND(0x1) | TMR_PRD_IRQ_EN(0x1);                          // ������������ʱ�������ж�
    TMR0_CONL = TMR_SOURCE_SEL(0x7) | TMR_PRESCALE_SEL(0x7) | TMR_MODE_SEL(0x1); // ѡ��ϵͳʱ�ӣ�128��Ƶ������ģʽ
}

// TMR0�жϷ�����
void TIMR0_IRQHandler(void) interrupt TMR0_IRQn
{
    // �����ж�����IP������ɾ��
    __IRQnIPnPush(TMR0_IRQn);

    // ---------------- �û��������� -------------------

    // �����ж�
    if (TMR0_CONH & TMR_PRD_PND(0x1))
    {
        TMR0_CONH |= TMR_PRD_PND(0x1); // ���pending

        {
            static u16 cnt = 0;
            cnt++;
            if (cnt >= 100)
            {
                cnt = 0;
                flag_is_adjust_pwm_time_comes = 1;
            }
        }
    }

    // �˳��ж�����IP������ɾ��
    __IRQnIPnPop(TMR0_IRQn);
}
