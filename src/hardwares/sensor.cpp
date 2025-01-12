#include "sensor.h"

void Sensor::add_command(const std::string &command_name, std::array<uint8_t, 6> &raw_command_map) {
    std::array<uint8_t, 6> data;
    for (size_t i = 0; i < 6; i++) {
        data[i] = raw_command_map[i];
    }
    std::array<uint8_t, 8> command_with_crc = process_CRC(data);
    command_map[command_name] = command_with_crc;
}

std::array<uint8_t, 8> Sensor::get_command(const std::string &command_name) {
    auto it = command_map.find(command_name);
    if (it != command_map.end()) {
        return it->second;
    }
    return {};
};

float Sensor::process_value(const uint8_t *buffer) {
    // Check if the buffer contains data from the sensor
    if (buffer[0] != id) {
        return -1.0;
    }

    // Parse the data for value
    std::array<uint8_t, 4> data = {buffer[5], buffer[6], buffer[3], buffer[4]};
    return convert_to_float(data);
};

void Sensor_ISHC::publish(const std::string &command_name, const float &value) {
    // Publish data to MQTT server
    // Topic: <BOARD_ID>/SensorISHC/<COMMAND_NAME>
    publish_data("SensorISHC", String(command_name.c_str()), String(value, 2));
}

void Sensor_ISDC::publish(const std::string &command_name, const float &value) {
    // Publish data to MQTT server
    // Topic: <BOARD_ID>/SensorISDC/<COMMAND_NAME>
    publish_data("SensorISDC", String(command_name.c_str()), String(value, 2));
}

void Sensor_ISEC::publish(const std::string &command_name, const float &value) {
    // Publish data to MQTT server
    // Topic: <BOARD_ID>/SensorISEC/<COMMAND_NAME>
    publish_data("SensorISEC", String(command_name.c_str()), String(value, 2));
}

void Sensor::print_command() {
    for (auto it = command_map.begin(); it != command_map.end(); ++it) {
        // std::cout << it->first << "\n";
    }
}
