// my_config.h -- �Զ���������ļ�
#ifndef __MY_CONFIG_H
#define __MY_CONFIG_H

#include "include.h" // ʹ��оƬ�ٷ��ṩ��ͷ�ļ�

// �Ƿ�򿪵���
#define USE_MY_DEBUG            1
 

// �Ƿ�ʹ�ö�Ӧ��Ƭ������/����
#define USE_PRINTF_UART0        0   // printf()�������ô���0�ķ��ͺ���
#define USE_PRINTF_UART1        0   // printf()�������ô���1�ķ��ͺ���
#define USE_UART0               0
#define USE_UART1               0
 

#define ARRAY_SIZE(arry) (sizeof(arry) / sizeof(arry[0]))


enum
{
    ADC_SEL_PIN_GET_BAT, // �ɼ���ص�ѹ������
    ADC_SEL_PIN_GET_CHARGE, // �ɼ�����ѹ������
};



 
#include <stdio.h>   // printf()
#include <string.h>  // memset()
// #include "my_gpio.h" // �Զ���ġ�ʹ�õ�������
// #include "uart0.h" 
// #include "uart1.h" 
#include "tmr0.h"
#include "tmr1.h"
// #include "tmr4.h"
 
#include "adc.h" 



#endif // end file
