#ifndef GPIO_TASK_H_
#define GPIO_TASK_H_

#include "global.h"

void set_blinking_frequency(uint8_t freq);
void set_rgb_color(color c);
void gpio_task_init();

#endif