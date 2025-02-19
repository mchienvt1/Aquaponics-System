#ifndef GPIO_TASK_H_
#define GPIO_TASK_H_

#include "global.h"

void set_rgb_color(color c);
void write_relay_pin(String pin, uint8_t mode);
void gpio_task_init();

#endif