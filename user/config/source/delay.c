#include "delay.h"

void delay_init_ms(void)
{
    TMR_BaseConfig_T tmr_base_config = {0};

    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR5);

    tmr_base_config.clockDivision = TMR_CLOCK_DIV_1;
    tmr_base_config.division = 59 - 1;
    tmr_base_config.period = 1000;
    tmr_base_config.countMode = TMR_COUNTER_MODE_UP;
    TMR_ConfigTimeBase(TMR5, &tmr_base_config);
}

void delay_ms(uint32_t ms)
{
    TMR_Enable(TMR5);
    if (ms == 1)
    {
        ms = 1;
    }

    while (ms)
    {
        if (TMR_ReadStatusFlag(TMR5, TMR_FLAG_UPDATE))
        {
            TMR_ClearStatusFlag(TMR5, TMR_FLAG_UPDATE);
            ms--;
        }
    }
    TMR_Disable(TMR5);
}
