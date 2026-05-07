#include "device_cfg.h"

static void gpio_init(void);

/* @brief configure USB device clock, NVIC and GPIO */
void device_config(void)
{
    gpio_init();
}

void gpio_init(void)
{
    GPIO_Config_T gpio_init = {0};

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOB);

    gpio_init.pin  = GPIO_PIN_6 | GPIO_PIN_7;
    gpio_init.mode = GPIO_MODE_OUT_PP;
    gpio_init.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOB, &gpio_init);
}
