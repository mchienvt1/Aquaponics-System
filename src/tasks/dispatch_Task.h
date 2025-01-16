#ifndef DISPATCH_TASK_H_
#define DISPATCH_TASK_H_

#include "global.h"
#include "MQTT_Task.h"
#include "OTA_Task.h"
#include "GPS_Task.h"
#include "GPIO_Task.h"
#include "RS485_Task.h"
#include "server_Task.h"
#include "wifi_Task.h"

void dispatch();    

#endif