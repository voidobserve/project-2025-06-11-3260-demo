// use encoding UTF-8
#include "include.h"
#include "my_config.h"

volatile bit flag_is_charging_detected = 0; // 标志位，是否检测到充电
volatile bit flag_is_adjust_pwm_time_comes = 0;

volatile u16 last_pwm_val;
volatile u16 max_pwm_val;

volatile u16 adc_bat_val;      // 采集到的电池电压
volatile u16 adc_charging_val; // 采集到的充电电压
volatile u16 tmp_bat_val;
volatile u16 tmp_val;
volatile u8 tmp_val_cnt;
volatile u16 tmp_val_l[8] = 0;

void charge_handle(void)
{
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
        (电池的分压电阻： 上拉220K，下拉1M，分压系数： 220 / 1220)

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
    tmp_val = max_pwm_val - ((u32)adc_charging_val * max_pwm_val * 94 / 147) / tmp_bat_val; // TODO: 可能要根据外部输入的电压范围来修改公式

    // adc_charging_val 改成了 adc_initial_charging_val，只使用刚插入充电头时对应的ad值来代入公式：
    // tmp_val = max_pwm_val - ((u32)adc_initial_charging_val * max_pwm_val * 94 / 147) / tmp_bat_val;

    if (tmp_val >= max_pwm_val)
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

void user_init(void)
{
    adc_config();
    timer0_config();
    timer1_pwm_config();

    timer1_pwm_disable();
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

    timer1_set_pwm_duty_val(1);
    timer1_pwm_enable();

    /* 系统主循环 */
    while (1)
    {
        // adc_sel_pin(ADC_SEL_PIN_GET_BAT);
        // adc_bat_val = adc_getval();
        // charge_handle(); 
    }
}

/**
 * @}
 */

/*************************** (C) COPYRIGHT 2022 HUGE-IC ***** END OF FILE *****/
