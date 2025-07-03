// 定时器TMR3的驱动源文件
#include "tmr3.h"

#define TMR3_CNT_TIME (SYSCLK / 32 / 1000 - 1) // 计数周期 1KHz，1ms产生一次中断

volatile u32 tmr3_cnt = 0; // 定时器TMR3的计数值（每次在中断服务函数中会加一）
// volatile bit tmr3_flag = 0; // tmr3中断服务函数中会置位的标志位

// 定时器TMR3的初始化（初始化完成后，默认是关闭的）
void tmr3_config(void)
{
    __SetIRQnIP(TMR3_IRQn, TMR3_IQn_CFG); // 设置中断优先级（TMR3）

    TMR3_CONL &= ~TMR_PRESCALE_SEL(0x07); // 清除TMR3的预分频配置寄存器
    TMR3_CONL |= TMR_PRESCALE_SEL(0x05);  // 配置分频系数
    TMR3_CONL &= ~TMR_MODE_SEL(0x03);     // 清除TMR3的模式配置寄存器
    TMR3_CONL |= TMR_MODE_SEL(0x01);      // 配置TMR3的模式为计数器模式，最后对系统时钟的脉冲进行计数

    TMR3_CONH &= ~TMR_PRD_PND(0x01); // 清除TMR3的计数标志位，表示未完成计数
    TMR3_CONH |= TMR_PRD_IRQ_EN(1);  // 使能TMR3的计数中断

    __DisableIRQ(TMR3_IRQn); // 禁用中断

    // 配置TMR3的计数周期
    TMR3_PRL = (unsigned char)(TMR3_CNT_TIME % 255);
    TMR3_PRH = (unsigned char)(TMR3_CNT_TIME / 255);

    // 清除TMR3的计数值
    // TMR3_CNTL = 0;
    // TMR3_CNTH = 0;
    TMR_ALLCON = TMR3_CNT_CLR(0x1); // 清除计数值(写1清零)

    TMR3_CONL &= ~(TMR_SOURCE_SEL(0x07)); // 清除TMR3的时钟源配置寄存器
    // TMR3_CONL |= TMR_SOURCE_SEL(0x07); // 配置TMR3的时钟源，使用系统时钟
    TMR3_CONL |= TMR_SOURCE_SEL(0x05); // 配置TMR3的时钟源，不用任何时钟
    // __EnableIRQ(TMR3_IRQn);			   // 使能中断
    IE_EA = 1; // 打开总中断
}

// 开启定时器TMR3，开始计时
void tmr3_enable(void)
{
    // 重新给TMR3配置时钟
    TMR3_CONL &= ~(TMR_SOURCE_SEL(0x07)); // 清除定时器的时钟源配置寄存器
    TMR3_CONL |= TMR_SOURCE_SEL(0x06);    // 配置定时器的时钟源，使用系统时钟 

    __EnableIRQ(TMR3_IRQn); // 使能中断
    IE_EA = 1;              // 打开总中断
}

// 关闭定时器，清空计数值
void tmr3_disable(void)
{
    // 不给定时器提供时钟，让它停止计数
    TMR3_CONL &= ~(TMR_SOURCE_SEL(0x07)); // 清除定时器的时钟源配置寄存器
    TMR3_CONL |= TMR_SOURCE_SEL(0x05);    // 配置定时器的时钟源，不用任何时钟

    // 清除定时器的计数值
    // TMR3_CNTL = 0;
    // TMR3_CNTH = 0;
     TMR_ALLCON = TMR3_CNT_CLR(0x1); // 清除计数值(写1清零)

    __DisableIRQ(TMR3_IRQn); // 关闭中断（不使能中断）
}

// TMR3中断服务函数
void TIMR3_IRQHandler(void) interrupt TMR3_IRQn
{
#if 1 // 定时器的定时中断
    // 进入中断设置IP，不可删除
    __IRQnIPnPush(TMR3_IRQn);

    // ---------------- 用户函数处理 -------------------
    // 周期中断
    if (TMR3_CONH & TMR_PRD_PND(0x1))
    {
        TMR3_CONH |= TMR_PRD_PND(0x1); // 清除pending

        tmr3_cnt++;
    }

    // 退出中断设置IP，不可删除
    __IRQnIPnPop(TMR3_IRQn);
#endif

#if 0 // 定时器的输入捕获中断

    // 进入中断设置IP，不可删除
    __IRQnIPnPush(TMR3_IRQn);

    // ---------------- 用户函数处理 -------------------

    if (TMR3_CONH & TMR_CAP_PND(0x1))
    {
        TMR3_CONH |= TMR_CAP_PND(0x1); // 清除pending

        tmr3_cap_val    = ((TMR3_PWMH << 8) | TMR3_PWML); // 读取捕获的值
        tmr3_cnt++; // 脉冲计数值加一

        TMR_ALLCON = TMR3_CNT_CLR(0x1); // 清除计数值
    }

    // 退出中断设置IP，不可删除
    __IRQnIPnPop(TMR3_IRQn);

#endif
}
