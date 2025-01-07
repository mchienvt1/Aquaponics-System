#ifndef CRC16_H_
#define CRC16_H_

#include "global.h"

// This is the CRC-16-IBM calculation
std::array<uint8_t, 8> ProcessCRC(const std::array<uint8_t, 6> &data);
// Could be changed since this only calculates the static 6 bytes of data

#endif
