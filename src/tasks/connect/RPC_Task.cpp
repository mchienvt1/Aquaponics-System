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

bool rpc_setup() {
    // ESP_LOGI("RPC", "Subscribing to RPC methods");
    const std::array<RPC_Callback, MAX_RPC_SUBSCRIPTIONS> callbacks = {
        RPC_Callback {RPC_RELAY_STATUS, process_relay_status},
        RPC_Callback {RPC_RGB_STATUS, process_rgb_status}
    };
    if (!rpc.RPC_Subscribe(callbacks.cbegin(), callbacks.cend())) {
        ESP_LOGE("RPC", "Failed to subscribe to RPC methods");
        return false;
    }   
    ESP_LOGI("RPC", "Subscribed to RPC methods");
    return true;
}