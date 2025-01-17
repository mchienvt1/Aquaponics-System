#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <array>
#include <map>
#include <fcntl.h>
// #include <termios.h>
#include <unistd.h>

// Global Libraries for the project
#include "WiFi.h"
#include "SPIFFS.h"
#include "DHT20.h"
#include "HardwareSerial.h"
// #include "SoftwareSerial.h"
#include "TinyGPSPlus.h"
#include "Adafruit_NeoPixel.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "PubSubClient.h"
#include "esp_partition.h"
#include "esp_system.h"
#include "esp_event.h"
#include <esp_err.h>
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_wifi.h"
#include "nvs.h"
#include "nvs_flash.h"

// #ifdef RS485_H_
// #endif

// #ifdef GPS_H_
// #endif

// Libraries for OTA Update
#ifdef OTA_H_
#include <HTTPClient.h>
#include "esp_http_client.h"
#include "esp_https_ota.h"
#endif

// Libraries for AsyncWebServer
#ifdef SERVER_H_
#include "ESPAsyncWebServer.h"
// #include "AsyncJson.h"
#endif

// Include configs
#include "../src/configs/defines.h"
#include "../src/configs/sensor.h"

// Include utils
#include "../src/utils/utils.h"

// Include tasks
#include "../src/tasks/dispatch_Task.h"

#endif