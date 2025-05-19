#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <array>
#include <list>
#include <map>
#include <vector>
#include <fcntl.h>
// #include <termios.h>
#include <unistd.h>
#include <nlohmann/json.hpp>
#include "esp_partition.h"
#include "esp_system.h"
#include "esp_event.h"
#include <esp_err.h>
#include "esp_log.h"
#include <cstdio>
// #include "esp_ota_ops.h"
// #include "esp_wifi.h"
#include "esp_sntp.h"

#include "nvs.h"
#include "nvs_flash.h"

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
// #include "PubSubClient.h"
#include "ThingsBoard.h"
#include "Arduino_MQTT_Client.h"
// #include "ArduinoJson.h"
#include <Espressif_Updater.h>
#include <OTA_Firmware_Update.h>
// #include "RPC_Request_Callback.h"
// #include <Server_Side_RPC.h>
// // #include <Client_Side_RPC.h>
#include <Shared_Attribute_Update.h>
#include <Attribute_Request.h>
// Libraries for AsyncWebServer
#ifdef SERVER_TASK_H_
#include "ESPAsyncWebServer.h"
// #include "AsyncJson.h"
#endif

// Global constants and structs
#include "constants.h"

// Include configs
#include "../src/configs/sensor.h"
#include "../src/configs/sensor_data.h"
#include "../src/configs/gps_data.h"

// Include utils
#include "../src/utils/utils.h"

// Include tasks
#include "../src/tasks/connect/connect_Task.h"
#include "../src/tasks/device/device_Task.h"
#include "../src/tasks/dispatch.h"

#endif
