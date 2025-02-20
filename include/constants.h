#ifndef CONSTANT_H_
#define CONSTANT_H_

// GPIO //
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

// GPS //
struct gps_location {
    double lat, lng;
};


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

constexpr std::array<const char*, 2U> SHARED_ATTRIBUTES_LIST = {
    LED_STATE_ATTR,
    BLINKING_INTERVAL_ATTR
};

#endif