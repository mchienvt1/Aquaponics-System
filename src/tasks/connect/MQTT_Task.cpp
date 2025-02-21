#include "MQTT_Task.h"

void publish_data(String data) {
    tbClient.sendTelemetryString(data.c_str());
    tbClient.sendAttributeString(data.c_str());
}

void mqtt_task(void *pvParameters) {
    // Await WiFi connection
    while (WiFi.status() != WL_CONNECTED || !tbClient.connected()) {
        delay(WIFI_TIMER);
    }
    vTaskDelete(NULL);
}

void mqtt_task_init() {
    xTaskCreate(mqtt_task, "MQTT_Task", 2048, NULL, 1, NULL);
}