#ifndef THINGSBOARD_TASK_H_
#define THINGSBOARD_TASK_H_

#include "global.h"

void sendTelemetry(const String&data);
void sendAttribute(const String&data);
void update_sensor_data(const String &data);
void update_gps_data(const String &data);
void update_wifi_data(const String &data);
void update_env_data(const String &data);
void update_process_data(const String &data);

void thingsboard_task_init();

#endif