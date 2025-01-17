#ifndef MQTT_TASK_H_
#define MQTT_TASK_H_

#include "global.h"

extern WiFiClient espClient;
extern PubSubClient psClient;

void publish_data(String type, String feed, String data);

void subscribe(String type, String feed);

void mqtt_task_init();

#endif // MQTT_TASK_H_