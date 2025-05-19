#include "connect_Task.h"

// Initialize used apis
OTA_Firmware_Update<> ota;
Shared_Attribute_Update<MAX_ATTRIBUTES_SUBSCRIPTIONS, MAX_ATTRIBUTES> shared_attributes;
Attribute_Request<2U, MAX_ATTRIBUTES> attr_request;
const std::array<IAPI_Implementation *, MAX_APIS> apis = {
    &ota,
    &shared_attributes,
    &attr_request};

WiFiClient wifiClient;
Arduino_MQTT_Client mqttClient(wifiClient);
ThingsBoard tbClient(mqttClient, MAX_MESSAGE_RECEIVE_SIZE, MAX_MESSAGE_SEND_SIZE, Default_Max_Stack_Size, apis);

void connect_task_init()
{
    wifi_task_init();
    // ntp_task_init();
    thingsboard_task_init();
}