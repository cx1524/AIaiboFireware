#include "device_cfg.h"

void gpio_config(void)
{
    GPIO_Config_T gpio_init = {0};

    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_OTGFS);

    gpio_init.mode = GPIO_MODE_AF_PP;
    gpio_init.pin  = GPIO_PIN_11 | GPIO_PIN_12;
    gpio_init.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOA, &gpio_init);
}
