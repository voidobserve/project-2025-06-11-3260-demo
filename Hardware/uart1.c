// 不能使用UART1+DMA来接收不定长的数据，这里的代码不能用
#include "uart1.h"
#if USE_UART1

// 设置的波特率需要适配单片机的时钟，这里直接使用了官方的代码
#define USER_UART1_BAUD ((SYSCLK - UART1_BAUDRATE) / (UART1_BAUDRATE))

// 用来存放接收的数据帧的缓冲区
volatile u8 uart1_recv_buf[UART1_RXBUF_LEN] = {0};
volatile u16 uart1_recv_cnt = 0;  // 接收缓冲区计数值
volatile bit uart1_recv_flag = 0; // 接收完成一帧的标志位

/*
    // 使用示例
    if (uart1_recv_flag)
    {
        uart1_recv_flag = 0;
        printf("%s\n", uart1_recv_buf);
        memset(uart1_recv_buf, 0, UART1_RXBUF_LEN);
        uart1_recv_cnt = 0;
    }
*/

#if USE_PRINTF_UART1
// 重写putchar()函数
char putchar(char c)
{
    uart1_sendbyte(c); // 调用串口1的单个字节发送函数
    return c;
}
#endif // USE_PRINTF_UART1

void uart1_config(void)
{
    // // P11为发送引脚，P12为接收引脚
    // P1_MD0 &= (~GPIO_P11_MODE_SEL(0x3) | ~GPIO_P12_MODE_SEL(0x3));
    // P1_MD0 |= GPIO_P11_MODE_SEL(0x1); // 输出模式
    // FOUT_S11 |= GPIO_FOUT_UART1_TX;   // 配置P11为UART1_TX
    // FIN_S8 |= GPIO_FIN_SEL_P12;       // 配置P12为UART1_RX

    // P20为发送引脚
    P2_MD0 &= ~GPIO_P20_MODE_SEL(0x3);
    P2_MD0 |= GPIO_P20_MODE_SEL(0x1); // 输出模式
    FOUT_S20 |= GPIO_FOUT_UART1_TX;   // 配置为UART1_TX

    UART1_BAUD1 = (USER_UART1_BAUD >> 8) & 0xFF; // 配置波特率高八位
    UART1_BAUD0 = USER_UART1_BAUD & 0xFF;        // 配置波特率低八位
    UART1_CON0 = UART_STOP_BIT(0x0) |
                 UART_RX_IRQ_EN(0x1) |
                 UART_EN(0x1); // 8bit数据，1bit停止位，使能接收中断，使能串口

    __EnableIRQ(UART1_IRQn); // 打开UART模块中断
    IE_EA = 1;               // 打开总中断
}

// UART1发送一个字节数据的函数
void uart1_sendbyte(u8 senddata)
{
    while (!(UART1_STA & UART_TX_DONE(0x01))) // 等待前一次发送完成
        ;
    UART1_DATA = senddata;
    while (!(UART1_STA & UART_TX_DONE(0x01))) // 等待这次发送完成
        ;
}

// UART1中断服务函数（DMA接收满才会触发该中断）
void UART1_IRQHandler(void) interrupt UART1_IRQn
{
    // 进入中断设置IP，不可删除
    __IRQnIPnPush(UART1_IRQn);

    // ---------------- 用户函数处理 -------------------

    // RX接收完成中断
    if (UART1_STA & UART_RX_DONE(0x1))
    {
        uart1_recv_buf[uart1_recv_cnt++] = UART1_DATA;
        if (UART1_DATA == '\n')
        {
            uart1_recv_flag = 1;
        }
    }

    // 退出中断设置IP，不可删除
    __IRQnIPnPop(UART1_IRQn);
}

#endif // USE_UART1
