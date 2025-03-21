#include "RPC_Task.h"

// void process_relay_status(const JsonVariantConst &data, JsonDocument &response) {
//     ESP_LOGI("RPC", "Relay Data: %s", data.as<String>().c_str());
//     ESP_LOGI("RPC", "Process Relay Status");
// }

// void process_rgb_status(const JsonVariantConst &data, JsonDocument &response) {
//     ESP_LOGI("RPC", "RGB Data: %s", data.as<String>().c_str());
//     set_blinking_frequency(data.as<uint8_t>());
//     // ESP_LOGI("RPC", "Process RGB Status");
//     // if (data) set_rgb_color(BLUE_RGB);
//     // else set_rgb_color(BLACK_RGB);
// }


void rpc_toggle_auto_manual(const JsonVariantConst &data, JsonDocument &response) {
    ESP_LOGI("RPC", "Toggle Auto Manual Data: %s", data.as<String>().c_str());
    ESP_LOGI("RPC", "Process Toggle Auto Manual");
    if (relay_task_handle == NULL) return;
    if (data.as<uint8_t>()) {
        vTaskResume(relay_task_handle);
    } else {
        vTaskSuspend(relay_task_handle);
    }
}

void process_relay_status_2(const JsonVariantConst &data, JsonDocument &response) {
    ESP_LOGI("RPC", "Relay 2 Data: %s", data.as<String>().c_str());
    ESP_LOGI("RPC", "Process Relay Status");
    digitalWrite(RELAY_CH2, data.as<uint8_t>());
}

void process_relay_status_3(const JsonVariantConst &data, JsonDocument &response) {
    ESP_LOGI("RPC", "Relay 3 Data: %s", data.as<String>().c_str());
    ESP_LOGI("RPC", "Process Relay Status");
    digitalWrite(RELAY_CH3, data.as<uint8_t>());
}

const std::array<RPC_Callback, MAX_RPC_SUBSCRIPTIONS> callbacks = {
    // RPC_Callback {RPC_RELAY_STATUS, process_relay_status},
    // RPC_Callback {RPC_RGB_STATUS, process_rgb_status},
    RPC_Callback {RPC_AUTO_STATUS, rpc_toggle_auto_manual},
    RPC_Callback {RPC_RELAY_STATUS_2, process_relay_status_2},
    RPC_Callback {RPC_RELAY_STATUS_3, process_relay_status_3}
};

bool rpc_setup() {
    // ESP_LOGI("RPC", "Subscribing to RPC methods");
    if (!rpc.RPC_Subscribe(callbacks.cbegin(), callbacks.cend())) {
        ESP_LOGE("RPC", "Failed to subscribe to RPC methods");
        return false;
    }   
    ESP_LOGI("RPC", "Subscribed to RPC methods");
    return true;
}