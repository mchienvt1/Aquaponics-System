#ifndef CONSTANT_H_
#define CONSTANT_H_

#define DEVELOP "1"

/* BOARD */
// Information
#define MY_BOARD "ESP32-AQUAPONICS"
#define MY_VERSION "1.0.0"
// #define BOARD_ID 2003

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

#define TXD_GPS 9       // GPS AT6668
#define RXD_GPS 10      // GPS AT6668
#define DHT_SDA 8     // Sensor DHT
#define DHT_SCL 7      // Sensor DHT
#define TXD_RS485 17    // Sensor ISx
#define RXD_RS485 18    // Sensor ISx

#define LED_RGB 38
#define NUM_PIXELS 1
#define Brightness 39 // Set brightness to (0 to 255)

#define BUZZER 21 // relay_6_ch

struct color {
    uint8_t red, green, blue;
};

constexpr color RED_RGB = {34, 139, 34};
constexpr color GREEN_RGB = {220, 20, 60};
constexpr color BLUE_RGB = {60, 0, 255};
constexpr color WHITE_RGB = {255, 255, 255};
constexpr color BLACK_RGB = {0, 0, 0};

constexpr uint8_t GPIO_Relay_Pin[6] = {
    RELAY_CH1, RELAY_CH2, RELAY_CH3, RELAY_CH4, RELAY_CH5, RELAY_CH6
};

/* SENSORs */
// Sensor ISx Status
#define ISHC 
#define ISDC 
#define ISEC

// Sensor IDs
constexpr uint8_t ISHC_ID = 0x02;
constexpr uint8_t ISDC_ID = 0x10;
constexpr uint8_t ISEC_ID = 0x04;

// Sensor Commands
constexpr uint8_t SENSOR_DATA_COUNT = 7;
#define MEASURE_TEMP "temperature"
#define MEASURE_PH "pH"
#define MEASURE_DO "DO"
#define MEASURE_CONDUCT "EC"
#define MEASURE_SALI "salinity"
#define MEASURE_TDS "tds"
#define MEASURE_RESIS "resistance"

/* TASK Timers */
// CONNECT Tasks Timers
#define WIFI_TIMER 5000
#define THINGSBOARD_LOOP_TIMER 1000
#define THINGSBOARD_CONNECT_TIMER 1000
#define TELEMETRY_TIMER 5000
#define OTA_TIMER 10000
#define NTP_LOOP_TIMER 10000
#define NTP_CONNECT_TIMER 2000

// DEVICE Tasks Timers
#define GPS_TIMER 10000
#define LED_RGB_TIMER 1000
#define INSANELY_LONG_TIMER 60000
#define SENSOR_ISX_PROCESS_TIMER 1000
#define SENSOR_ISX_READ_WRITE_TIMER 20
#define SENSOR_DHT_TIMER 2000
#define SENSOR_CLEAR_TIMEOUT 10
#define SENSOR_DATA_PROCESS_TIME 10000

/* GPS */
#define GPS_DATA_COUNT 2
#define GPS_LATITUDE "latitude"
#define GPS_LONGITUDE "longitude"

/* WiFi Credentials */
#define WIFI_SSID "QuocPhong"
#define WIFI_PASSWORD "quocphong2003"

/* ThingsBoard Credentials */
#define THINGSBOARD_SERVER "app.coreiot.io"
#define THINGSBOARD_PORT 1883U
#define DEVICE_TOKEN "5ww7zny85HcDyOgKnhBI"
constexpr uint16_t MAX_MESSAGE_SEND_SIZE = 512U;
constexpr uint16_t MAX_MESSAGE_RECEIVE_SIZE = 512U;
constexpr uint8_t MAX_APIS = 5U;

/* OTA */
// https://github.com/thingsboard/thingsboard-client-sdk/tree/master/examples/0009-esp8266_esp32_process_OTA_MQTT
constexpr char CURRENT_FIRMWARE_TITLE[] = MY_BOARD;
constexpr char CURRENT_FIRMWARE_VERSION[] = MY_VERSION;
constexpr uint8_t FIRMWARE_FAILURE_RETRIES = 12U;
constexpr uint16_t FIRMWARE_PACKET_SIZE = 4096U;

// RPC //
// https://github.com/thingsboard/thingsboard-client-sdk/tree/master/examples/0010-esp8266_esp32_rpc
constexpr uint8_t MAX_RPC_SUBSCRIPTIONS = 9U;
constexpr uint8_t MAX_RPC_RESPONSE = 5U;

constexpr const char RPC_AUTO_STATUS[] = "rpc_toggle_auto_manual";
constexpr const char RPC_RELAY_STATUS_2[] = "process_relay_status_2";
constexpr const char RPC_RELAY_STATUS_3[] = "process_relay_status_3";
constexpr const char RPC_SWITCH_RELAY_CHANGE[] = "process_switch_relay_change";
constexpr const char RPC_CHECK_HEALTH[] = "process_check_health";

/* SHARED ATTRIBUTES */
// https://github.com/thingsboard/thingsboard-client-sdk/blob/master/examples/0006-esp8266_esp32_process_shared_attribute_update
constexpr const char BLINKING_INTERVAL_ATTR[] = "blinkingInterval";
constexpr const char RGB_MODE_ATTR[] = "rgb_mode";
constexpr const char RGB_VALUE_ATTR[] = "rgb_value";
constexpr const char RELAY_COMMAND_ATTR[] = "command";
constexpr const char RELAY_1_STATUS[] = "relay_1_status";
constexpr const char RELAY_2_STATUS[] = "relay_2_status";
constexpr const char RELAY_3_STATUS[] = "relay_3_status";

constexpr uint8_t MAX_ATTRIBUTES_SUBSCRIPTIONS = 1U;
constexpr uint8_t MAX_ATTRIBUTES = 10U;
constexpr std::array<const char*, MAX_ATTRIBUTES> SHARED_ATTRIBUTES_LIST = {
    RELAY_COMMAND_ATTR,
    RGB_MODE_ATTR,
    RGB_VALUE_ATTR,
    RELAY_1_STATUS,
    RELAY_2_STATUS,
    RELAY_3_STATUS
};

constexpr std::array<const char *, MAX_ATTRIBUTES> REQUESTED_SHARED_ATTRIBUTES = {RELAY_1_STATUS};
const std::vector<const char *> REQUESTED_CLIENT_ATTRIBUTES = {RELAY_1_STATUS,RELAY_2_STATUS,RELAY_3_STATUS};


constexpr uint64_t REQUEST_TIMEOUT_MICROSECONDS = 5000U * 1000U;


#endif