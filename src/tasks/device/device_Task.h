#ifndef DEVICE_TASK_H_
#define DEVICE_TASK_H_

#include "global.h"
#include "GPIO_Task.h"
#include "Sensor_Task.h"
#include "GPS_Task.h"
#include "Relay_Task.h"
#include "DHT_Task.h"

extern Adafruit_NeoPixel pixels;
extern TaskHandle_t sensor_task_handle;
extern TaskHandle_t relay_task_handle;

void device_task_init();

#endif