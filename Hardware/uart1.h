#ifndef __UART1_H
#define __UART1_H

#include "include.h" // 使用芯片官方提供的头文件
#include "my_config.h" // 包含自定义的头文件

#if USE_UART1

// 串口1波特率
#ifndef UART1_BAUDRATE
#define UART1_BAUDRATE  115200
#endif // UART1_BAUDRATE

// 串口0接收缓冲区的大小
#ifndef UART1_RXBUF_LEN
#define UART1_RXBUF_LEN 250
#endif

// 用来存放接收的数据帧的缓冲区
extern volatile u8 uart1_recv_buf[UART1_RXBUF_LEN];
extern volatile u16 uart1_recv_cnt;  // 接收缓冲区计数值
extern volatile bit uart1_recv_flag; // 接收完成一帧的标志位

void uart1_config(void);
void uart1_sendbyte(u8 senddata);

#endif // USE_UART1

#endif
