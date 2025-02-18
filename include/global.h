#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <array>
#include <map>
#include <fcntl.h>
// #include <termios.h>
#include <unistd.h>
#include "esp_partition.h"
#include "esp_system.h"
#include "esp_event.h"
#include <esp_err.h>
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_wifi.h"
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
#include "PubSubClient.h"
#include "ThingsBoard.h"
#include "Arduino_MQTT_Client.h"

// Libraries for OTA Update
#ifdef OTA_TASK_H_
#include <HTTPClient.h>
#include "esp_http_client.h"
#include "esp_https_ota.h"
#endif

// Libraries for AsyncWebServer
#ifdef SERVER_TASK_H_
#include "ESPAsyncWebServer.h"
// #include "AsyncJson.h"
#endif

// Global defines and constants
#include "defines.h"
// Global structs and classes
#include "constants.h"

// Include configs
#include "../src/configs/sensor.h"
#include "../src/configs/sensor_data.h"

// Include utils
#include "../src/utils/utils.h"

// Include tasks
#include "../src/tasks/GPIO_Task.h"
#include "../src/tasks/Relay_Task.h"
#include "../src/tasks/GPS_Task.h"
#include "../src/tasks/server_Task.h"
#include "../src/tasks/RS485_Task.h"
#include "../src/tasks/dispatch_Task.h"
#include "../src/tasks/MQTT_Task.h"
#include "../src/tasks/OTA_Task.h"
#include "../src/tasks/wifi_Task.h"

// Global variables
extern WiFiClient wifiClient;
extern PubSubClient psClient;
extern Arduino_MQTT_Client mqttClient;
extern ThingsBoard tb;

#endif