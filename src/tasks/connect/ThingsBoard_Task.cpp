#include "ThingsBoard_Task.h"

// volatile bool tb_need_reconnect = false;

// bool currentFWSent = false;
// bool updateRequestSent = false;
bool ota_subscribed = false;
bool rpc_subscribed = false;
bool shared_attributes_subscribed = false;

void thingsboard_task(void *pvParameters) {
    while (true) {
        while (WiFi.status() != WL_CONNECTED) {
            delay(WIFI_TIMER);
        }
        if (!tbClient.connected()) {
            // ESP_LOGI("MQTT", "Connecting to ThingsBoard server %s with token %s", THINGSBOARD_SERVER, SENSOR_TOKEN);
            while (!tbClient.connect(THINGSBOARD_SERVER, DEVICE_TOKEN, THINGSBOARD_PORT)) {
                ESP_LOGI("TB", "Try to connect to ThingsBoard MQTT server");
                delay(THINGSBOARD_CONNECT_TIMER);
            }
            // ESP_LOGI("TB", "Connected to ThingsBoard MQTT server with token %s", DEVICE_TOKEN);
            ESP_LOGI("TB", "Setting up Tasks");
            // shared_attributes_setup();
        }
        if (!ota_subscribed) ota_subscribed = ota_setup();
        if (!rpc_subscribed) rpc_subscribed = rpc_setup();
        tbClient.loop();
        delay(THINGSBOARD_LOOP_TIMER);
    }
}

void thingsboard_task_init() {
    xTaskCreate(thingsboard_task, "ThingsBoard_Task", 4096, NULL, 1, NULL);
}