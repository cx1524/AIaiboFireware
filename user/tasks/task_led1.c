#include "tasks.h"
#include "apm32f402_403_gpio.h"

void task_led1(void* parameter)
{
    while (1)
    {
        GPIO_SetBit(GPIOB, GPIO_PIN_6);
        vTaskDelay(250);
        GPIO_ResetBit(GPIOB, GPIO_PIN_6);
        vTaskDelay(500);
    }
}

