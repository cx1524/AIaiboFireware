#include "tasks.h"
#include "apm32f402_403_gpio.h"

void task_led2(void* parameter)
{
    while (1)
    {
        GPIO_SetBit(GPIOB, GPIO_PIN_7);
        vTaskDelay(500);
        GPIO_ResetBit(GPIOB, GPIO_PIN_7);
        vTaskDelay(250);
    }
}
