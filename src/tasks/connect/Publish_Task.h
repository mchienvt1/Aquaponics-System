#ifndef PUBLISH_TASK_H_
#define PUBLISH_TASK_H_

#include "global.h"

// void publish_data(String data);

void update_sensor_data(StaticJsonDocument<MAX_MESSAGE_SEND_SIZE> sensor_data);
void update_gps_data(StaticJsonDocument<MAX_MESSAGE_SEND_SIZE> gps_data);
void publish_task_init();

#endif