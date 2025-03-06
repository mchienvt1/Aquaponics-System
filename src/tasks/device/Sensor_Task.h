#ifndef RS485_TASK_H_
#define RS485_TASK_H_

#include "global.h"

extern bool need_processing;

void send_processed_data();
void sensor_task_init();

#endif
