#include "Relay_Task.h"

// bool need_control = false;

uint8_t relay_ch = RELAY_CH1;
uint8_t state = 1;
uint64_t duration = 0;

// void pump_setup() {
//     relay_ch = 1;
//     state = 1;
//     duration = 5000U;
//     need_control = true;
// }

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
    
    // need_control = true;
}

uint32_t PUMP_TIMER = 10000u;
uint32_t STAB_TIMER = 10000u;
uint32_t SEND_TIMER = 10000u * 1u;

DATA_COLLECTION_STATE data_collection_state = INIT;

void relay_task(void *pvParameters) {
    // TODO: change this to a state machine
    unsigned long timer = millis();
    while (true) {
        switch (data_collection_state) {
            case INIT:
                data_collection_state = PUMP;
                digitalWrite(RELAY_CH1, 1);
                ESP_LOGI("RELAY", "INIT");
                break;
            case PUMP:
                if (millis() - timer >= PUMP_TIMER) {
                    data_collection_state = STABILIZE;
                    digitalWrite(RELAY_CH1, 1);
                    ESP_LOGI("RELAY", "STATE = STABILIZE");
                    timer = millis();
                }
                break;
                case STABILIZE:
                if (millis() - timer >= STAB_TIMER) {
                    data_collection_state = COLLECT;
                    need_processing = true;
                    ESP_LOGI("RELAY", "STATE = COLLECT");
                    timer = millis();
                }
                break;
            case COLLECT:
                if (millis() - timer >= SEND_TIMER) {
                    send_processed_data();
                    need_processing = false;
                    data_collection_state = PUMP;
                    digitalWrite(RELAY_CH1, 0);
                    ESP_LOGI("RELAY", "STATE = PUMP");
                    timer = millis();
                }
                break;
            default:
                break;
        }
        delay(10);
    }
}

void relay_task_init() {
    // xTaskCreate(relay_task, "Relay_Task", 4096, NULL, 1, &relay_task_handle);
    // xTaskCreate(relay_control_task, "relay_control_task", 2048, nullptr, 2, nullptr);
}