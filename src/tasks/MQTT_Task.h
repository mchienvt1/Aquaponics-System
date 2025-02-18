#ifndef MQTT_TASK_H_
#define MQTT_TASK_H_

#include "global.h"

void publish_data(String type_topic, String data);

void subscribe(String type_topic);

void mqtt_task_init();

#endif // MQTT_TASK_H_