#include "device_Task.h"

void device_task_init() {
    gpio_task_init();
    sensor_task_init();
    gps_task_init();
    relay_task_init();
}
