#include "connect_Task.h"

// Initialize used apis
OTA_Firmware_Update<> ota;
Server_Side_RPC<MAX_RPC_SUBSCRIPTIONS, MAX_RPC_RESPONSE> rpc;
const std::array<IAPI_Implementation*, MAX_APIS> apis = {
    &ota,
    &rpc
};

WiFiClient wifiClient;
Arduino_MQTT_Client mqttClient(wifiClient);
ThingsBoard tbClient(mqttClient, MAX_MESSAGE_RECEIVE_SIZE, MAX_MESSAGE_SEND_SIZE, Default_Max_Stack_Size, apis);

bool tb_connected = false;

void connect_task_init() {
    wifi_task_init();
    thingsboard_task_init();
    while (!tb_connected) {
        continue;
    }
    publish_task_init();
    rpc_task_init();
    ota_task_init();
}