#ifndef GPIO_H_
#define GPIO_H_

#include "global.h"
#include "../configs/esp32_relay_6ch_config.h"

extern int GPIO_Relay_Pin[6];

void gpio_task_init();

#endif