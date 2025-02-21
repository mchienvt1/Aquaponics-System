#include "Publish_Task.h"

StaticJsonDocument<MAX_MESSAGE_SEND_SIZE> telemetry_data;

bool sensor_data_updated = false;
bool gps_data_updated = false;

void update_sensor_data(StaticJsonDocument<MAX_MESSAGE_SEND_SIZE> sensor_data) {
    if (sensor_data_updated) return;
    telemetry_data.add(sensor_data);
    sensor_data_updated = true;
}

void update_gps_data(StaticJsonDocument<MAX_MESSAGE_SEND_SIZE> gps_data) {
    if (gps_data_updated) return;
    telemetry_data.add(gps_data);
    gps_data_updated = true;
}

void publish_telemetry_task(void *pvParameters) {
    // Await WiFi connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(WIFI_TIMER);
    }
    while (!tbClient.connected()) {
        delay(THINGSBOARD_CONNECT_TIMER);
    }
    while (true) {
        if (!tbClient.sendTelemetryJson(telemetry_data, MAX_MESSAGE_SEND_SIZE)) {
            ESP_LOGE("TELEMETRY", "Failed to send telemetry data");
        }
        telemetry_data.clear();
        sensor_data_updated = false;
        gps_data_updated = false;
        delay(TELEMETRY_TIMER);
    }
}

void publish_task_init() {
    xTaskCreate(publish_telemetry_task, "Publish_Telemetry_Task", 2048, NULL, 1, NULL);
}