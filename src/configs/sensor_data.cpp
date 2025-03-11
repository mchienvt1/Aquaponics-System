#include "sensor_data.h"

SensorData::SensorData() {
    sensor_data_map[0] = std::make_pair(MEASURE_TEMP, default_value);
    sensor_data_map[1] = std::make_pair(MEASURE_PH, default_value);
    sensor_data_map[2] = std::make_pair(MEASURE_DO, default_value);
    sensor_data_map[3] = std::make_pair(MEASURE_CONDUCT, default_value);
    sensor_data_map[4] = std::make_pair(MEASURE_SALI, default_value);
    sensor_data_map[5] = std::make_pair(MEASURE_TDS, default_value);
    sensor_data_map[6] = std::make_pair(MEASURE_RESIS, default_value);
}

SensorData::~SensorData() {}

void SensorData::set_data(const std::string &data_name, const float &value) {
    for (auto &data : sensor_data_map) {
        if (data.first == data_name) {
            data.second = value;
            break;
        }
    }
}

void SensorData::set_data(uint8_t index, const float &value) {
    sensor_data_map[index].second = value;
}

float SensorData::get_data(const std::string &data_name) {
    for (auto &data : sensor_data_map) {
        if (data.first == data_name) {
            return data.second;
        }
    }
    return default_value;
}

float SensorData::get_data(uint8_t index) {
    return sensor_data_map[index].second;
}

String SensorData::format_raw_data() {
    String res = "{";
    for (auto &data : sensor_data_map) {
        res += "\"raw_" + String(data.first.c_str()) + "\":" + String(data.second) + ",";
    }
    res[res.length() - 1] = '}';
    return res;
}

String SensorData::format_processed_data() {
    String res = "{";
    for (auto &data : sensor_data_map) {
        res += "\"processed_" + String(data.first.c_str()) + "\":" + String(data.second) + ",";
    }
    res[res.length() - 1] = '}';
    return res;
}