#include "GPIO_Task.h"

void set_rgb_color(color c) {
    // ESP_LOGI("GPIO", "Change pixel color to #%2x%2x%2x", c.red, c.green, c.blue);
    pixels.clear();
    pixels.setPixelColor(0, c.red, c.green, c.blue);
    pixels.show();
}

void set_led_color() {
    pixels.begin();
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

// void test_relay_control(void *pvParameters) {
//     while (true) {
//         for(int i = 0; i < 6; ++i) {
//             digitalWrite(GPIO_Relay_Pin[i], !digitalRead(GPIO_Relay_Pin[i]));
//             vTaskDelay(60000 / portTICK_PERIOD_MS);
//             ESP_LOGI("GPIO", "Relay %d toggle %d", i, digitalRead(GPIO_Relay_Pin[i]));
//         }
//     }
// }

// void test_rgb(void *pvParameters) {
//     while (true) {
//         set_rgb_color(RED_RGB);
//         vTaskDelay(LED_RGB_TIMER / portTICK_PERIOD_MS);
//         set_rgb_color(GREEN_RGB);
//         vTaskDelay(LED_RGB_TIMER / portTICK_PERIOD_MS);
//         set_rgb_color(BLUE_RGB);
//         vTaskDelay(LED_RGB_TIMER / portTICK_PERIOD_MS);
//     }
// }

void gpio_task(void *pvParameters) {    
    set_led_color();
    set_relay_pin_mode();
    while (true) {
        uint32_t color = pixels.getPixelColor(0);
        pixels.clear();
        delay(1000);
        pixels.setPixelColor(0, color);
        delay(1000);
    }
    vTaskDelete(NULL);
}

void gpio_task_init() {
    xTaskCreate(gpio_task, "GPIO_Task", 4096, NULL, 1, NULL);
    // xTaskCreate(test_relay_control, "", 2048, NULL, 2, NULL);
    // xTaskCreate(test_rgb, "test_rgb", 2048, NULL, 258, NULL);
};