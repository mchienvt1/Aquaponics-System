#include "calculate_CRC.h"

uint16_t calculate_CRC(const std::array<uint8_t, 6> &data) {

    // Initialize 16-bit register 0xFFFF as CRC value
    uint16_t crc = 0xFFFF;

    // For each byte in data
    for (size_t i = 0; i < data.size(); ++i) {

        // XOR first byte of data[i] with CRC value 
        crc ^= data[i];

        // for each bit in data[i]
        for(size_t j = 0; j < 8; ++j) {

            // Get the last CRC bit then shift left CRC by 1 bit
            size_t last_CRC_bit = crc & 1;
            crc >>= 1;

            // XOR CRC with 0xA001 if last CRC bit is 1 else do nothing
            if (last_CRC_bit) crc ^= 0xA001;
        }
    }

    // Return result
    return crc;
}

std::array<uint8_t, 8> process_CRC(const std::array<uint8_t, 6> &data) {

    // Initialize empty command and CRC
    size_t data_size = data.size();
    std::array<uint8_t, 8> command;
    uint16_t crc = calculate_CRC(data);

    // Append old data to command
    for(size_t i = 0; i < data_size; ++i) {
        command[i] = data[i];
    }

    // Append CRC to command
    command[data_size] = static_cast<uint8_t>(crc & 0xFF);
    command[data_size + 1] = static_cast<uint8_t>((crc >> 8) & 0xFF); // Because it's an 8-bit command

    return command;
}
