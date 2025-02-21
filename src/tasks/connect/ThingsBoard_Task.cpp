#include "ThingsBoard_Task.h"

// bool is_subscribed = false;

// void process_relay_status(const JsonVariantConst &data, JsonDocument &response) {
//     digitalWrite(RELAY_CH1, digitalRead(RELAY_CH1) == HIGH ? LOW : HIGH);
// }

void thingsboard_task(void *pvParameters) {
    while (WiFi.status() != WL_CONNECTED) {
        delay(WIFI_TIMER);
    }
    if (!tbClient.connected()) {
        // ESP_LOGI("MQTT", "Connecting to ThingsBoard server %s with token %s", THINGSBOARD_SERVER, SENSOR_TOKEN);
        while (!tbClient.connect(THINGSBOARD_SERVER, DEVICE_TOKEN, THINGSBOARD_PORT)) {
            ESP_LOGI("TB", "Connecting to ThingsBoard MQTT server");
            delay(THINGSBOARD_CONNECT_TIMER);
        }
        ESP_LOGI("TB", "Connected to ThingsBoard MQTT server with token %s", DEVICE_TOKEN);
        // const std::array<RPC_Callback, MAX_RPC_SUBSCRIPTIONS> rpc_callbacks = {
        //     RPC_Callback {"processRelayStatus", process_relay_status}
        // };
        // if (!rpc.RPC_Subscribe(rpc_callbacks.cbegin(), rpc_callbacks.cend())) {
        //     ESP_LOGE("RPC", "Failed to subscribe to RPC methods");
        // }   
        // ESP_LOGI("RPC", "Subscribed to RPC methods");
        // is_subscribed = true;
    }
    while (true) {
        tbClient.loop();
        delay(THINGSBOARD_LOOP_TIMER);
    }
}

void thingsboard_task_init() {
    xTaskCreate(thingsboard_task, "ThingsBoard_Task", 4096, NULL, 1, NULL);
}