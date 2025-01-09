#ifndef RS485_H_
#define RS485_H_

// #include <fcntl.h>
// #include <termios.h>
// #include <unistd.h>
// #include <chrono>
// #include <ctime>

#include "global.h"
#include "../configs/esp32_relay_6ch_config.h"
#include "../configs/sensor_config.h"
#include "../hardwares/Sensor.h"
// #include "MQTT_Task.h"

// #include "neural_network/neural_network.h"

void RS485_task_init();

#endif
