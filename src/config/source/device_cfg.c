#include "device_cfg.h"

static void rcc_config(void);
static void rcc_deinit(void);
static void nvic_config(void);
static void nvic_deinit(void);
static void gpio_config(void);
static void gpio_deinit(void);

/* @brief configure USB device clock, NVIC and GPIO */
void device_config(void)
{
    rcc_config();
    nvic_config();
    gpio_config();
}

void device_deinit(void)
{
    rcc_deinit();
    nvic_deinit();
    gpio_deinit();
}

void rcc_config(void)
{
    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_OTGFS);
}

void rcc_deinit(void)
{
    RCM_DisableAHBPeriphClock(RCM_AHB_PERIPH_OTGFS);
}

void nvic_config(void)
{
    NVIC_EnableIRQ(OTG_FS_IRQn);
}

void nvic_deinit(void)
{
    NVIC_DisableIRQ(OTG_FS_IRQn);
}

void gpio_config(void)
{
    GPIO_Config_T gpio_init = {0};

    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_OTGFS);

    gpio_init.mode = GPIO_MODE_AF_PP;
    gpio_init.pin  = GPIO_PIN_11 | GPIO_PIN_12;
    gpio_init.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOA, &gpio_init);
}

void gpio_deinit(void)
{
    GPIO_Reset(GPIOA);
}
