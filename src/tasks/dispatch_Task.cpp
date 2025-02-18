#include "dispatch_Task.h"

void dispatch() {
    gpio_task_init();
    RS485_task_init();
    wifi_task_init();
    GPS_task_init();
    mqtt_task_init();
    // OTA_task_init();
    // server_task_init();
}
