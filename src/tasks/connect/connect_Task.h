#ifndef CONNECT_TASK_H_
#define CONNECT_TASK_H_

#include "global.h"
#include "WiFi_Task.h"
#include "ThingsBoard_Task.h"
#include "MQTT_Task.h"
#include "OTA_Task.h"
#include "RPC_Task.h"

extern OTA_Firmware_Update<> ota;
extern Server_Side_RPC<MAX_RPC_SUBSCRIPTIONS, MAX_RPC_RESPONSE> rpc;
extern const std::array<IAPI_Implementation*, MAX_APIS> apis;

extern WiFiClient wifiClient;
extern Arduino_MQTT_Client mqttClient;
extern ThingsBoard tbClient;

// extern bool tb_connected = false;

void connect_task_init();

#endif