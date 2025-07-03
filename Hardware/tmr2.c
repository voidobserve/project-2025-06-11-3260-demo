// 定时器TMR2的驱动源文件
#include "tmr2.h"

#define TMR2_CNT_TIME (SYSCLK / 32 / 1000 - 1) // 计数周期 1KHz，1ms产生一次中断

// volatile unsigned char tmr2_flag = 0; // tmr2中断服务函数中会置位的标志位
volatile u32 tmr2_cnt = 0; // 定时器TMR2的计数值（每次在中断服务函数中会加一）

/**
 * @brief 配置定时器TMR2
 */
void tmr2_config(void)
{
    // 配置定时器，用来记录RF接收到的高电平持续时间
    __SetIRQnIP(TMR2_IRQn, TMR2_IQn_CFG); // 设置中断优先级（TMR2）

    TMR2_CONL &= ~TMR_PRESCALE_SEL(0x03); // 清除TMR2的预分频配置寄存器
    TMR2_CONL |= TMR_PRESCALE_SEL(0x05);  // 配置分频系数
    TMR2_CONL &= ~TMR_MODE_SEL(0x03);     // 清除TMR2的模式配置寄存器
    TMR2_CONL |= TMR_MODE_SEL(0x01);      // 配置TMR2的模式为计数器模式，最后对系统时钟的脉冲进行计数

    TMR2_CONH &= ~TMR_PRD_PND(0x01); // 清除TMR2的计数标志位，表示未完成计数
    TMR2_CONH |= TMR_PRD_IRQ_EN(1);  // 使能TMR2的计数中断

    // 配置TMR2的计数周期
    TMR2_PRL = (unsigned char)(TMR2_CNT_TIME % 255);
    TMR2_PRH = (unsigned char)(TMR2_CNT_TIME / 255);

    // 清除TMR2的计数值
    // TMR2_CNTL = 0;
    // TMR2_CNTH = 0;
    TMR_ALLCON = TMR2_CNT_CLR(0x1); // 清除计数值

    TMR2_CONL &= ~(TMR_SOURCE_SEL(0x07)); // 清除TMR2的时钟源配置寄存器
    // TMR2_CONL |= TMR_SOURCE_SEL(0x07); // 配置TMR2的时钟源，使用系统时钟
    TMR2_CONL |= TMR_SOURCE_SEL(0x05); // 配置TMR2的时钟源，不用任何时钟
                                       // __EnableIRQ(TMR2_IRQn);			   // 使能中断

    __DisableIRQ(TMR2_IRQn); // 禁用中断
    IE_EA = 1;               // 打开总中断
}

/**
 * @brief 开启定时器TMR2，开始计时
 */
void tmr2_enable(void)
{
    // 重新给TMR2配置时钟
    TMR2_CONL &= ~(TMR_SOURCE_SEL(0x07)); // 清除定时器的时钟源配置寄存器
    TMR2_CONL |= TMR_SOURCE_SEL(0x06);    // 配置定时器的时钟源，使用系统时钟

    __EnableIRQ(TMR2_IRQn); // 使能中断
    IE_EA = 1;              // 打开总中断
}

/**
 * @brief 关闭定时器2，清空计数值
 */
void tmr2_disable(void)
{
    // 不给定时器提供时钟，让它停止计数
    TMR2_CONL &= ~(TMR_SOURCE_SEL(0x07)); // 清除定时器的时钟源配置寄存器
    TMR2_CONL |= TMR_SOURCE_SEL(0x05);    // 配置定时器的时钟源，不用任何时钟

    // 清除定时器的计数值
    // TMR2_CNTL = 0;
    // TMR2_CNTH = 0;
    TMR_ALLCON = TMR2_CNT_CLR(0x1); // 清除计数值

    __DisableIRQ(TMR2_IRQn); // 关闭中断（不使能中断）
}

// 定时器配置成PWM输出模式（调用该函数前，要先将对应的IO复用到定时器的PWM输出上）
void tmr2_pwm_config(void)
{
    //  配置P24为timer2的PWM输出端口
    P2_MD1 &= ~GPIO_P24_MODE_SEL(0x3); // 清零
    P2_MD1 |= GPIO_P24_MODE_SEL(0x1);  // 输出模式
    FOUT_S24 = GPIO_FOUT_TMR2_PWMOUT;  // 复用成TMR的PWM输出

// #define PEROID_VAL (SYSCLK / 128 / 10000 - 1) // 周期值=系统时钟/分频/频率 - 1     // 10KHz
#define PEROID_VAL (SYSCLK / 128 / 1000 - 1) // 周期值=系统时钟/分频/频率 - 1     // 1KHz
    // #define PEROID_VAL (SYSCLK / 128 / 100 - 1) // 周期值=系统时钟/分频/频率 - 1     // 100Hz
    // #define PEROID_VAL (SYSCLK / 128 / 10 - 1) // 周期值=系统时钟/分频/频率 - 1     // 10Hz

    // 配置频率为1kHZ，50%占空比的PWM    PWM频率=系统时钟/分频/(周期值+1)
    TMR_ALLCON = TMR2_CNT_CLR(0x1);                        // 清除计数值
    TMR2_PRH = TMR_PERIOD_VAL_H((PEROID_VAL >> 8) & 0xFF); // 周期值
    TMR2_PRL = TMR_PERIOD_VAL_L((PEROID_VAL >> 0) & 0xFF);
    TMR2_PWMH = TMR_PWM_VAL_H(((PEROID_VAL / 2) >> 8) & 0xFF); // 占空比设置值
    TMR2_PWML = TMR_PWM_VAL_L(((PEROID_VAL / 2) >> 0) & 0xFF);
    TMR2_CONH = TMR_PRD_PND(0x1) | TMR_PRD_IRQ_EN(0x1);                          // 使能计数中断
    TMR2_CONL = TMR_SOURCE_SEL(0x7) | TMR_PRESCALE_SEL(0x7) | TMR_MODE_SEL(0x2); // 选择系统时钟，128分频，PWM模式
}

// TMR2中断服务函数
void TIMR2_IRQHandler(void) interrupt TMR2_IRQn
{

    // 进入中断设置IP，不可删除
    __IRQnIPnPush(TMR2_IRQn);

    // ---------------- 用户函数处理 -------------------

    // 周期中断
    if (TMR2_CONH & TMR_PRD_PND(0x1))
    {
        TMR2_CONH |= TMR_PRD_PND(0x1); // 清除pending

        tmr2_cnt++; // 每5ms加一次
    }

    // 退出中断设置IP，不可删除
    __IRQnIPnPop(TMR2_IRQn);
}
