#ifndef AI_TASK_H
#define AI_TASK_H

#include "global.h"
#include "../src/configs/sensor_data.h"
#include "../device/Sensor_Task.h"

extern bool ai_tasks_running;


void AI_Task_Init();
void AI_Task_Stop();

#endif // AI_TASK_H