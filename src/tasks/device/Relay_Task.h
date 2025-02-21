#ifndef RELAY_TASK_H_
#define RELAY_TASK_H_

#include "global.h"

struct relay_data {
    uint8_t relay_ch;
    uint8_t state;
    uint8_t duration;
};

void set_callback();
void set_duration(uint8_t d);
void relay_task_init();

#endif