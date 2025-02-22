#include "RPC_Task.h"
// #include "RPC_Callback.h"
#include "RPC_Request_Callback.h"

void process_relay_status(const JsonVariantConst &data, JsonDocument &response) {
    ESP_LOGI("RPC", "Data: %s", data.as<String>().c_str());
    ESP_LOGI("RPC", "Process Relay Status");
}

void process_rgb_status(const JsonVariantConst &data, JsonDocument &response) {
    ESP_LOGI("RPC", "Process RGB Status");
}
