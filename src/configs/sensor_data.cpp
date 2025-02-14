#include "sensor_data.h"

SensorData::SensorData() {
    data_map[0] = std::make_pair(MEASURE_TEMP, -1.0);
    data_map[1] = std::make_pair(MEASURE_PH, -1.0);
    data_map[2] = std::make_pair(MEASURE_DO, -1.0);
    data_map[3] = std::make_pair(MEASURE_CONDUCT, -1.0);
    data_map[4] = std::make_pair(MEASURE_SALI, -1.0);
    data_map[5] = std::make_pair(MEASURE_TDS, -1.0);
    // data_map[6] = std::make_pair(MEASURE_RESIS, -1.0);
}

SensorData::~SensorData() {}

void SensorData::set_data(const std::string &data_name, const float &value) {
    for (auto &data : data_map) {
        if (data.first == data_name) {
            data.second = value;
            break;
        }
    }
}

float SensorData::get_data(const std::string &data_name) {
    for (auto &data : data_map) {
        if (data.first == data_name) {
            return data.second;
        }
    }
    return -1.0;
}

String SensorData::format_data() {
    String res = "";
    for (uint8_t idx = 0; idx < SENSOR_DATA_COUNT; ++idx) {
        res += String(data_map[idx].first.c_str()) + ":" + String(data_map[idx].second);
        if (idx < SENSOR_DATA_COUNT - 1) res += "#";
        // Serial.printf("res: %s\n", res.c_str());
    }
    return res;
}
