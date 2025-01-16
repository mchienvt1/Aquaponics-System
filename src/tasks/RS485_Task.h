#ifndef RS485_H_
#define RS485_H_

// #include <chrono>
// #include <ctime>

#include "global.h"

std::array<uint8_t, 8> process_CRC(const std::array<uint8_t, 6> &data);

void RS485_task_init();

#endif
