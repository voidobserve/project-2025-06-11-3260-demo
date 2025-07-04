// use encoding UTF-8
#include "include.h"
#include "my_config.h"

volatile bit flag_kind_of_bat = 0;

volatile bit flag_is_charging_detected = 0; // 标志位，是否检测到充电
volatile bit flag_is_in_charging = 0;       // 标志位，是否在充电，是否打开了控制充电的PWM
volatile bit flag_is_adjust_pwm_time_comes = 0;

volatile bit flag_is_led_25_percent_enable = 0;  // 标志位，指示灯是否使能
volatile bit flag_is_led_50_percent_enable = 0;  // 标志位，指示灯是否使能
volatile bit flag_is_led_75_percent_enable = 0;  // 标志位，指示灯是否使能
volatile bit flag_is_led_100_percent_enable = 0; // 标志位，指示灯是否使能

/*
    enum
    {
        LED_POSITION_25_PERCENT = 0,
        LED_POSITION_50_PERCENT,
        LED_POSITION_75_PERCENT,
        LED_POSITION_100_PERCENT,
    };
*/
volatile u8 cur_led_position = 0;
volatile u8 last_led_position = 0;

volatile u16 last_pwm_val; // 3260使用16位，HC15P121B1使用8位
volatile u16 max_pwm_val;  // 3260使用16位，HC15P121B1使用8位

volatile u16 adc_bat_val;      // 采集到的电池电压
volatile u16 adc_charging_val; // 采集到的充电电压
volatile u16 tmp_bat_val;
volatile u16 tmp_val; // 3260使用16位，HC15P121B1使用8位
volatile u8 tmp_val_cnt;
volatile u16 tmp_val_l[8] = {0};

void charge_handle(void)
{
    // 如果电池已经满电或接近满电，并且此时没有打开控制充电的PWM，退出

    // 如果没有检测到充电输入，退出

#if 1
    u8 i = 0;

    last_pwm_val = ((u16)TMR1_PWMH << 8) | TMR1_PWML; // 读出上一次PWM占空比对应的值
    max_pwm_val = (TIMER1_PEROID_VAL + 1);            // 读出PWM占空比设定的、最大的值

    /*
        修改电压差值，电压差值 = 203 - (adc_bat_val * 122 / 1000)

        推导过程：
        在充电时测得，充电电流1.1A左右，压差为-30(ad值)时，电池一侧电压为7.8V(ad值：1917)
                     充电电流1.1A左右，压差为0(ad值)时，电池一侧电压为6.8V(ad值：1671)
        假设x轴为电压对应的ad值，y轴为压差对应的ad值，建立参考坐标系
        根据这两个测试点，发现x轴正向增长，y轴负向增长，画出的斜线向下，斜率为负，求出斜率
            k = Δy/Δx = (0 - 30) / (1917 - 1671)，约为 -0.122
        建立公式：y = kx + b，代入，解得 b 约为 203 （四舍五入是204）
        y = kx + b ==> 压差 = -0.122 * 充电时的电池电压 + 203
        转换成单片机可以计算的形式：压差 = 203 - (充电时的电池电压 * 122 / 1000)
    */

    /*
        检测电池电压 1M上拉、470K下拉
        检测电池电压的分压系数 == 470K / (470K + 1M)
        约为 0.31972789115646258503401360544218
    */
#if 1
    tmp_bat_val = adc_bat_val;
    if (adc_bat_val <= 2837) // 如果检测电池电压小于 6.5V
    {
        // tmp_bat_val += 30;
        tmp_bat_val += 70; /* 6.25--1.02，6.35--1.08 */
    }
    else if (adc_bat_val <= 3056) // 如果检测电池电压小于 7.0V
    {
        // tmp_bat_val += 30; //
        tmp_bat_val += 50; // 6.64--1.01，6.70--1.03，6.80--1.028，6.90--1.10
        // tmp_bat_val += 70; //
    }
    else if (adc_bat_val <= 3188) // 如果检测电池电压小于 7.3V
    {
        // tmp_bat_val += 20; //
        tmp_bat_val += 35; // 7.20--1.08，7.25--1.04
        // tmp_bat_val += 40; // 7.02--1.06，7.13--1.07，7.17--1.115
        // tmp_bat_val += 60; //
    }
    else if (adc_bat_val <= 3326) // 如果检测电池电压小于 7.62V
    {
        // tmp_bat_val += 10; //
        tmp_bat_val += 20; // 7.33--0.991，7.37--1.03，7.40--1.021，7.50--1.05
        // tmp_bat_val += 30; // 7.33--1.087
        // tmp_bat_val += 40; //
    }
    else // 如果在充电时检测到电池电压大于
    {
        /*
            tmp_bat_val += 15;  这个时候常态下可能只有0.97，但是动一下线路板或者线缆，会跳到1.07A
            7.76--0.975，7.78--1.04，8.00--1.084
        */
        tmp_bat_val += 15;
        // tmp_bat_val += 25; //
        // tmp_bat_val += 30; // 7.70--1.06，7.73--1.100，
        // tmp_bat_val += 35; //
        // tmp_bat_val += 40; // 7.70--1.16，8.07V -- 1.06A，8.2V之后好像会升到1.1A，8.23V--1.08A
        // tmp_bat_val += 45; //
        // tmp_bat_val += 50; // 在8.08V会到1.10
        // tmp_bat_val += 52; //
        // tmp_bat_val += 55; //
        // tmp_bat_val += 60; //   超过8V会到1.10
        // tmp_bat_val += 70; // 超过8V时会超过1.1A，导致电感发热
        tmp_bat_val -= ((u32)adc_bat_val * 157 / 1000 - 522);
    }

    tmp_bat_val += 50;
    // tmp_bat_val += 60;
    // tmp_bat_val += 70;
    // tmp_bat_val += 80;
    // tmp_bat_val += 90;

    // if (adc_bat_val >= 3579) // 8.2V及以上 , 降低电流
    if (adc_bat_val >= 3623) // 8.30V及以上 , 降低电流
    {
        u16 i;
        // for (i = 0; i < 40; i++) //
        // for (i = 0; i < 50; i++) //
        // for (i = 0; i < 70; i++) // 8.33--0.93A
        // for (i = 0; i < 75; i++) // 8.33--0.86A
        // for (i = 0; i < 80; i++) // 8.34--0.88
        // for (i = 0; i < 85; i++) // 8.33V--0.62A
        // for (i = 0; i < 90; i++) // 8.33--0.866，
        // for (i = 0; i < 100; i++) // 8.32--0.90A，8.34--0.89
        for (i = 0; i < 120; i++) //
        {
            if (tmp_bat_val > 2)
            {
                tmp_bat_val--;
            }
        }
    }
#endif

    /*
        升压公式：Vo = Vi / (1 - D)

        通过PWM来控制升压，这里假设当前PWM占空比寄存器的值 为 D，PWM占空比寄存器可以到的最大的值 为 1
        Vo = Vi / (PWM占空比寄存器可以到的最大的值 - 当前PWM占空比寄存器的值)
        当前PWM占空比越大，Vo也越大，充电的电流也会越大

        (PWM占空比寄存器可以到的最大的值 - 当前PWM占空比寄存器的值) = Vi / Vo
        当前PWM占空比寄存器的值 = PWM占空比寄存器可以到的最大的值 - Vi / Vo

        这里检测到的充电电压的ad值 == USB-C口电压 / 2[上下拉电阻分压] / 参考电压[3V，那么这里就是除以3] * 4096[ad转换精度，12位-->0~4096]
        即，这里检测到的充电电压的ad值 == USB-C口电压 / 2 / 3 * 4096
        检测到的电池电压的ad值 == 电池电压 * 0.18 / 3V参考电压 * 4096 == 电池电压 * 220 / 1220 / 3V参考电压 * 4096
        (电池的分压电阻：下拉220K，上拉1M，分压系数： 220 / 1220)

        检测充电电压和检测电池电压使用的不是同一个分压系数，要一起运算时，这里将充电电压的ad再 * 2 * 220 / 1220
        即 (adc_charging_val * 22 / 61)

        再代回公式：当前PWM占空比寄存器的值 = PWM占空比寄存器可以到的最大的值 - Vi / Vo
        当前PWM占空比寄存器的值 = PWM占空比寄存器可以到的最大的值 - 充电电压 / 充电时电池两侧的电压
        tmp_val = max_pwm_val - 充电电压 / 充电时电池两侧的电压
        转换成单片机可以计算的形式：
        tmp_val = max_pwm_val - (adc_charging_val * 22 / 61) / tmp_bat_val，但是 max_pwm_val 的值不是1，不符合 Vo = Vi / (1 - D)
        这里要改成 tmp_val = max_pwm_val - max_pwm_val * (adc_charging_val * 22 / 61) / tmp_bat_val
        tmp_val = max_pwm_val - (adc_charging_val * max_pwm_val * 22 / 61) / tmp_bat_val
    */
    // D = 1 - (Vi / Vo)
    // tmp_val = max_pwm_val - (adc_charging_val * max_pwm_val * 22 / 61) / tmp_bat_val;
    // tmp_val = max_pwm_val - (adc_charging_val * max_pwm_val * 94 / 147) / tmp_bat_val;
    // tmp_val = max_pwm_val - ((u32)4095 * max_pwm_val * 94 / 147) / tmp_bat_val;

    /*
        假设充电检测脚外部用100K下拉，1M上拉，输入电压变为1/11分压
    */
    // tmp_val = max_pwm_val - (max_pwm_val) * ((u32)adc_charging_val / 4096 * 3 * 11 / 1) / tmp_bat_val * / 4096 *3 * 1470 / 470;
    tmp_val = max_pwm_val - (max_pwm_val) * ((u32)adc_charging_val * 11 / 1) / tmp_bat_val * 1470 / 470;
    // tmp_val = max_pwm_val - ((u32)adc_charging_val * max_pwm_val * 1 / 11) / tmp_bat_val; // TODO: 可能要根据外部输入的电压范围来修改公式

    // adc_charging_val 改成了 adc_initial_charging_val，只使用刚插入充电头时对应的ad值来代入公式：
    // tmp_val = max_pwm_val - ((u32)adc_initial_charging_val * max_pwm_val * 94 / 147) / tmp_bat_val;

    if ((u16)tmp_val >= max_pwm_val) // 3260使用16位，HC15P121B1使用8位
    {
        // 如果PWM占空比对应的值 大于 最大占空比对应的值，说明计算溢出（可能是电池电压过小），按0处理
        tmp_val = 0;
    }

    // 滤波操作，一开始tmp_val会很小，采集多次后趋于一个平均值：
    tmp_val_cnt++;
    tmp_val_cnt &= 0x07;
    tmp_val_l[tmp_val_cnt] = (tmp_val_l[tmp_val_cnt] + tmp_val) >> 1;
    tmp_val = 0;
    for (i = 0; i < 8; i++)
    {
        tmp_val += tmp_val_l[i];
    }

    tmp_val >>= 3;

    {
        /*
            如果差值过大，则快速调节，如果差值过小，则慢速调节，
            防止电流突变，导致不同的板子最终充电电流不一致
        */
        if (flag_is_adjust_pwm_time_comes) // 调节时间到来
        {
            flag_is_adjust_pwm_time_comes = 0;

            if (tmp_val > last_pwm_val)
            {

                last_pwm_val++;
            }
            else if (tmp_val < last_pwm_val)
            {

                last_pwm_val--;
            }
        }
    }

    // T2DATA = last_pwm_val;
    TMR1_PWMH = last_pwm_val >> 8;
    TMR1_PWML = last_pwm_val & 0xFF;

#endif
}

// 电池类型检测（包含检测引脚的初始化）
void kind_of_bat_init(void)
{
    P1_PU |= GPIO_P11_PULL_UP(0x01);      // 上拉
    P1_MD0 &= ~(GPIO_P11_MODE_SEL(0x03)); // 输入模式

    delay_ms(1);

    if (LEVEL_LEAD_BATTERY == DETECT_KIND_OF_BAT_PIN)
    {
        // 检测脚检测到的电平是铅酸电池对应的电平
        flag_kind_of_bat = KIND_OF_BAT_LEAD; // 铅酸电池
    }
    else
    {
        // 检测脚检测到的电平是锂电池对应的电平
        flag_kind_of_bat = KIND_OF_BAT_LITHIUM; // 锂电池
    }

#if USE_MY_DEBUG

    if (KIND_OF_BAT_LEAD == flag_kind_of_bat) // 铅酸电池
    {
        printf("lead battery\n");
    }
    else
    {
        printf("lithium battery\n");
    }

#endif
}

// 指示灯引脚
// 电池电量指示灯引脚
void led_config(void)
{
    P1_MD0 &= ~GPIO_P12_MODE_SEL(0x03);
    P1_MD0 |= GPIO_P12_MODE_SEL(0x01);
    FOUT_S12 = GPIO_FOUT_AF_FUNC;
    // P12 = LED_OFF_LEVEL; // 如果不给初始值，上电之后，指示灯会闪一下
    LED_25_PERCENT_OFF();

    P1_MD0 &= ~GPIO_P13_MODE_SEL(0x03);
    P1_MD0 |= GPIO_P13_MODE_SEL(0x01);
    FOUT_S13 = GPIO_FOUT_AF_FUNC;
    // P13 = LED_OFF_LEVEL;
    LED_50_PERCENT_OFF();

    P1_MD1 &= ~GPIO_P14_MODE_SEL(0x03);
    P1_MD1 |= GPIO_P14_MODE_SEL(0x01);
    FOUT_S14 = GPIO_FOUT_AF_FUNC;
    // P14 = LED_OFF_LEVEL;
    LED_75_PERCENT_OFF();

    P1_MD1 &= ~GPIO_P15_MODE_SEL(0x03);
    P1_MD1 |= GPIO_P15_MODE_SEL(0x01);
    FOUT_S15 = GPIO_FOUT_AF_FUNC;
    // P15 = LED_OFF_LEVEL;
    LED_100_PERCENT_OFF();
}

// 充电检测
void charging_detect(void)
{
#if 0
    if (adc_charging_val >= ) // 大于充电阈值，说明检测到了充电
    {
        flag_is_charging_detected = 1;
    }
    else if (adc_charging_val < ) // 小于充电阈值 - 死区，说明没有检测到充电
    {
        flag_is_charging_detected = 0;
    }
#endif
}

// 电池电量指示灯控制
void led_handle(void)
{
    // static u8 last_bat_level = 4; //
    static bit flag_last_charging_status = 0;

    if (flag_is_in_charging)
    {
        // 如果正在充电
        // 充电时，如果电池电量升高，例如：从3格指示灯变为4格指示灯，指示灯不能再变回3格指示灯，防止指示灯闪烁，除非退出了充电

        if (KIND_OF_BAT_LEAD == flag_kind_of_bat) // 如果是铅酸电池 在充电
        {
            // if (adc_bat_val >= )  // 25%
            {
            }

            // if (adc_bat_val >= ) // 50%
            {
            }

            // if (adc_bat_val >= ) // 75%
            {
            }

            // if (adc_bat_val >= ) // 100%
            {
            }
        }
        else // 如果是锂电池 在 充电
        {
            // 充电时，如果电池电量升高，例如：从3格指示灯变为4格指示灯，指示灯不能再变回3格指示灯，防止指示灯闪烁，除非退出了充电

            if (adc_bat_val >= ADC_VAL_CHARGE_25_PERCENT_OF_LITHIUM_BATTERY) // 25%
            {
                cur_led_position = LED_POSITION_25_PERCENT;
            }

            if (adc_bat_val >= ADC_VAL_CHARGE_50_PERCENT_OF_LITHIUM_BATTERY) // 50%
            {
                LED_50_PERCENT_ENABLE();
            }

            if (adc_bat_val >= ADC_VAL_CHARGE_75_PERCENT_OF_LITHIUM_BATTERY) // 75%
            {
                LED_75_PERCENT_ENABLE();
            }

            if (adc_bat_val >= ADC_VAL_CHARGE_100_PERCENT_OF_LITHIUM_BATTERY) // 100%
            {
                LED_100_PERCENT_ENABLE();
            }
        }
    }
    else
    {
        // 如果不在充电
        // 放电时，如果电池电量降低，例如：从4格指示灯变为3格指示灯，指示灯不能再变回4格指示灯，防止指示灯闪烁，除非又进行了充电

        if (KIND_OF_BAT_LEAD == flag_kind_of_bat) // 如果是铅酸电池
        {
            // if (adc_bat_val >= )  // 25%
            {
            }

            // if (adc_bat_val >= ) // 50%
            {
            }

            // if (adc_bat_val >= ) // 75%
            {
            }

            // if (adc_bat_val >= ) // 100%
            {
            }
        }
        else // 如果是锂电池
        {
            if (adc_bat_val >= ADC_VAL_CHARGE_25_PERCENT_OF_LITHIUM_BATTERY) // 25%
            {
            }

            if (adc_bat_val >= ADC_VAL_CHARGE_50_PERCENT_OF_LITHIUM_BATTERY) // 50%
            {
            }

            if (adc_bat_val >= ADC_VAL_CHARGE_75_PERCENT_OF_LITHIUM_BATTERY) // 75%
            {
                LED_75_PERCENT_ENABLE();
            }

            if (adc_bat_val >= ADC_VAL_CHARGE_100_PERCENT_OF_LITHIUM_BATTERY) // 100%
            {
                LED_100_PERCENT_ENABLE();
            }
        }
    }
}

// 变量初始化
void param_init(void)
{
    // =================================================================
    // 电池类型                                                       //
    // =================================================================
    P1_PU |= GPIO_P11_PULL_UP(0x01);      // 上拉
    P1_MD0 &= ~(GPIO_P11_MODE_SEL(0x03)); // 输入模式

    delay_ms(1);

    if (LEVEL_LEAD_BATTERY == DETECT_KIND_OF_BAT_PIN)
    {
        // 检测脚检测到的电平是铅酸电池对应的电平
        flag_kind_of_bat = KIND_OF_BAT_LEAD; // 铅酸电池
    }
    else
    {
        // 检测脚检测到的电平是锂电池对应的电平
        flag_kind_of_bat = KIND_OF_BAT_LITHIUM; // 锂电池
    }

#if USE_MY_DEBUG

    // if (KIND_OF_BAT_LEAD == flag_kind_of_bat) // 铅酸电池
    // {
    //     printf("lead battery\n");
    // }
    // else
    // {
    //     printf("lithium battery\n");
    // }

#endif

    // =================================================================
    // 电池电量指示灯                                                  //
    // =================================================================
    adc_bat_val = adc_getval();

    if (KIND_OF_BAT_LEAD == flag_kind_of_bat) // 铅酸电池
    {
    }
    else // 锂电池
    {
        if (adc_bat_val >= ADC_VAL_CHARGE_25_PERCENT_OF_LITHIUM_BATTERY) // 25%
        {
            cur_led_position = LED_POSITION_25_PERCENT;
        }

        if (adc_bat_val >= ADC_VAL_CHARGE_50_PERCENT_OF_LITHIUM_BATTERY) // 50%
        {
            cur_led_position = LED_POSITION_50_PERCENT;
        }

        if (adc_bat_val >= ADC_VAL_CHARGE_75_PERCENT_OF_LITHIUM_BATTERY) // 75%
        {
            cur_led_position = LED_POSITION_75_PERCENT;
        }

        if (adc_bat_val >= ADC_VAL_CHARGE_100_PERCENT_OF_LITHIUM_BATTERY) // 100%
        {
            cur_led_position = LED_POSITION_100_PERCENT;
        }

        last_led_position = cur_led_position;
    }
}

void user_init(void)
{
// DEBUG:
#if USE_MY_DEBUG
    uart0_config();
#endif

    adc_config();
    timer0_config();
    timer1_pwm_config();

    timer1_pwm_disable();

    delay_ms(1); // 等待系统稳定
    kind_of_bat_init();

    param_init(); // 变量初始化
}

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
void main(void)
{
    // 看门狗默认打开, 复位时间2s
    system_init();

    WDT_KEY = WDT_KEY_VAL(0xDD); // 关闭看门狗

    // 关闭HCK和HDA的调试功能
    WDT_KEY = 0x55;  // 解除写保护
    IO_MAP &= ~0x01; // 清除这个寄存器的值，实现关闭HCK和HDA引脚的调试功能（解除映射）
    WDT_KEY = 0xBB;

    /* 用户代码初始化接口 */
    user_init();

    // DEBUG:
    flag_kind_of_bat = KIND_OF_BAT_LITHIUM; // 测试用

    timer1_set_pwm_duty_val(1);
    timer1_pwm_enable();

    /* 系统主循环 */
    while (1)
    {
        adc_sel_pin(ADC_SEL_PIN_GET_BAT);
        adc_bat_val = adc_getval();

        adc_sel_pin(ADC_SEL_PIN_GET_CHARGE);
        adc_charging_val = adc_getval();
        charge_handle();
    }
}

/**
 * @}
 */

/*************************** (C) COPYRIGHT 2022 HUGE-IC ***** END OF FILE *****/
