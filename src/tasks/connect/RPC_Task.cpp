#include "RPC_Task.h"

// void rpc_toggle_auto_manual(const JsonVariantConst &data, JsonDocument &response) {
//     ESP_LOGI("RPC", "Toggle Auto Manual Data: %s", data.as<String>().c_str());
//     ESP_LOGI("RPC", "Process Toggle Auto Manual");
//     if (relay_task_handle == NULL) return;
//     if (data.as<uint8_t>()) {
//         vTaskResume(relay_task_handle);
//     } else {
//         vTaskSuspend(relay_task_handle);
//     }
// }

// void process_relay_status_2(const JsonVariantConst &data, JsonDocument &response) {
//     ESP_LOGI("RPC", "Relay 2 Data: %s", data.as<String>().c_str());
//     ESP_LOGI("RPC", "Process Relay Status");
//     digitalWrite(RELAY_CH2, data.as<uint8_t>());
// }

// void process_relay_status_3(const JsonVariantConst &data, JsonDocument &response) {
//     ESP_LOGI("RPC", "Relay 3 Data: %s", data.as<String>().c_str());
//     ESP_LOGI("RPC", "Process Relay Status");
//     digitalWrite(RELAY_CH3, data.as<uint8_t>());
// }

void process_switch_relay_change(const JsonVariantConst &data, JsonDocument &response){
    int relayIndexes[] = {RELAY_CH1, RELAY_CH2, RELAY_CH3};
    int relayIndex = data["relayIndex"];
    int value = data["value"] ? 1 : 0;
    if(relayIndex < 3){
        digitalWrite(relayIndexes[relayIndex], value);
    }
    std::string key = "relay_" + std::to_string(relayIndex + 1) + "_status";;
    nlohmann::json j = {};
    j[key] = value; 
 
    sendAttribute(String(j.dump().c_str()));
}
void process_check_health(const JsonVariantConst &data, JsonDocument &response){
    response["status"] = true;
}


const std::array<RPC_Callback, MAX_RPC_SUBSCRIPTIONS> callbacks = {
    // RPC_Callback {RPC_RELAY_STATUS, process_relay_status},
    // RPC_Callback {RPC_RGB_STATUS, process_rgb_status},
    // RPC_Callback {RPC_AUTO_STATUS, rpc_toggle_auto_manual},
    // RPC_Callback {RPC_RELAY_STATUS_2, process_relay_status_2},
    // RPC_Callback {RPC_RELAY_STATUS_3, process_relay_status_3},
    // RPC_Callback {RPC_RELAY_STATUS_3, process_relay_status_3},

    RPC_Callback {RPC_SWITCH_RELAY_CHANGE, process_switch_relay_change},
    RPC_Callback {RPC_CHECK_HEALTH, process_check_health},
};


bool rpc_setup() {
    if (!rpc.RPC_Subscribe(callbacks.cbegin(), callbacks.cend())) {
        ESP_LOGE("RPC", "Failed to subscribe to RPC methods");
        return false;
    }   
    ESP_LOGI("RPC", "Subscribed to RPC methods");
    return true;
}