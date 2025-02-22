#include "RPC_Task.h"
// #include "RPC_Callback.h"
#include "RPC_Request_Callback.h"

bool rpc_subscribed = false;

/// @brief Processes function for RPC call "processRelayStatus"
/// JsonVariantConst is a JSON variant, that can be queried using operator[]
/// See https://arduinojson.org/v5/api/jsonvariant/subscript/ for more details
/// @param data Data containing the rpc data that was called and its current value
/// @param response Data containgin the response value, any number, string or json, that should be sent to the cloud. Useful for getMethods
void process_relay_status(const JsonVariantConst &data, JsonDocument &response) {
    digitalWrite(RELAY_CH1, digitalRead(RELAY_CH1) == HIGH ? LOW : HIGH);
    ESP_LOGI("RPC", "Relay status changed");
}

void rpc_task(void *pvParameters) {
    while (WiFi.status() != WL_CONNECTED) {
        delay(WIFI_TIMER);
    }
    while (!tb_connected) {
        delay(THINGSBOARD_CONNECT_TIMER);
    }
    if (!rpc_subscribed) {
        // ESP_LOGI("RPC", "Subscribing to RPC methods");
        const std::array<RPC_Callback, MAX_RPC_SUBSCRIPTIONS> callbacks = {
            RPC_Callback {"processRelayStatus", process_relay_status}
        };
        if (!rpc.RPC_Subscribe(callbacks.cbegin(), callbacks.cend())) {
            ESP_LOGE("RPC", "Failed to subscribe to RPC methods");
        }   
        ESP_LOGI("RPC", "Subscribed to RPC methods");
        rpc_subscribed = true;
    }
    vTaskDelete(NULL);
}

void rpc_task_init() {
    xTaskCreate(rpc_task, "RPC_Task", 4096, NULL, 1, NULL);
}