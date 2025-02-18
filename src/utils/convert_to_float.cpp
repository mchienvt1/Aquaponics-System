#include "convert_to_float.h"

float convert_to_float(const std::array<uint8_t, 4> &data) {
    // Combine the bytes into a single unsigned integer
    // The only important bit of this shit
    unsigned int intValue = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];

    // Extract sign, exponent, and mantissa
    unsigned int sign = (intValue >> 31) & 1;
    unsigned int exponent = (intValue >> 23) & 0xFF;
    unsigned int mantissa = intValue & 0x7FFFFF;

    // Calculate the floating-point value
    float result = std::ldexp(static_cast<float>(mantissa) / (1 << 23) + 1.0f, exponent - 127);
    if (sign) {
        result = -result;
    }

    return result;
}