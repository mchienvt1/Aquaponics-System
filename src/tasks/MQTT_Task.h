#ifndef MQTT_TASK_H
#define MQTT_TASK_H

#include <PubSubClient.h>

#include "global.h"
#include "wifi_Task.h"
#include "GPIO_Task.h"
#include "../configs/server_config.h"
#include "../configs/esp32_relay_6ch_config.h"

extern WiFiClient espClient;
extern PubSubClient client;

void publish_data(String type, String feed, String data);

void subscribe(String type, String feed);

void callback(char* topic, byte* payload, unsigned int length);

void mqtt_task_init();

#endif // MQTT_TASK_H