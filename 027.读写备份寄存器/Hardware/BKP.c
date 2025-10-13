#include "stm32f10x.h"

void BKP_Init(void)
{
    // 开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); // PWR
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE); // BKP

    // 启用对RTC和备份寄存器的访问
    PWR_BackupAccessCmd(ENABLE);
}