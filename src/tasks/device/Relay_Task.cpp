#include "Relay_Task.h"

uint8_t relay_ch = 0;
uint8_t relay_state = 0;
uint8_t duration = 0;

void parse_payload(const char* payload) {
    return;
}

void set_duration(uint8_t d) {
    duration = d;
}

void relay_task_init() {
    // xTaskCreate(relay_status_task, "relay_status_task", 2048, nullptr, 1, nullptr);
    // xTaskCreate(relay_control_task, "relay_control_task", 2048, nullptr, 2, nullptr);
}