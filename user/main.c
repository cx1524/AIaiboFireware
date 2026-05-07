#include "device_cfg.h"
#include "tasks.h"

int main(void)
{
    device_config();

    xTaskCreate(task_led1, "led1", 1024, NULL, 1, NULL);
    xTaskCreate(task_led2, "led2", 1024, NULL, 1, NULL);
    xTaskCreate(task_comm, "comm", 1024, NULL, 1, NULL);

    vTaskStartScheduler();
}
