#include "adc.h"

// volatile u16 adc_val = 0;

void adc_config(void)
{ 
    P1_MD1 |= GPIO_P16_MODE_SEL(0x03); // ģ��ģʽ
    P2_MD0 |= GPIO_P22_MODE_SEL(0x03);// ģ��ģʽ
 

    ADC_CFG1 |= (0x0F << 3); // ADCʱ�ӷ�ƵΪ16��Ƶ��Ϊϵͳʱ��/16
    ADC_CFG2 = 0xFF;         // ͨ��0����ʱ������Ϊ256������ʱ������

    ADC_ACON1 &= ~((0x01 << 5) |   /* �ر�ADC�ⲿ�ο�ѡ���ź� */
                   (0x07 << 0));   /* ���ADC�ڲ��ο���ѹ��ѡ������ */
    ADC_ACON1 |= (0x01 << 6) |     /* ʹ��adc�ڲ��ο� */
                 (0x03 << 3) |     /* �رղ����ź� */
                 (0x03 << 0);      /* ѡ�� �ڲ� 3.0V ��Ϊ�ο���ѹ */
    ADC_ACON0 = ADC_CMP_EN(0x1) |  // ��ADC�е�CMPʹ���ź�
                ADC_BIAS_EN(0x1) | // ��ADCƫ�õ�����ʹ�ź�
                ADC_BIAS_SEL(0x1); // ƫ�õ���ѡ��1x
    ADC_TRGS0 |= (0x07 << 4);      // ͨ�� 0DLY �� ADC ʱ�Ӹ���ѡ������Ϊ 4n+1��4 * 29 + 1
    ADC_CHS0 |= (0x01 << 6);       // ʹ�� ͨ�� 0DLY ����
}

void adc_sel_pin(u8 adc_pin)
{
    ADC_CFG0 &= ~((0x01 << 6) | (0x01 << 3)); // �ر�adc����ʹ��ͨ��0ת��
    ADC_CHS0 &= ~((0x01 << 7) |               /* ���ݸ�ʽ����� */
                  (0x01 << 5) |               // ѡ���ڲ�ͨ��
                  (0x01 << 4) |
                  (0x01 << 3) |
                  (0x01 << 2) |
                  (0x01 << 1) |
                  (0x01 << 0)); // ѡ���ⲿͨ·�����ѡ���adc0ͨ·

    if (ADC_SEL_PIN_GET_CHARGE == adc_pin) // �ɼ�����ѹ
    { 
        ADC_CHS0 |= (0x0E << 0); // P16 ��Ӧ��ģ��ͨ�� 
    }
    else if (ADC_SEL_PIN_GET_BAT == adc_pin) // �ɼ���ص�ѹ
    {
        ADC_CHS0 |= (0x12 << 0); // P22 ��Ӧ��ģ��ͨ�� 
    }

    ADC_CFG0 |= ADC_CHAN0_EN(0x1) | // ʹ��ͨ��0ת��
                ADC_EN(0x1);        // ʹ��A/Dת��
    delay_ms(1);                    // �ȴ�adc�ȶ�
}

// adc�ɼ�+�˲�
u16 adc_getval(void)
{
    u8 i = 0; // adc�ɼ������ļ���
    volatile u16 g_temp_value = 0;
    volatile u32 g_tmpbuff = 0;
    volatile u16 g_adcmax = 0;
    volatile u16 g_adcmin = 0xFFFF;

    // �ɼ�20�Σ�ȥ��ǰ���β�������ȥ��һ�����ֵ��һ����Сֵ����ȡƽ��ֵ
    for (i = 0; i < 20; i++)
    {
        ADC_CFG0 |= ADC_CHAN0_TRG(0x1); // ����ADC0ת��
        while (!(ADC_STA & ADC_CHAN0_DONE(0x1)))
            ;                                                 // �ȴ�ת�����
        g_temp_value = (ADC_DATAH0 << 4) | (ADC_DATAL0 >> 4); // ��ȡ channel0 ��ֵ
        ADC_STA = ADC_CHAN0_DONE(0x1);                        // ���ADC0ת����ɱ�־λ

        if (i < 2)
            continue; // ����ǰ���β�����
        if (g_temp_value > g_adcmax)
            g_adcmax = g_temp_value; // ���
        if (g_temp_value < g_adcmin)
            g_adcmin = g_temp_value; // ��С

        g_tmpbuff += g_temp_value;
    }

    g_tmpbuff -= g_adcmax;           // ȥ��һ�����
    g_tmpbuff -= g_adcmin;           // ȥ��һ����С
    g_temp_value = (g_tmpbuff >> 4); // ����16��ȡƽ��ֵ

    return g_temp_value;
}
