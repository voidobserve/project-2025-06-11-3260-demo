#ifndef __UART0_H
#define __UART0_H

#include "include.h"   // 使用芯片官方提供的头文件
#include "my_config.h" // 包含自定义的头文件

#include "include.h"

#define UART0_BAUD (115200UL)
#define USER_UART0_BAUD ((SYSCLK - UART0_BAUD) / (UART0_BAUD))

void uart0_config(void);

#endif
