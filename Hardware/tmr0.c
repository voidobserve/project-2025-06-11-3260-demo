#include "tmr0.h"



/**
 * @brief 配置定时器TMR0
 */
void timer0_config(void)
{
    __EnableIRQ(TMR0_IRQn); // 使能timer0中断
    IE_EA = 1;              // 使能总中断

    // 设置timer0的计数功能，配置一个频率为1kHz的中断
    TMR_ALLCON = TMR0_CNT_CLR(0x1);                               // 清除计数值
    TMR0_PRH = TMR_PERIOD_VAL_H((TIMER0_PERIOD_VAL >> 8) & 0xFF); // 周期值
    TMR0_PRL = TMR_PERIOD_VAL_L((TIMER0_PERIOD_VAL >> 0) & 0xFF);
    TMR0_CONH = TMR_PRD_PND(0x1) | TMR_PRD_IRQ_EN(0x1);                          // 计数等于周期时允许发生中断
    TMR0_CONL = TMR_SOURCE_SEL(0x7) | TMR_PRESCALE_SEL(0x7) | TMR_MODE_SEL(0x1); // 选择系统时钟，128分频，计数模式
}

// TMR0中断服务函数
void TIMR0_IRQHandler(void) interrupt TMR0_IRQn
{
    // 进入中断设置IP，不可删除
    __IRQnIPnPush(TMR0_IRQn);

    // ---------------- 用户函数处理 -------------------

    // 周期中断
    if (TMR0_CONH & TMR_PRD_PND(0x1))
    {
        TMR0_CONH |= TMR_PRD_PND(0x1); // 清除pending

        { 
            static u16 cnt = 0;
            cnt++;
            if (cnt >= 100)
            {
                cnt = 0;
                flag_is_adjust_pwm_time_comes = 1; // 表示调节充电的时间到来
            }
        }

        #if 1 // 电池电量指示灯控制

        {
            static u16 cnt = 0;

            if (1)
            {
                // 如果正在充电，最高位指示灯闪烁，满电则不闪烁
            }
            else
            {
                // 如果不在充电
            }
        }

        #endif// 电池电量指示灯控制

    }

    // 退出中断设置IP，不可删除
    __IRQnIPnPop(TMR0_IRQn);
}
