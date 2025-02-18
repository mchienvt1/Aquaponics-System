#include "MQTT_Task.h"

WiFiClient wifiClient;
Arduino_MQTT_Client mqttClient(wifiClient);
ThingsBoard tb(mqttClient, MAX_MESSAGE_RECEIVE_SIZE, MAX_MESSAGE_SEND_SIZE);

void publish_data(String data) {
    tb.sendTelemetryString(data.c_str());
}

void mqtt_task(void *pvParameters) {
    // Await WiFi connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(WIFI_TIMER);
    }
       
    if (!tb.connected()) {
        // ESP_LOGI("MQTT", "Connecting to ThingsBoard server %s with token %s", THINGSBOARD_SERVER, SENSOR_TOKEN);
        if (tb.connect(THINGSBOARD_SERVER, SENSOR_TOKEN, THINGSBOARD_PORT)) {
            ESP_LOGI("MQTT", "Connected to ThingsBoard MQTT server");

            while (true) {
                tb.loop();
                delay(MQTT_LOOP_TIMER);
            }
        }
      }
}

void mqtt_task_init() {
    xTaskCreate(mqtt_task, "MQTT_Task", 4096, NULL, 1, NULL);
}