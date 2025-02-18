#ifndef CALCULATE_CRC_H_
#define CALCULATE_CRC_H_

#include "global.h"

std::array<uint8_t, 8> process_CRC(const std::array<uint8_t, 6> &data);

#endif