#ifndef CONSTANT_H_
#define CONSTANT_H_

/* BOARD */
// Information
#define MY_BOARD "ESP32-S3-RELAY-6CH"
#define BOARD_ID 2003
#define MY_VERSION "1.0.2"

// BAUD RATE
#define MAIN_BAUDRATE 115200
#define GPS_BAUDRATE 115200
#define RS485_BAUDRATE 9600

// PINs
#define BOOT 0
#define NUM_RELAY 6
#define RELAY_CH1 1
#define RELAY_CH2 2
#define RELAY_CH3 41
#define RELAY_CH4 42
#define RELAY_CH5 45
#define RELAY_CH6 46 
#define TXD_RS485 17 // relay_6_ch
#define RXD_RS485 18 // relay_6_ch
#define TXD_GPS 11 // GPS
#define RXD_GPS 12 // GPS
#define LED_RGB 38
#define NUM_PIXELS 1
#define Brightness 39 // Set brightness to (0 to 255)
#define BUZZER 21 // relay_6_ch

struct color {
    uint8_t red, green, blue;
};

constexpr color RED_RGB = {220, 20, 60};
constexpr color GREEN_RGB = {34, 139, 34};
constexpr color BLUE_RGB = {100, 149, 237};
constexpr color WHITE_RGB = {255, 255, 255};
constexpr color BLACK_RGB = {0, 0, 0};

constexpr uint8_t GPIO_Relay_Pin[6] = {
    RELAY_CH1,
    RELAY_CH2,
    RELAY_CH3, 
    RELAY_CH4,
    RELAY_CH5,
    RELAY_CH6
};

/* SENSORs */
// Sensor Status
#define ISHC 
#define ISDC 
#define ISEC

// Sensor IDs
#define ISHC_ID 0x02
#define ISDC_ID 0x10
#define ISEC_ID 0x04

// Sensor Commands
#define SENSOR_DATA_COUNT 7
#define MEASURE_TEMP "temperature"
#define MEASURE_PH "pH"
#define MEASURE_DO "DO"
#define MEASURE_CONDUCT "EC"
#define MEASURE_SALI "salinity"
#define MEASURE_TDS "tds"
#define MEASURE_RESIS "resistance"

/* TASK Timers */
#define WIFI_TIMER 1000
// #define RELAY_STATUS_TIMER 1000
// #define RELAY_CONTROL_TIMER 100
#define RS485_PROCESS_TIMER 1000
#define RS485_READ_WRITE_TIMER 1000
#define GPS_TIMER 10000
#define THINGSBOARD_LOOP_TIMER 1000
#define THINGSBOARD_CONNECT_TIMER 1000
#define TELEMETRY_TIMER 5000
#define OTA_TIMER 10000
#define LED_RGB_TIMER 1000
#define INSANELY_LONG_TIMER 60000

// GPS //
#define GPS_DATA_COUNT 2
#define GPS_LATITUDE "latitude"
#define GPS_LONGITUDE "longitude"

// WiFi Credentials //
#define WIFI_SSID "w"
#define WIFI_PASSWORD "tuideptrai"

// ThingsBoard Credentials //
#define THINGSBOARD_SERVER "app.coreiot.io"
#define THINGSBOARD_PORT 1883U
#define DEVICE_TOKEN "fl2qilkqmt9fx6c9i7bf"
#define MAX_MESSAGE_SEND_SIZE 512U
#define MAX_MESSAGE_RECEIVE_SIZE 512U
#define MAX_APIS 2U

// OTA //
// Firmware title and version used to compare with remote version, to check if an update is needed.
// Title needs to be the same and version needs to be different --> downgrading is possible
constexpr char CURRENT_FIRMWARE_TITLE[] = MY_BOARD;
constexpr char CURRENT_FIRMWARE_VERSION[] = MY_VERSION;

// Maximum amount of retries we attempt to download each firmware chunck over MQTT
constexpr uint8_t FIRMWARE_FAILURE_RETRIES = 12U;
// Size of each firmware chunck downloaded over MQTT,
// increased packet size, might increase download speed
constexpr uint16_t FIRMWARE_PACKET_SIZE = 4096U;

// RPC //
constexpr char BLINKING_INTERVAL_ATTR[] = "blinkingInterval";
constexpr char LED_MODE_ATTR[] = "ledMode";
constexpr char LED_STATE_ATTR[] = "ledState";
constexpr uint8_t MAX_RPC_SUBSCRIPTIONS = 3U;
constexpr uint8_t MAX_RPC_RESPONSE = 5U;

constexpr std::array<const char*, 2U> SHARED_ATTRIBUTES_LIST = {
    LED_STATE_ATTR,
    BLINKING_INTERVAL_ATTR
};

#endif