#ifndef __TASKS_H__
#define __TASKS_H__

#include "FreeRTOS.h"
#include "task.h"

void task_comm(void* parameter);
void task_led1(void* parameter);
void task_led2(void* parameter);

#endif
