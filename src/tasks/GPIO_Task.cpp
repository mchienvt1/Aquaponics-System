#include "GPIO_Task.h"

int GPIO_Relay_Pin[6] = {
    RELAY_CH1,
    RELAY_CH2,
    RELAY_CH3,
    RELAY_CH4,
    RELAY_CH5,
    RELAY_CH6
};

void set_relay_pin_mode() {
    pinMode(RELAY_CH1, OUTPUT);
    pinMode(RELAY_CH2, OUTPUT);
    pinMode(RELAY_CH3, OUTPUT);
    pinMode(RELAY_CH4, OUTPUT);
    pinMode(RELAY_CH5, OUTPUT);
    pinMode(RELAY_CH6, OUTPUT);
}

void gpio_task(void *pvParameters) {
    set_relay_pin_mode();
    vTaskDelete(NULL);
}

void gpio_task_init() {
    xTaskCreate(gpio_task, "GPIO_Task", 4096, NULL, 1, NULL);
};