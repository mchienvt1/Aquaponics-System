#ifndef RS485_TASK_H_
#define RS485_TASK_H_

#include "global.h"

class SensorData;

extern bool need_processing;
extern SensorData sensorData;

void send_processed_data();
void sensor_task_init();

#endif
