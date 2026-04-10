#include "usbd_core.h"
#include "usbd_cdc_acm.h"

#include <stdio.h>
#include "device_cfg.h"

int main(void) {
    //printf("Hello, world!\n");
    device_config();

    while (1)
    {
        delay_ms(500);
        GPIO_SetBit(GPIOB, GPIO_PIN_6 | GPIO_PIN_7);
        delay_ms(500);
        GPIO_ResetBit(GPIOB, GPIO_PIN_6 | GPIO_PIN_7);
    }
}


