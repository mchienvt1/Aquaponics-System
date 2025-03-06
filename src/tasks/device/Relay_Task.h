#ifndef RELAY_TASK_H_
#define RELAY_TASK_H_

#include "global.h"

enum DATA_COLLECTION_STATE {
    INIT, PUMP, STABILIZE, COLLECT
};

// void pump_setup();
void parse_payload(const char* payload);
void relay_task_init();

#endif