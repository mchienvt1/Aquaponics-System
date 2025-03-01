#include "Attribute_Task.h"

void processSharedAttributeUpdate(const JsonObjectConst &data) {
    for (auto it = data.begin(); it != data.end(); ++it) {
        // Serial.println(it->key().c_str());
        // Shared attributes have to be parsed by their type.
        // Serial.println(it->value().as<const char*>());
        
        // Parsing RELAY_COMMAND_ATTR attribute
        if (strcmp(it->key().c_str(), RELAY_COMMAND_ATTR) == 0) {
            parse_payload(it->value().as<const char*>());
        }
    }
  
    // const size_t jsonSize = Helper::Measure_Json(data);
    // char buffer[jsonSize];
    // serializeJson(data, buffer, jsonSize);
    // Serial.println(buffer);
}

bool shared_attributes_setup() {
    const Shared_Attribute_Callback<MAX_ATTRIBUTES> callback(
        &processSharedAttributeUpdate,
        SHARED_ATTRIBUTES_LIST.cbegin(), 
        SHARED_ATTRIBUTES_LIST.cend()
    );
    if (!shared_attributes.Shared_Attributes_Subscribe(callback)) {
        ESP_LOGE("SHARED_ATTR", "Failed to subscribe to shared attributes");
        return false;
    }
    ESP_LOGI("SHARED_ATTR", "Subscribed to shared attributes");
    return true;
}
