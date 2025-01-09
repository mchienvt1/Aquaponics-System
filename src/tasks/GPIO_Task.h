#ifndef GPIO_H_
#define GPIO_H_

#include "global.h"
#include "../configs/esp32_relay_6ch_config.h"

void gpio_write(uint8_t gpio_pin, uint8_t value);
void gpio_toggle(uint8_t gpio_pin);
void gpio_task_init();

#endif