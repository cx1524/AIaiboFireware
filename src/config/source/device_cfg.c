#include "device_cfg.h"
#include "usb_cfg.h"

static void rcc_init(void);
static void rcc_deinit(void);
static void nvic_init(void);
static void nvic_deinit(void);
static void gpio_init(void);
static void gpio_deinit(void);

/* @brief configure USB device clock, NVIC and GPIO */
void device_config(void)
{
    delay_init_ms();
    gpio_init();
    usb_init();
}

void device_deinit(void)
{
    gpio_deinit();
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

void gpio_deinit(void)
{
    GPIO_Reset(GPIOB);
}
