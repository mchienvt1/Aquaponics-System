#include "device_Task.h"

Adafruit_NeoPixel pixels(NUM_PIXELS, LED_RGB, NEO_GRB + NEO_KHZ800);
TaskHandle_t sensor_task_handle = NULL;
TaskHandle_t relay_task_handle = NULL;

void device_task_init()
{
    gpio_task_init();
    relay_task_init();
    sensor_task_init();
    gps_task_init();
    // dht_task_init();
}
