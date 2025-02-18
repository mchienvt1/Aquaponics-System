#ifndef CONSTANT_H_
#define CONSTANT_H_

/* TASK TIMERS */
constexpr uint16_t WIFI_TIMER = 1000;
constexpr uint16_t RELAY_STATUS_TIMER = 1000;
constexpr uint16_t RELAY_CONTROL_TIMER = 100;
constexpr uint16_t RS485_PROCESS_TIMER = 1000;
constexpr uint16_t RS485_READ_WRITE_TIMER = 1000;
constexpr uint16_t GPS_TIMER = 10000;
constexpr uint16_t MQTT_LOOP_TIMER = 100;
constexpr uint16_t MQTT_RECONNECT_TIMER = 10000;
constexpr uint16_t OTA_TIMER = 10000;
constexpr uint16_t LED_RGB_TIMER = 1000;
constexpr uint16_t INSANELY_LONG_TIMER = 60000;
constexpr uint16_t THINGSBOARD_LOOP_TIMER = 1000;

constexpr char TOKEN[] = "gu7r3jhlx8trp3yqvjng";

constexpr char THINGSBOARD_SERVER[] = "app.coreiot.io";
constexpr uint16_t THINGSBOARD_PORT = 1883U;

constexpr char BLINKING_INTERVAL_ATTR[] = "blinkingInterval";
constexpr char LED_MODE_ATTR[] = "ledMode";
constexpr char LED_STATE_ATTR[] = "ledState";

constexpr std::array<const char*, 2U> SHARED_ATTRIBUTES_LIST = {
    LED_STATE_ATTR,
    BLINKING_INTERVAL_ATTR
};

#endif