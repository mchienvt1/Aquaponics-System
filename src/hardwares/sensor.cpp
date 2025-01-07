#include "sensor.h"

void Sensor::AddCommand(const std::string &command_name, std::array<uint8_t, 6> &raw_command_map) {
    std::array<uint8_t, 6> data;
    for (size_t i = 0; i < 6; i++) {
        data[i] = raw_command_map[i];
    }
    std::array<uint8_t, 8> commandWithCRC = ProcessCRC(data);
    command_map[command_name] = commandWithCRC;
}

std::array<uint8_t, 8> Sensor::GetCommand(const std::string &command_name) {
    auto it = command_map.find(command_name);
    if (it != command_map.end()) {
        return it->second;
    }
    return {};
};

float Sensor::ProcessValue(const uint8_t *buffer) {
    std::array<uint8_t, 4> data = {buffer[5], buffer[6], buffer[3], buffer[4]};
    return ConvertToFloat(data);
}

void Sensor::PrintCommand() {
    for (auto it = command_map.begin(); it != command_map.end(); ++it) {
        // std::cout << it->first << "\n";
    }
}

