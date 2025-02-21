#include "RPC_Task.h"
// #include "RPC_Callback.h"
#include "RPC_Request_Callback.h"

bool is_subscribed = false;

void process_relay_status(const JsonVariantConst &data, JsonDocument &response) {
    digitalWrite(RELAY_CH1, digitalRead(RELAY_CH1) == HIGH ? LOW : HIGH);
    ESP_LOGI("RPC", "Relay status changed");
}

void rpc_task(void *pvParameters) {
    while (WiFi.status() != WL_CONNECTED) {
        delay(WIFI_TIMER);
    }
    while (!tbClient.connected()) {
        delay(THINGSBOARD_CONNECT_TIMER);
    }
    if (!is_subscribed) {
        // ESP_LOGI("RPC", "Subscribing to RPC methods");
        const std::array<RPC_Callback, MAX_RPC_SUBSCRIPTIONS> rpc_callbacks = {
            RPC_Callback {"processRelayStatus", process_relay_status}
        };
        if (!rpc.RPC_Subscribe(rpc_callbacks.cbegin(), rpc_callbacks.cend())) {
            ESP_LOGE("RPC", "Failed to subscribe to RPC methods");
        }   
        ESP_LOGI("RPC", "Subscribed to RPC methods");
        is_subscribed = true;
    }
    vTaskDelete(NULL);
}

void rpc_task_init() {
    xTaskCreate(rpc_task, "RPC_Task", 4096, NULL, 1, NULL);
}