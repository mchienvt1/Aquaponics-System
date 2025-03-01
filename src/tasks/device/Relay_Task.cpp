#include "Relay_Task.h"

bool need_control = false;

uint8_t relay_ch = 0;
uint8_t state = 0;
uint64_t duration = 0;

void pump_setup() {
    relay_ch = 1;
    state = 1;
    duration = 5000U;
    need_control = true;
}

void parse_payload(const char* payload) {
    ESP_LOGI("RELAY", "Payload: %s", payload);

    // Payload format: field-target-value-interval
    // Example: relay.controller-1-1-10
    // Parsing the field token which we don't need
    char* token = strtok((char*)payload, "-");
    
    // Parsing the target token
    token = strtok(nullptr, "-");
    // Serial.println(token);
    relay_ch = atoi(token);

    // Parsing the value token
    token = strtok(nullptr, "-");
    // Serial.println(token);
    state = atoi(token);

    // Parsing the interval token
    token = strtok(nullptr, "-");
    // Serial.println(token);
    duration = atoi(token);
    
    need_control = true;
}

void relay_task(void *pvParameters) {
    while (true) {
        if (need_control && sensor_task_handle != NULL) {
            vTaskSuspend(sensor_task_handle);
            unsigned long start = millis();
            digitalWrite(relay_ch, state);
            while (millis() - start <= duration) continue;
            need_control = false;
            digitalWrite(relay_ch, 1 - state);
            vTaskResume(sensor_task_handle);
        }
        delay(1000);
    }
}

void relay_task_init() {
    xTaskCreate(relay_task, "Relay_Task", 2048, NULL, 1, NULL);
    // xTaskCreate(relay_control_task, "relay_control_task", 2048, nullptr, 2, nullptr);
}