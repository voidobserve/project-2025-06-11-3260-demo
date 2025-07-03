// my_config.h -- 自定义的配置文件
#ifndef __MY_CONFIG_H
#define __MY_CONFIG_H

#include "include.h" // 使用芯片官方提供的头文件

// 是否打开调试
#define USE_MY_DEBUG            1
 

// 是否使用对应的片上外设/功能
#define USE_PRINTF_UART0        0   // printf()函数调用串口0的发送函数
#define USE_PRINTF_UART1        0   // printf()函数调用串口1的发送函数
#define USE_UART0               0
#define USE_UART1               0
 

#define ARRAY_SIZE(arry) (sizeof(arry) / sizeof(arry[0]))


enum
{
    ADC_SEL_PIN_GET_BAT, // 采集电池电压的引脚
    ADC_SEL_PIN_GET_CHARGE, // 采集充电电压的引脚
};



 
#include <stdio.h>   // printf()
#include <string.h>  // memset()
// #include "my_gpio.h" // 自定义的、使用到的引脚
// #include "uart0.h" 
// #include "uart1.h" 
#include "tmr0.h"
#include "tmr1.h"
// #include "tmr4.h"
 
#include "adc.h" 



#endif // end file
