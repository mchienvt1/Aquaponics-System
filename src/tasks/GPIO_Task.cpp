#include "GPIO_Task.h"

void gpio_write(uint8_t gpio_pin, uint8_t value) {
    digitalWrite(gpio_pin, value);
}

void gpio_toggle(uint8_t gpio_pin) {
    digitalWrite(gpio_pin, !digitalRead(gpio_pin));
}

void gpio_task(void *pvParameters) {
    
    
    pinMode(CH1, OUTPUT);
    pinMode(CH2, OUTPUT);
    pinMode(CH3, OUTPUT);
    pinMode(CH4, OUTPUT);
    pinMode(CH5, OUTPUT);
    pinMode(CH6, OUTPUT);
    
    vTaskDelete(NULL);
}

void gpio_task_init() {
    xTaskCreate(gpio_task, "GPIO_Task", 4096, NULL, 1, NULL);
};