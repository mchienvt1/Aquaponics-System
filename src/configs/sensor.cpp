#include "sensor.h"

Sensor::Sensor(uint8_t id, const std::map<std::string, std::array<uint8_t, 6>> &raw_command_map) {
    this->id = id;
    for (auto &raw_command : raw_command_map) {
        std::array<uint8_t, 8> command = process_CRC(raw_command.second);
        command_map[raw_command.first] = command;
    }
}

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

#ifdef ISHC
void Sensor_ISHC::publish(const std::string &command_name, const float &value) {
    // Publish data to MQTT server
    // Topic: <BOARD_ID>/SensorISHC/<COMMAND_NAME>
    ESP_LOGI("ISHC", "Publishing data to topic SensorISHC/%s with value: %f", command_name.c_str(), value);
    publish_data("sensor", String(command_name.c_str()), String(value, 2));
}
#endif

#ifdef ISDC
void Sensor_ISDC::publish(const std::string &command_name, const float &value) {
    // Publish data to MQTT server
    // Topic: <BOARD_ID>/SensorISDC/<COMMAND_NAME>
    ESP_LOGI("ISDC", "Publishing data to topic SensorISDC/%s with value: %f", command_name.c_str(), value);
    publish_data("sensor", String(command_name.c_str()), String(value, 2));
}
#endif

#ifdef ISEC
void Sensor_ISEC::publish(const std::string &command_name, const float &value) {
    // Publish data to MQTT server
    // Topic: <BOARD_ID>/SensorISEC/<COMMAND_NAME>
    ESP_LOGI("ISEC", "Publishing data to topic SensorISEC/%s with value: %f", command_name.c_str(), value);
    publish_data("sensor", String(command_name.c_str()), String(value, 2));
}
#endif

void Sensor::print_command() {
    for (auto it = command_map.begin(); it != command_map.end(); ++it) {
        // std::cout << it->first << "\n";
    }
}
