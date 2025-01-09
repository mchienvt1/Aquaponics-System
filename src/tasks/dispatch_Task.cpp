#include "dispatch_Task.h"

void dispatch_task_init() {
    gpio_task_init();
    wifi_task_init();
    server_task_init();
    RS485_task_init();
    mqtt_task_init();
}