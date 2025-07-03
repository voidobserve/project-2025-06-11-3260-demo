// ����ʹ��UART1+DMA�����ղ����������ݣ�����Ĵ��벻����
#include "uart1.h"
#if USE_UART1

// ���õĲ�������Ҫ���䵥Ƭ����ʱ�ӣ�����ֱ��ʹ���˹ٷ��Ĵ���
#define USER_UART1_BAUD ((SYSCLK - UART1_BAUDRATE) / (UART1_BAUDRATE))

// ������Ž��յ�����֡�Ļ�����
volatile u8 uart1_recv_buf[UART1_RXBUF_LEN] = {0};
volatile u16 uart1_recv_cnt = 0;  // ���ջ���������ֵ
volatile bit uart1_recv_flag = 0; // �������һ֡�ı�־λ

/*
    // ʹ��ʾ��
    if (uart1_recv_flag)
    {
        uart1_recv_flag = 0;
        printf("%s\n", uart1_recv_buf);
        memset(uart1_recv_buf, 0, UART1_RXBUF_LEN);
        uart1_recv_cnt = 0;
    }
*/

#if USE_PRINTF_UART1
// ��дputchar()����
char putchar(char c)
{
    uart1_sendbyte(c); // ���ô���1�ĵ����ֽڷ��ͺ���
    return c;
}
#endif // USE_PRINTF_UART1

void uart1_config(void)
{
    // // P11Ϊ�������ţ�P12Ϊ��������
    // P1_MD0 &= (~GPIO_P11_MODE_SEL(0x3) | ~GPIO_P12_MODE_SEL(0x3));
    // P1_MD0 |= GPIO_P11_MODE_SEL(0x1); // ���ģʽ
    // FOUT_S11 |= GPIO_FOUT_UART1_TX;   // ����P11ΪUART1_TX
    // FIN_S8 |= GPIO_FIN_SEL_P12;       // ����P12ΪUART1_RX

    // P20Ϊ��������
    P2_MD0 &= ~GPIO_P20_MODE_SEL(0x3);
    P2_MD0 |= GPIO_P20_MODE_SEL(0x1); // ���ģʽ
    FOUT_S20 |= GPIO_FOUT_UART1_TX;   // ����ΪUART1_TX

    UART1_BAUD1 = (USER_UART1_BAUD >> 8) & 0xFF; // ���ò����ʸ߰�λ
    UART1_BAUD0 = USER_UART1_BAUD & 0xFF;        // ���ò����ʵͰ�λ
    UART1_CON0 = UART_STOP_BIT(0x0) |
                 UART_RX_IRQ_EN(0x1) |
                 UART_EN(0x1); // 8bit���ݣ�1bitֹͣλ��ʹ�ܽ����жϣ�ʹ�ܴ���

    __EnableIRQ(UART1_IRQn); // ��UARTģ���ж�
    IE_EA = 1;               // �����ж�
}

// UART1����һ���ֽ����ݵĺ���
void uart1_sendbyte(u8 senddata)
{
    while (!(UART1_STA & UART_TX_DONE(0x01))) // �ȴ�ǰһ�η������
        ;
    UART1_DATA = senddata;
    while (!(UART1_STA & UART_TX_DONE(0x01))) // �ȴ���η������
        ;
}

// UART1�жϷ�������DMA�������Żᴥ�����жϣ�
void UART1_IRQHandler(void) interrupt UART1_IRQn
{
    // �����ж�����IP������ɾ��
    __IRQnIPnPush(UART1_IRQn);

    // ---------------- �û��������� -------------------

    // RX��������ж�
    if (UART1_STA & UART_RX_DONE(0x1))
    {
        uart1_recv_buf[uart1_recv_cnt++] = UART1_DATA;
        if (UART1_DATA == '\n')
        {
            uart1_recv_flag = 1;
        }
    }

    // �˳��ж�����IP������ɾ��
    __IRQnIPnPop(UART1_IRQn);
}

#endif // USE_UART1
