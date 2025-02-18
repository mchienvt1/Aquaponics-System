#ifndef RELAY_TASK_H_
#define RELAY_TASK_H_

#include "defines.h"

struct relay_data {
    uint8_t relay_ch;
    uint8_t state;
    uint8_t duration;
};

void relay_task_init();

#endif