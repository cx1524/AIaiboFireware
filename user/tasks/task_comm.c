#include "tasks.h"
#include "comm_interface.h"
#include <string.h>

char buffer[] = "Hello World!\r\n";

void task_comm(void* parameter)
{
    while (1)
    {
        comm_interface_send(buffer, strlen(buffer));
        vTaskDelay(1000);
    }
}
