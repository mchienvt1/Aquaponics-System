#include "Relay_Task.h"

bool has_callback = false;
bool need_control = false;

uint8_t relay_ch = 0;
uint8_t duration = 0;

void set_callback() {
    Serial.println("Set Callback true");
    has_callback = true;
}

void set_duration(uint8_t d) {
    duration = d;
}

// void process_relay_payload(byte* payload) {
//     relay_data data;

//     // Payload format: field-target-value-interval
//     // Example: relay.controller-1-1-10
//     // Parsing the field token which we don't need
//     char* token = strtok((char*)payload, "-");
    
//     // Parsing the target token
//     token = strtok(nullptr, "-");
//     data.relay_ch = atoi(token);

//     // Parsing the value token
//     token = strtok(nullptr, "-");
//     data.state = atoi(token);

//     // Parsing the interval token
//     token = strtok(nullptr, "-");
//     data.duration = atoi(token);

//     digitalWrite(data.relay_ch, data.state);
//     // publish_data(RELAY_ACK, String(data.relay_ch) + "-" + String(data.state));
//     delay(data.duration);
//     digitalWrite(data.relay_ch, !data.state);
//     need_control = false;
// }

void relay_status_task(void *pvParameters) {
    while (true) {
        if (has_callback) {
            Serial.println("Set Control true");
            need_control = true;
            has_callback = false;
        }
        delay(RELAY_STATUS_TIMER);
    }
}

void relay_control_task(void *pvParameters) {
    while (true) {
        if (need_control) {
            // Process relay control
            digitalWrite(RELAY_CH1, HIGH);
            delay(duration);
            digitalWrite(RELAY_CH1, LOW);
            need_control = false;
            duration = 0;
        }
        delay(RELAY_CONTROL_TIMER);
    }
}

void relay_task_init() {
    xTaskCreate(relay_status_task, "relay_status_task", 2048, nullptr, 1, nullptr);
    xTaskCreate(relay_control_task, "relay_control_task", 2048, nullptr, 2, nullptr);
}