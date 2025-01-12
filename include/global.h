#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <array>
#include <map>

#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "DHT20.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Sensors
#define ISHC 
// #define ISDC 
// #define ISEC 

// Baudrate
#define BAUDRATE 115200

// MQTT server details



// OTA Config
#define OTA_URL_SIZE 256
#define HASH_LEN 32
#define FIREBASE_DATABASE_URL "https://ota-test-8caf0-default-rtdb.firebaseio.com/"
#define OTA_RECV_TIMEOUT_MS 5000

#endif