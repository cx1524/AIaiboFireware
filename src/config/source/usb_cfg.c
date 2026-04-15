#include "usb_cfg.h"
#include "apm32f402_403_rcm.h"
#include "apm32f402_403_gpio.h"

static void usb_msp_init(void);

void usb_init(void)
{
    usb_msp_init();
}

static void usb_msp_init(void)
{
    GPIO_Config_T gpio_init = {0};

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);

    /* PA11: OTG_DM PA12: OTG_DP */
    gpio_init.pin  = GPIO_PIN_11 | GPIO_PIN_12;
    gpio_init.mode = GPIO_MODE_AF_PP;
    gpio_init.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOA, &gpio_init);
}
