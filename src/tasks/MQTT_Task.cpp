#include "MQTT_Task.h"

WiFiClient wifiClient;
Arduino_MQTT_Client mqttClient(wifiClient);
ThingsBoard tb(mqttClient);

void mqtt_task() {
    // Await WiFi connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(WIFI_TIMER);
    }
       
    if (!tb.connected()) {
        ESP_LOGI("TB", "Connecting to ThingsBoard server %s with token %s", THINGSBOARD_SERVER, TOKEN);
        if (tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) {
            ESP_LOGI("TB", "Connected to ThingsBoard server");

            tb.sendTelemetryData("temperature", 25.0);

            while (true) {
                tb.loop();
                delay(THINGSBOARD_LOOP_TIMER);
            }
        }
      }
}

void mqtt_task_init() {
    xTaskCreate(mqtt_task, "MQTT_Task", 4096, NULL, 1, NULL);
}