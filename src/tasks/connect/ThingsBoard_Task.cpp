#include "ThingsBoard_Task.h"

volatile bool tb_need_reconnect = false;

void thingsboard_task(void *pvParameters) {
    while (WiFi.status() != WL_CONNECTED) {
        delay(WIFI_TIMER);
    }
    if (!tbClient.connected()) {
        // ESP_LOGI("MQTT", "Connecting to ThingsBoard server %s with token %s", THINGSBOARD_SERVER, SENSOR_TOKEN);
        while (!tbClient.connect(THINGSBOARD_SERVER, DEVICE_TOKEN, THINGSBOARD_PORT)) {
            ESP_LOGI("TB", "Connecting to ThingsBoard MQTT server");
            delay(THINGSBOARD_CONNECT_TIMER);
        }
        ESP_LOGI("TB", "Connected to ThingsBoard MQTT server with token %s", DEVICE_TOKEN);
    }
    while (true) {
        if (!tbClient.connected()) {
            if (!tb_need_reconnect) {
                tb_need_reconnect = true;
                ESP_LOGI("TB", "Disconnected.");
                tbClient.disconnect();
                while (!tbClient.connect(THINGSBOARD_SERVER, DEVICE_TOKEN, THINGSBOARD_PORT)) {
                    ESP_LOGI("TB", "Reconnecting to ThingsBoard MQTT server");
                    delay(THINGSBOARD_CONNECT_TIMER);
                }
            }
        }
        if (tb_need_reconnect && tbClient.connected()) {
            tb_need_reconnect = false;
        }
        tbClient.loop();
        tb_connected = tbClient.connected();
        delay(THINGSBOARD_LOOP_TIMER);
    }
}

void thingsboard_task_init() {
    xTaskCreate(thingsboard_task, "ThingsBoard_Task", 4096, NULL, 1, NULL);
}