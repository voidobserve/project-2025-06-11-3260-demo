// ��ʱ��TMR3������Դ�ļ�
#include "tmr3.h"

#define TMR3_CNT_TIME (SYSCLK / 32 / 1000 - 1) // �������� 1KHz��1ms����һ���ж�

volatile u32 tmr3_cnt = 0; // ��ʱ��TMR3�ļ���ֵ��ÿ�����жϷ������л��һ��
// volatile bit tmr3_flag = 0; // tmr3�жϷ������л���λ�ı�־λ

// ��ʱ��TMR3�ĳ�ʼ������ʼ����ɺ�Ĭ���ǹرյģ�
void tmr3_config(void)
{
    __SetIRQnIP(TMR3_IRQn, TMR3_IQn_CFG); // �����ж����ȼ���TMR3��

    TMR3_CONL &= ~TMR_PRESCALE_SEL(0x07); // ���TMR3��Ԥ��Ƶ���üĴ���
    TMR3_CONL |= TMR_PRESCALE_SEL(0x05);  // ���÷�Ƶϵ��
    TMR3_CONL &= ~TMR_MODE_SEL(0x03);     // ���TMR3��ģʽ���üĴ���
    TMR3_CONL |= TMR_MODE_SEL(0x01);      // ����TMR3��ģʽΪ������ģʽ������ϵͳʱ�ӵ�������м���

    TMR3_CONH &= ~TMR_PRD_PND(0x01); // ���TMR3�ļ�����־λ����ʾδ��ɼ���
    TMR3_CONH |= TMR_PRD_IRQ_EN(1);  // ʹ��TMR3�ļ����ж�

    __DisableIRQ(TMR3_IRQn); // �����ж�

    // ����TMR3�ļ�������
    TMR3_PRL = (unsigned char)(TMR3_CNT_TIME % 255);
    TMR3_PRH = (unsigned char)(TMR3_CNT_TIME / 255);

    // ���TMR3�ļ���ֵ
    // TMR3_CNTL = 0;
    // TMR3_CNTH = 0;
    TMR_ALLCON = TMR3_CNT_CLR(0x1); // �������ֵ(д1����)

    TMR3_CONL &= ~(TMR_SOURCE_SEL(0x07)); // ���TMR3��ʱ��Դ���üĴ���
    // TMR3_CONL |= TMR_SOURCE_SEL(0x07); // ����TMR3��ʱ��Դ��ʹ��ϵͳʱ��
    TMR3_CONL |= TMR_SOURCE_SEL(0x05); // ����TMR3��ʱ��Դ�������κ�ʱ��
    // __EnableIRQ(TMR3_IRQn);			   // ʹ���ж�
    IE_EA = 1; // �����ж�
}

// ������ʱ��TMR3����ʼ��ʱ
void tmr3_enable(void)
{
    // ���¸�TMR3����ʱ��
    TMR3_CONL &= ~(TMR_SOURCE_SEL(0x07)); // �����ʱ����ʱ��Դ���üĴ���
    TMR3_CONL |= TMR_SOURCE_SEL(0x06);    // ���ö�ʱ����ʱ��Դ��ʹ��ϵͳʱ�� 

    __EnableIRQ(TMR3_IRQn); // ʹ���ж�
    IE_EA = 1;              // �����ж�
}

// �رն�ʱ������ռ���ֵ
void tmr3_disable(void)
{
    // ������ʱ���ṩʱ�ӣ�����ֹͣ����
    TMR3_CONL &= ~(TMR_SOURCE_SEL(0x07)); // �����ʱ����ʱ��Դ���üĴ���
    TMR3_CONL |= TMR_SOURCE_SEL(0x05);    // ���ö�ʱ����ʱ��Դ�������κ�ʱ��

    // �����ʱ���ļ���ֵ
    // TMR3_CNTL = 0;
    // TMR3_CNTH = 0;
     TMR_ALLCON = TMR3_CNT_CLR(0x1); // �������ֵ(д1����)

    __DisableIRQ(TMR3_IRQn); // �ر��жϣ���ʹ���жϣ�
}

// TMR3�жϷ�����
void TIMR3_IRQHandler(void) interrupt TMR3_IRQn
{
#if 1 // ��ʱ���Ķ�ʱ�ж�
    // �����ж�����IP������ɾ��
    __IRQnIPnPush(TMR3_IRQn);

    // ---------------- �û��������� -------------------
    // �����ж�
    if (TMR3_CONH & TMR_PRD_PND(0x1))
    {
        TMR3_CONH |= TMR_PRD_PND(0x1); // ���pending

        tmr3_cnt++;
    }

    // �˳��ж�����IP������ɾ��
    __IRQnIPnPop(TMR3_IRQn);
#endif

#if 0 // ��ʱ�������벶���ж�

    // �����ж�����IP������ɾ��
    __IRQnIPnPush(TMR3_IRQn);

    // ---------------- �û��������� -------------------

    if (TMR3_CONH & TMR_CAP_PND(0x1))
    {
        TMR3_CONH |= TMR_CAP_PND(0x1); // ���pending

        tmr3_cap_val    = ((TMR3_PWMH << 8) | TMR3_PWML); // ��ȡ�����ֵ
        tmr3_cnt++; // �������ֵ��һ

        TMR_ALLCON = TMR3_CNT_CLR(0x1); // �������ֵ
    }

    // �˳��ж�����IP������ɾ��
    __IRQnIPnPop(TMR3_IRQn);

#endif
}
