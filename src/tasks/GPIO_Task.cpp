#include "GPIO_Task.h"

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED_RGB, NEO_GRB + NEO_KHZ800);

int GPIO_Relay_Pin[6] = {
    RELAY_CH1, RELAY_CH2, RELAY_CH3, RELAY_CH4, RELAY_CH5, RELAY_CH6
};

const Color RED_RGB = {255, 0, 0};
const Color GREEN_RGB = {0, 255, 0};
const Color BLUE_RGB = {0, 0, 255};

void set_rgb_color(Color c) {
    return;
    pixels.setPixelColor(0, pixels.Color(c.red, c.green, c.blue));
    pixels.show();
}

void write_relay_pin(String pin, uint8_t mode) {
    digitalWrite(GPIO_Relay_Pin[std::atoi(pin.c_str()) - 1], mode);
}

void set_relay_pin_mode() {
    pinMode(RELAY_CH1, OUTPUT);
    pinMode(RELAY_CH2, OUTPUT);
    pinMode(RELAY_CH3, OUTPUT);
    pinMode(RELAY_CH4, OUTPUT);
    pinMode(RELAY_CH5, OUTPUT);
    pinMode(RELAY_CH6, OUTPUT);
}

void set_uart_pin_mode() {
    pinMode(RXD_GPS, INPUT);
    pinMode(TXD_GPS, OUTPUT);
}

void gpio_task(void *pvParameters) {
    pixels.begin();
    set_relay_pin_mode();
    set_uart_pin_mode();
    vTaskDelete(NULL);
}

void gpio_task_init() {
    xTaskCreate(gpio_task, "GPIO_Task", 4096, NULL, 1, NULL);
};