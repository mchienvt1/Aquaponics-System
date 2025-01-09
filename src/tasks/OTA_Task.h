#ifndef OTA_H
#define OTA_H

#include <HTTPClient.h>

#include "global.h"
#include "wifi_Task.h"
#include "GPIO_Task.h"
#include "esp_partition.h"
#include "esp_system.h"
#include "esp_event.h"
#include <esp_err.h>
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "esp_wifi.h"
#include "nvs.h"
#include "nvs_flash.h"

void OTA_task_init();

#endif // OTA_H