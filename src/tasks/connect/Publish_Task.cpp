#include "Publish_Task.h"

// StaticJsonDocument<MAX_MESSAGE_SEND_SIZE> telemetry_data;

void update_telemetry_data(String data) {
    tbClient.sendTelemetryString(data.c_str());
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

void publish_wifi_attributes() {
    tbClient.sendAttributeData("rssi", WiFi.RSSI());
    tbClient.sendAttributeData("channel", WiFi.channel());
    tbClient.sendAttributeData("bssid", WiFi.BSSIDstr().c_str());
    tbClient.sendAttributeData("localIp", WiFi.localIP().toString().c_str());
    tbClient.sendAttributeData("ssid", WiFi.SSID().c_str());    
}

void subscribe_attribute_task(void *pvParameters) {
    // Await WiFi connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(WIFI_TIMER);
    }
    while (true) {
        publish_wifi_attributes();
        delay(5000);
    }
}

void publish_task_init() {
    xTaskCreate(subscribe_attribute_task, "Subscribe_Attribute_Task", 2048, NULL, 1, NULL);
}