#include "GPIO_Task.h"

void GPIO_Write(uint8_t gpio_pin, uint8_t value) {
    digitalWrite(gpio_pin, value);
}

void GPIO_Toggle(uint8_t gpio_pin) {
    digitalWrite(gpio_pin, !digitalRead(gpio_pin));
}

void GPIO_Task(void *pvParameters) {
    pinMode(CH1, OUTPUT);
    pinMode(CH2, OUTPUT);
    pinMode(CH3, OUTPUT);
    pinMode(CH4, OUTPUT);
    pinMode(CH5, OUTPUT);
    pinMode(CH6, OUTPUT);
    
    vTaskDelete(NULL);
}

void GPIOTask_INIT() {
    xTaskCreate(GPIO_Task, "GPIO_Task", 4096, NULL, 1, NULL);
};