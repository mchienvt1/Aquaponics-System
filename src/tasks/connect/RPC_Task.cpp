#include "RPC_Task.h"

void process_relay_status(const JsonVariantConst &data, JsonDocument &response) {
    ESP_LOGI("RPC", "Relay Data: %s", data.as<String>().c_str());
    ESP_LOGI("RPC", "Process Relay Status");
}

void process_rgb_status(const JsonVariantConst &data, JsonDocument &response) {
    ESP_LOGI("RPC", "RGB Data: %s", data.as<String>().c_str());
    // response["color"] = pixels.getPixelColor(0);
    ESP_LOGI("RPC", "Process RGB Status");
}
