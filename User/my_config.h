// my_config.h -- 自定义的配置文件
#ifndef __MY_CONFIG_H
#define __MY_CONFIG_H

#include "include.h" // 使用芯片官方提供的头文件

// 是否打开调试
#define USE_MY_DEBUG 1

#define ENABLE 1
#define DISABLE 0
#define ARRAY_SIZE(arry) (sizeof(arry) / sizeof(arry[0]))

enum
{
    ADC_SEL_PIN_GET_BAT,    // 采集电池电压的引脚
    ADC_SEL_PIN_GET_CHARGE, // 采集充电电压的引脚
};

// =================================================================
// 电池类型                                                      //
// =================================================================
// 定义电池的类型，用于上电后赋值给对应的变量：
enum
{
    KIND_OF_BAT_LITHIUM = 0,
    KIND_OF_BAT_LEAD, //
};

//    刚上电，检测使用的电池是铅酸电池还是锂电池的引脚
#define DETECT_KIND_OF_BAT_PIN P11
/*
    检测脚检测到的电平
    检测脚检测到是铅酸电池还是锂电池对应的电平
*/
#define LEVEL_LITHIUM_BATTERY 1 // 锂电池对应的电平
#define LEVEL_LEAD_BATTERY 0    // 铅酸电池对应的电平

// =================================================================
// adc电压，电池电压，充电电压                                       //
// =================================================================

// ADC检测电池电压时，使用的参考电压，单位：10mV
#define ADC_REFERENCE_VOLTAGE_OF_DETECT_BAT ((u16)3000 / 10)
// ADC检测充电电压时，使用的参考电压，单位：10mV
#define ADC_REFERENCE_VOLTAGE_OF_DETECT_CHARGE ((u16)3000 / 10)

// resistance
// 电池电压检测脚外部的上拉电阻阻值，单位：100R
#define DETECT_BAT_PIN_RESISTANCE_PULL_UP ((u32)100 * 10000 / 100)
// 电池电压检测脚外部的下拉电阻阻值，单位：100R
#define DETECT_BAT_PIN_RESISTANCE_PULL_DOWN ((u32)470 * 1000 / 100)
// 电池电压分压系数 battery voltage division coefficient （不能直接单独使用，值会是0）
#define BATTERY_VOL_DIVISION_COEFFICIENT \
    (u32) DETECT_BAT_PIN_RESISTANCE_PULL_DOWN / (DETECT_BAT_PIN_RESISTANCE_PULL_UP + DETECT_BAT_PIN_RESISTANCE_PULL_DOWN)

// 充电电压检测脚外部的上拉电阻阻值，单位：100R
#define DETECT_CHARGE_PIN_RESISTANCE_PULL_UP ((u32)100 * 10000 / 100)
// 充电电压检测脚外部的下拉电阻阻值，单位：100R
#define DETECT_CHARGE_PIN_RESISTANCE_PULL_DOWN ((u32)100 * 1000 / 100)
// 充电电压分压系数 Charging voltage division coefficient （不能直接单独使用，值会是0）
#define CHARGING_VOL_DIVISION_COEFFICIENT \
    (u32) DETECT_CHARGE_PIN_RESISTANCE_PULL_DOWN / (DETECT_CHARGE_PIN_RESISTANCE_PULL_UP + DETECT_CHARGE_PIN_RESISTANCE_PULL_DOWN)

// 充电时 铅酸电池 100% 时，对应的电压，单位：10mV
#define CHARGE_100_PERCENT_OF_LEAD_BATTERY ((u32)7250 / 10)
// 充电时 铅酸电池 75% 时，对应的电压，单位：10mV
#define CHARGE_75_PERCENT_OF_LEAD_BATTERY ((u32)7100 / 10)
// 充电时 铅酸电池 50% 时，对应的电压，单位：10mV
#define CHARGE_50_PERCENT_OF_LEAD_BATTERY ((u32)6950 / 10)
// 充电时 铅酸电池 25% 时，对应的电压，单位：10mV
#define CHARGE_25_PERCENT_OF_LEAD_BATTERY ((u32)6800 / 10)

// 放电时 铅酸电池 100% 时，对应的电压，单位：10mV
#define DISCHARGE_100_PERCENT_OF_LEAD_BATTERY ((u32)6140 / 10)
// 放电时 铅酸电池 75% 时，对应的电压，单位：10mV
#define DISCHARGE_75_PERCENT_OF_LEAD_BATTERY ((u32)5960 / 10)
// 放电时 铅酸电池 50% 时，对应的电压，单位：10mV
#define DISCHARGE_50_PERCENT_OF_LEAD_BATTERY ((u32)5780 / 10)
// 放电时 铅酸电池 25% 时，对应的电压，单位：10mV
#define DISCHARGE_25_PERCENT_OF_LEAD_BATTERY ((u32)5600 / 10)

/*
    逐渐转换成单片机可以计算的形式：
    电池电压 * 分压系数 / adc参考电压 * 4096
    电池电压 * 4096 * 分压系数 / adc参考电压  ， 乘以分压系数中的下拉电阻就有可能超过2^32
    电池电压 * 4096 / adc参考电压 * 分压系数
*/
// 充电时 铅酸电池 100% 时，对应的ad值
#define ADC_VAL_CHARGE_100_PERCENT_OF_LEAD_BATTERY \
    ((u16)((u32)CHARGE_100_PERCENT_OF_LEAD_BATTERY * 4096 / ADC_REFERENCE_VOLTAGE_OF_DETECT_BAT * BATTERY_VOL_DIVISION_COEFFICIENT))
// 充电时 铅酸电池 75% 时，对应的ad值
#define ADC_VAL_CHARGE_75_PERCENT_OF_LEAD_BATTERY \
    ((u16)((u32)CHARGE_75_PERCENT_OF_LEAD_BATTERY * 4096 / ADC_REFERENCE_VOLTAGE_OF_DETECT_BAT * BATTERY_VOL_DIVISION_COEFFICIENT))
// 充电时 铅酸电池 50% 时，对应的ad值
#define ADC_VAL_CHARGE_50_PERCENT_OF_LEAD_BATTERY \
    ((u16)((u32)CHARGE_50_PERCENT_OF_LEAD_BATTERY * 4096 / ADC_REFERENCE_VOLTAGE_OF_DETECT_BAT * BATTERY_VOL_DIVISION_COEFFICIENT))
// 充电时 铅酸电池 25% 时，对应的ad值
#define ADC_VAL_CHARGE_25_PERCENT_OF_LEAD_BATTERY \
    ((u16)((u32)CHARGE_25_PERCENT_OF_LEAD_BATTERY * 4096 / ADC_REFERENCE_VOLTAGE_OF_DETECT_BAT * BATTERY_VOL_DIVISION_COEFFICIENT))

// 放电时 酸电池 100% 时，对应的ad值
#define ADC_VAL_DISCHARGE_100_PERCENT_OF_LEAD_BATTERY \
    ((u16)((u32)DISCHARGE_100_PERCENT_OF_LEAD_BATTERY * 4096 / ADC_REFERENCE_VOLTAGE_OF_DETECT_BAT * BATTERY_VOL_DIVISION_COEFFICIENT))
// 放电时 酸电池 75% 时，对应的ad值
#define ADC_VAL_DISCHARGE_75_PERCENT_OF_LEAD_BATTERY \
    ((u16)((u32)DISCHARGE_75_PERCENT_OF_LEAD_BATTERY * 4096 / ADC_REFERENCE_VOLTAGE_OF_DETECT_BAT * BATTERY_VOL_DIVISION_COEFFICIENT))
// 放电时 酸电池 50% 时，对应的ad值
#define ADC_VAL_DISCHARGE_50_PERCENT_OF_LEAD_BATTERY \
    ((u16)((u32)DISCHARGE_50_PERCENT_OF_LEAD_BATTERY * 4096 / ADC_REFERENCE_VOLTAGE_OF_DETECT_BAT * BATTERY_VOL_DIVISION_COEFFICIENT))
// 放电时 酸电池 25% 时，对应的ad值
#define ADC_VAL_DISCHARGE_25_PERCENT_OF_LEAD_BATTERY \
    ((u16)((u32)DISCHARGE_25_PERCENT_OF_LEAD_BATTERY * 4096 / ADC_REFERENCE_VOLTAGE_OF_DETECT_BAT * BATTERY_VOL_DIVISION_COEFFICIENT))

// 充电时 锂电池 100%时，对应的电压，单位：10mV
#define CHARGE_100_PERCENT_OF_LITHIUM_BATTERY ((u32)8400 / 10)
// 充电时 锂电池 75%时，对应的电压，单位：10mV
#define CHARGE_75_PERCENT_OF_LITHIUM_BATTERY ((u32)7900 / 10)
// 充电时 锂电池 50%时，对应的电压，单位：10mV
#define CHARGE_50_PERCENT_OF_LITHIUM_BATTERY ((u32)7400 / 10)
// 充电时 锂电池 25%时，对应的电压，单位：10mV
#define CHARGE_25_PERCENT_OF_LITHIUM_BATTERY ((u32)6800 / 10)

// 放电时 锂电池 100%时，对应的电压，单位：10mV
#define DISCHARGE_100_PERCENT_OF_LITHIUM_BATTERY ((u32)8400 / 10)
// 放电时 锂电池 75%时，对应的电压，单位：10mV
#define DISCHARGE_75_PERCENT_OF_LITHIUM_BATTERY ((u32)7900 / 10)
// 放电时 锂电池 50%时，对应的电压，单位：10mV
#define DISCHARGE_50_PERCENT_OF_LITHIUM_BATTERY ((u32)7400 / 10)
// 放电时 锂电池 25%时，对应的电压，单位：10mV
#define DISCHARGE_25_PERCENT_OF_LITHIUM_BATTERY ((u32)6800 / 10)

/*
    逐渐转换成单片机可以计算的形式：
    电池电压 * 分压系数 / adc参考电压 * 4096
    电池电压 * 4096 * 分压系数 / adc参考电压  ， 乘以分压系数中的下拉电阻就有可能超过2^32
    电池电压 * 4096 / adc参考电压 * 分压系数
*/
// 充电时 锂电池 100%时 对应的ad值
#define ADC_VAL_CHARGE_100_PERCENT_OF_LITHIUM_BATTERY \
    ((u16)((u32)CHARGE_100_PERCENT_OF_LITHIUM_BATTERY * 4096 / ADC_REFERENCE_VOLTAGE_OF_DETECT_BAT * BATTERY_VOL_DIVISION_COEFFICIENT))
// 充电时 锂电池 75%时 对应的ad值
#define ADC_VAL_CHARGE_75_PERCENT_OF_LITHIUM_BATTERY \
    ((u16)((u32)CHARGE_75_PERCENT_OF_LITHIUM_BATTERY * 4096 / ADC_REFERENCE_VOLTAGE_OF_DETECT_BAT * BATTERY_VOL_DIVISION_COEFFICIENT))
// 充电时 锂电池 50%时 对应的ad值
#define ADC_VAL_CHARGE_50_PERCENT_OF_LITHIUM_BATTERY \
    ((u16)((u32)CHARGE_50_PERCENT_OF_LITHIUM_BATTERY * 4096 / ADC_REFERENCE_VOLTAGE_OF_DETECT_BAT * BATTERY_VOL_DIVISION_COEFFICIENT))
// 充电时 锂电池 25%时 对应的ad值
#define ADC_VAL_CHARGE_25_PERCENT_OF_LITHIUM_BATTERY \
    ((u16)((u32)CHARGE_25_PERCENT_OF_LITHIUM_BATTERY * 4096 / ADC_REFERENCE_VOLTAGE_OF_DETECT_BAT * BATTERY_VOL_DIVISION_COEFFICIENT))

// 放电时 锂电池 100%时 对应的ad值
#define ADC_VAL_DISCHARGE_100_PERCENT_OF_LITHIUM_BATTERY \
    ((u16)((u32)DISCHARGE_100_PERCENT_OF_LITHIUM_BATTERY * 4096 / ADC_REFERENCE_VOLTAGE_OF_DETECT_BAT * BATTERY_VOL_DIVISION_COEFFICIENT))
// 放电时 锂电池 75%时 对应的ad值
#define ADC_VAL_DISCHARGE_75_PERCENT_OF_LITHIUM_BATTERY \
    ((u16)((u32)DISCHARGE_75_PERCENT_OF_LITHIUM_BATTERY * 4096 / ADC_REFERENCE_VOLTAGE_OF_DETECT_BAT * BATTERY_VOL_DIVISION_COEFFICIENT))
// 放电时 锂电池 50%时 对应的ad值
#define ADC_VAL_DISCHARGE_50_PERCENT_OF_LITHIUM_BATTERY \
    ((u16)((u32)DISCHARGE_50_PERCENT_OF_LITHIUM_BATTERY * 4096 / ADC_REFERENCE_VOLTAGE_OF_DETECT_BAT * BATTERY_VOL_DIVISION_COEFFICIENT))
// 放电时 锂电池 25%时 对应的ad值
#define ADC_VAL_DISCHARGE_25_PERCENT_OF_LITHIUM_BATTERY \
    ((u16)((u32)DISCHARGE_25_PERCENT_OF_LITHIUM_BATTERY * 4096 / ADC_REFERENCE_VOLTAGE_OF_DETECT_BAT * BATTERY_VOL_DIVISION_COEFFICIENT))

// 锂电池50mV对应的ad值
#define ADC_VAL_50mV_OF_LITHIUM_BATTERY \
    ((u8)((u32)5 * 4096 / ADC_REFERENCE_VOLTAGE_OF_DETECT_BAT * BATTERY_VOL_DIVISION_COEFFICIENT))

// enum
// {

// };

// =================================================================
// 电池电量指示灯                                                  //
// =================================================================
// 点亮指示灯对应的电平
#define LED_ON_LEVEL 1
// 熄灭指示灯对应的电平
#define LED_OFF_LEVEL 0
// 控制电池电量指示灯的引脚
#define LED_PIN_25_PERCENT P12                      // 电池电量指示灯 25%
#define LED_PIN_50_PERCENT P13                      // 电池电量指示灯 50%
#define LED_PIN_75_PERCENT P14                      // 电池电量指示灯 75%
#define LED_PIN_100_PERCENT P15                     // 电池电量指示灯 100%
extern volatile bit flag_is_led_25_percent_enable;  // 标志位，指示灯是否使能
extern volatile bit flag_is_led_50_percent_enable;  // 标志位，指示灯是否使能
extern volatile bit flag_is_led_75_percent_enable;  // 标志位，指示灯是否使能
extern volatile bit flag_is_led_100_percent_enable; // 标志位，指示灯是否使能
enum
{
    LED_POSITION_LESS_THAN_25_PERCENT = 0, // 少于25% 指示灯全灭
    LED_POSITION_25_PERCENT,
    LED_POSITION_50_PERCENT,
    LED_POSITION_75_PERCENT,
    LED_POSITION_100_PERCENT,
};

#define LED_25_PERCENT_ON()                    \
    {                                          \
        do                                     \
        {                                      \
            LED_PIN_25_PERCENT = LED_ON_LEVEL; \
        } while (0);                           \
    }
#define LED_25_PERCENT_OFF()                    \
    {                                           \
        do                                      \
        {                                       \
            LED_PIN_25_PERCENT = LED_OFF_LEVEL; \
        } while (0);                            \
    }
#define LED_50_PERCENT_ON()                    \
    {                                          \
        do                                     \
        {                                      \
            LED_PIN_50_PERCENT = LED_ON_LEVEL; \
        } while (0);                           \
    }
#define LED_50_PERCENT_OFF()                    \
    {                                           \
        do                                      \
        {                                       \
            LED_PIN_50_PERCENT = LED_OFF_LEVEL; \
        } while (0);                            \
    }
#define LED_75_PERCENT_ON()                    \
    {                                          \
        do                                     \
        {                                      \
            LED_PIN_75_PERCENT = LED_ON_LEVEL; \
        } while (0);                           \
    }
#define LED_75_PERCENT_OFF()                    \
    {                                           \
        do                                      \
        {                                       \
            LED_PIN_75_PERCENT = LED_OFF_LEVEL; \
        } while (0);                            \
    }
#define LED_100_PERCENT_ON()                    \
    {                                           \
        do                                      \
        {                                       \
            LED_PIN_100_PERCENT = LED_ON_LEVEL; \
        } while (0);                            \
    }
#define LED_100_PERCENT_OFF()                    \
    {                                            \
        do                                       \
        {                                        \
            LED_PIN_100_PERCENT = LED_OFF_LEVEL; \
        } while (0);                             \
    }

#define LED_25_PERCENT_ENABLE()                     \
    {                                               \
        do                                          \
        {                                           \
            flag_is_led_25_percent_enable = ENABLE; \
        } while (0);                                \
    }
#define LED_25_PERCENT_DISABLE()                     \
    {                                                \
        do                                           \
        {                                            \
            flag_is_led_25_percent_enable = DISABLE; \
        } while (0);                                 \
    }
#define LED_50_PERCENT_ENABLE()                     \
    {                                               \
        do                                          \
        {                                           \
            flag_is_led_50_percent_enable = ENABLE; \
        } while (0);                                \
    }
#define LED_50_PERCENT_DISABLE()                     \
    {                                                \
        do                                           \
        {                                            \
            flag_is_led_50_percent_enable = DISABLE; \
        } while (0);                                 \
    }
#define LED_75_PERCENT_ENABLE()                     \
    {                                               \
        do                                          \
        {                                           \
            flag_is_led_75_percent_enable = ENABLE; \
        } while (0);                                \
    }
#define LED_75_PERCENT_DISABLE()                     \
    {                                                \
        do                                           \
        {                                            \
            flag_is_led_75_percent_enable = DISABLE; \
        } while (0);                                 \
    }
#define LED_100_PERCENT_ENABLE()                     \
    {                                                \
        do                                           \
        {                                            \
            flag_is_led_100_percent_enable = ENABLE; \
        } while (0);                                 \
    }
#define LED_100_PERCENT_DISABLE()                     \
    {                                                 \
        do                                            \
        {                                             \
            flag_is_led_100_percent_enable = DISABLE; \
        } while (0);                                  \
    }

extern volatile bit flag_is_adjust_pwm_time_comes; // 调节充电电流的时间是否到来

#include <stdio.h>  // printf()
#include <string.h> // memset()
// #include "my_gpio.h" // 自定义的、使用到的引脚
#include "uart0.h"
// #include "uart1.h"
#include "tmr0.h"
#include "tmr1.h"
// #include "tmr4.h"

#include "adc.h"

#endif // end file
