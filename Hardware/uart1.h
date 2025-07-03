#ifndef __UART1_H
#define __UART1_H

#include "include.h" // ʹ��оƬ�ٷ��ṩ��ͷ�ļ�
#include "my_config.h" // �����Զ����ͷ�ļ�

#if USE_UART1

// ����1������
#ifndef UART1_BAUDRATE
#define UART1_BAUDRATE  115200
#endif // UART1_BAUDRATE

// ����0���ջ������Ĵ�С
#ifndef UART1_RXBUF_LEN
#define UART1_RXBUF_LEN 250
#endif

// ������Ž��յ�����֡�Ļ�����
extern volatile u8 uart1_recv_buf[UART1_RXBUF_LEN];
extern volatile u16 uart1_recv_cnt;  // ���ջ���������ֵ
extern volatile bit uart1_recv_flag; // �������һ֡�ı�־λ

void uart1_config(void);
void uart1_sendbyte(u8 senddata);

#endif // USE_UART1

#endif
