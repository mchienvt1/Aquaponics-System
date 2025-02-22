#include "Publish_Task.h"

// StaticJsonDocument<MAX_MESSAGE_SEND_SIZE> telemetry_data;

bool sensor_data_updated = false;
bool gps_data_updated = false;

void update_sensor_data(String sensor_data) {
    tbClient.sendTelemetryString(sensor_data.c_str());
}

void update_gps_data(String gps_data) {
    tbClient.sendTelemetryString(gps_data.c_str());
}

// void publish_telemetry_task(void *pvParameters) {
//     // Await WiFi connection
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(WIFI_TIMER);
//     }
//     while (!tb_connected) {
//         delay(THINGSBOARD_CONNECT_TIMER);
//     }
//     while (true) {
//         delay(TELEMETRY_TIMER);
//     }
// }    

void publish_task_init() {
    // xTaskCreate(publish_telemetry_task, "Publish_Telemetry_Task", 2048, NULL, 1, NULL);
}