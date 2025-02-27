#include "Attribute_Task.h"

// StaticJsonDocument<MAX_MESSAGE_SEND_SIZE> telemetry_data;

void update_data(const String &data) {
    if (!tbClient.connected()) return;
    ESP_LOGI("TB", "Upload Data %s", data.c_str());
    tbClient.sendTelemetryString(data.c_str());
    tbClient.sendAttributeString(data.c_str());
}

void processSharedAttributeUpdate(const JsonObjectConst &data) {
    for (auto it = data.begin(); it != data.end(); ++it) {
      Serial.println(it->key().c_str());
      // Shared attributes have to be parsed by their type.
      Serial.println(it->value().as<const char*>());
    }
  
    const size_t jsonSize = Helper::Measure_Json(data);
    char buffer[jsonSize];
    serializeJson(data, buffer, jsonSize);
    Serial.println(buffer);
}

// void subscribe_attribute_task(void *pvParameters) {
//     // Await WiFi connection
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(WIFI_TIMER);
//     }
//     while (true) {
//         tbClient.sendTelemetryData("rgb_value", pixels.getPixelColor(0));
//         publish_wifi_attributes();
//         delay(5000);
//     }
// }

// void publish_task_init() {
//     xTaskCreate(subscribe_attribute_task, "Subscribe_Attribute_Task", 4096, NULL, 1, NULL);
// }