#include "Publish_Task.h"

// StaticJsonDocument<MAX_MESSAGE_SEND_SIZE> telemetry_data;

bool shared_attributes_subscribed = false;

void update_telemetry_data(String data) {
    tbClient.sendTelemetryString(data.c_str());
}

/// @brief Update callback that will be called as soon as one of the provided shared attributes changes value,
/// if none are provided we subscribe to any shared attribute change instead
/// @param data Data containing the shared attributes that were changed and their current value
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

void attribute_task(void *pvParameters) {
    // Await WiFi connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(WIFI_TIMER);
    }
    while (!tb_connected) {
        delay(THINGSBOARD_CONNECT_TIMER);
    }
    if (!shared_attributes_subscribed) {
        const Shared_Attribute_Callback<MAX_ATTRIBUTES> callback(processSharedAttributeUpdate, SHARED_ATTRIBUTES_LIST);
        if (!shared_attributes.Shared_Attributes_Subscribe(callback)) {
            ESP_LOGE("SHARED_ATTR", "Failed to subscribe to shared attributes");
        }
        ESP_LOGI("SHARED_ATTR", "Subscribed to shared attributes");
        shared_attributes_subscribed = true;        
    }
    vTaskDelete(NULL);
}    

void publish_task_init() {
    xTaskCreate(attribute_task, "Attribute_Task", 2048, NULL, 1, NULL);
}