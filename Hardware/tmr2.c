// ��ʱ��TMR2������Դ�ļ�
#include "tmr2.h"

#define TMR2_CNT_TIME (SYSCLK / 32 / 1000 - 1) // �������� 1KHz��1ms����һ���ж�

// volatile unsigned char tmr2_flag = 0; // tmr2�жϷ������л���λ�ı�־λ
volatile u32 tmr2_cnt = 0; // ��ʱ��TMR2�ļ���ֵ��ÿ�����жϷ������л��һ��

/**
 * @brief ���ö�ʱ��TMR2
 */
void tmr2_config(void)
{
    // ���ö�ʱ����������¼RF���յ��ĸߵ�ƽ����ʱ��
    __SetIRQnIP(TMR2_IRQn, TMR2_IQn_CFG); // �����ж����ȼ���TMR2��

    TMR2_CONL &= ~TMR_PRESCALE_SEL(0x03); // ���TMR2��Ԥ��Ƶ���üĴ���
    TMR2_CONL |= TMR_PRESCALE_SEL(0x05);  // ���÷�Ƶϵ��
    TMR2_CONL &= ~TMR_MODE_SEL(0x03);     // ���TMR2��ģʽ���üĴ���
    TMR2_CONL |= TMR_MODE_SEL(0x01);      // ����TMR2��ģʽΪ������ģʽ������ϵͳʱ�ӵ�������м���

    TMR2_CONH &= ~TMR_PRD_PND(0x01); // ���TMR2�ļ�����־λ����ʾδ��ɼ���
    TMR2_CONH |= TMR_PRD_IRQ_EN(1);  // ʹ��TMR2�ļ����ж�

    // ����TMR2�ļ�������
    TMR2_PRL = (unsigned char)(TMR2_CNT_TIME % 255);
    TMR2_PRH = (unsigned char)(TMR2_CNT_TIME / 255);

    // ���TMR2�ļ���ֵ
    // TMR2_CNTL = 0;
    // TMR2_CNTH = 0;
    TMR_ALLCON = TMR2_CNT_CLR(0x1); // �������ֵ

    TMR2_CONL &= ~(TMR_SOURCE_SEL(0x07)); // ���TMR2��ʱ��Դ���üĴ���
    // TMR2_CONL |= TMR_SOURCE_SEL(0x07); // ����TMR2��ʱ��Դ��ʹ��ϵͳʱ��
    TMR2_CONL |= TMR_SOURCE_SEL(0x05); // ����TMR2��ʱ��Դ�������κ�ʱ��
                                       // __EnableIRQ(TMR2_IRQn);			   // ʹ���ж�

    __DisableIRQ(TMR2_IRQn); // �����ж�
    IE_EA = 1;               // �����ж�
}

/**
 * @brief ������ʱ��TMR2����ʼ��ʱ
 */
void tmr2_enable(void)
{
    // ���¸�TMR2����ʱ��
    TMR2_CONL &= ~(TMR_SOURCE_SEL(0x07)); // �����ʱ����ʱ��Դ���üĴ���
    TMR2_CONL |= TMR_SOURCE_SEL(0x06);    // ���ö�ʱ����ʱ��Դ��ʹ��ϵͳʱ��

    __EnableIRQ(TMR2_IRQn); // ʹ���ж�
    IE_EA = 1;              // �����ж�
}

/**
 * @brief �رն�ʱ��2����ռ���ֵ
 */
void tmr2_disable(void)
{
    // ������ʱ���ṩʱ�ӣ�����ֹͣ����
    TMR2_CONL &= ~(TMR_SOURCE_SEL(0x07)); // �����ʱ����ʱ��Դ���üĴ���
    TMR2_CONL |= TMR_SOURCE_SEL(0x05);    // ���ö�ʱ����ʱ��Դ�������κ�ʱ��

    // �����ʱ���ļ���ֵ
    // TMR2_CNTL = 0;
    // TMR2_CNTH = 0;
    TMR_ALLCON = TMR2_CNT_CLR(0x1); // �������ֵ

    __DisableIRQ(TMR2_IRQn); // �ر��жϣ���ʹ���жϣ�
}

// ��ʱ�����ó�PWM���ģʽ�����øú���ǰ��Ҫ�Ƚ���Ӧ��IO���õ���ʱ����PWM����ϣ�
void tmr2_pwm_config(void)
{
    //  ����P24Ϊtimer2��PWM����˿�
    P2_MD1 &= ~GPIO_P24_MODE_SEL(0x3); // ����
    P2_MD1 |= GPIO_P24_MODE_SEL(0x1);  // ���ģʽ
    FOUT_S24 = GPIO_FOUT_TMR2_PWMOUT;  // ���ó�TMR��PWM���

// #define PEROID_VAL (SYSCLK / 128 / 10000 - 1) // ����ֵ=ϵͳʱ��/��Ƶ/Ƶ�� - 1     // 10KHz
#define PEROID_VAL (SYSCLK / 128 / 1000 - 1) // ����ֵ=ϵͳʱ��/��Ƶ/Ƶ�� - 1     // 1KHz
    // #define PEROID_VAL (SYSCLK / 128 / 100 - 1) // ����ֵ=ϵͳʱ��/��Ƶ/Ƶ�� - 1     // 100Hz
    // #define PEROID_VAL (SYSCLK / 128 / 10 - 1) // ����ֵ=ϵͳʱ��/��Ƶ/Ƶ�� - 1     // 10Hz

    // ����Ƶ��Ϊ1kHZ��50%ռ�ձȵ�PWM    PWMƵ��=ϵͳʱ��/��Ƶ/(����ֵ+1)
    TMR_ALLCON = TMR2_CNT_CLR(0x1);                        // �������ֵ
    TMR2_PRH = TMR_PERIOD_VAL_H((PEROID_VAL >> 8) & 0xFF); // ����ֵ
    TMR2_PRL = TMR_PERIOD_VAL_L((PEROID_VAL >> 0) & 0xFF);
    TMR2_PWMH = TMR_PWM_VAL_H(((PEROID_VAL / 2) >> 8) & 0xFF); // ռ�ձ�����ֵ
    TMR2_PWML = TMR_PWM_VAL_L(((PEROID_VAL / 2) >> 0) & 0xFF);
    TMR2_CONH = TMR_PRD_PND(0x1) | TMR_PRD_IRQ_EN(0x1);                          // ʹ�ܼ����ж�
    TMR2_CONL = TMR_SOURCE_SEL(0x7) | TMR_PRESCALE_SEL(0x7) | TMR_MODE_SEL(0x2); // ѡ��ϵͳʱ�ӣ�128��Ƶ��PWMģʽ
}

// TMR2�жϷ�����
void TIMR2_IRQHandler(void) interrupt TMR2_IRQn
{

    // �����ж�����IP������ɾ��
    __IRQnIPnPush(TMR2_IRQn);

    // ---------------- �û��������� -------------------

    // �����ж�
    if (TMR2_CONH & TMR_PRD_PND(0x1))
    {
        TMR2_CONH |= TMR_PRD_PND(0x1); // ���pending

        tmr2_cnt++; // ÿ5ms��һ��
    }

    // �˳��ж�����IP������ɾ��
    __IRQnIPnPop(TMR2_IRQn);
}
