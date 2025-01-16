#ifndef UTILS_H_
#define UTILS_H_

#include "global.h"

// Convert 4 bytes to float
float convert_to_float(const std::array<uint8_t, 4> &data);

// Calculate CRC16
std::array<uint8_t, 8> process_CRC(const std::array<uint8_t, 6> &data);

#endif // UTILS_H_