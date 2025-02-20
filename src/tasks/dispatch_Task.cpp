#include "dispatch_Task.h"

void dispatch() {
    gpio_task_init();
    rs485_task_init();
    wifi_task_init();
    gps_task_init();
    mqtt_task_init();
    // ota_task_init();
    // server_task_init();
}
