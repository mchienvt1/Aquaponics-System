#include "GPIO_Task.h"

uint8_t blinking_freq = 1;

void set_blinking_frequency(uint8_t freq) {
    blinking_freq = freq;
}

void set_rgb_color(color c) {
    pixels.setPixelColor(0, c.red, c.green, c.blue);
    pixels.show();
    pixels.setBrightness(Brightness);
}

void set_led_color() {
    pixels.begin();
    set_rgb_color(RED_RGB);
    pixels.setBrightness(Brightness);
}

void set_relay_pin_mode() {
    pinMode(RELAY_CH1, OUTPUT);
    pinMode(RELAY_CH2, OUTPUT);
    pinMode(RELAY_CH3, OUTPUT);
    pinMode(RELAY_CH4, OUTPUT);
    pinMode(RELAY_CH5, OUTPUT);
    pinMode(RELAY_CH6, OUTPUT);
}

void gpio_task(void *pvParameters) {    
    set_led_color();
    set_relay_pin_mode();
    vTaskDelete(NULL);
}

void blink_rgb_task(void *pvParameters) {
    uint32_t pixel_color = pixels.getPixelColor(0);
    // bool isOn = false;
    while (true) {
        pixels.setPixelColor(0, pixel_color);
        pixels.show();
        delay(1000 / blinking_freq);
        pixel_color = pixels.getPixelColor(0);
        set_rgb_color(BLACK_RGB);
        pixels.show();
        delay(1000 / blinking_freq);
    }
}

void gpio_task_init() {
    xTaskCreate(gpio_task, "GPIO_Task", 4096, NULL, 1, NULL);
    xTaskCreate(blink_rgb_task, "Blink_RGB_Task", 2048, NULL, 2, NULL);
    // xTaskCreate(test_relay_control, "", 2048, NULL, 2, NULL);
    // xTaskCreate(test_rgb, "test_rgb", 2048, NULL, 258, NULL);
};