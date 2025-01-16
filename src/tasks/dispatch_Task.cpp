#include "dispatch_Task.h"

void dispatch() {
    ESP_LOGI("DISPATCH", "Initializing tasks");
    // Initialize GPIO
    // gpio_task_init();

    // Initialize RS485
    // RS485_task_init();

    // Initialize Wi-Fi
    wifi_task_init();

    // Initialize GPS
    // GPS_task_init();

    // Initialize MQTT
    // mqtt_task_init();

    // Initialize OTA
    // OTA_task_init();

    // Initialize Server
    // server_task_init();
}
