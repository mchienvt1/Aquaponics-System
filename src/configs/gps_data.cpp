#include "gps_data.h"

GPSData::GPSData() {
    gps_data_map[0] = std::make_pair(GPS_LATITUDE, default_value);
    gps_data_map[1] = std::make_pair(GPS_LONGITUDE, default_value);
}

GPSData::~GPSData() {}

void GPSData::set_data(const std::string &data_name, const float &value) {
    for (auto &data : gps_data_map) {
        if (data.first == data_name) {
            data.second = value;
            break;
        }
    }
}

float GPSData::get_data(const std::string &data_name) {
    for (auto &data : gps_data_map) {
        if (data.first == data_name) {
            return data.second;
        }
    }
    return default_value;
}

const StaticJsonDocument<MAX_MESSAGE_SEND_SIZE> GPSData::format_data() {
    StaticJsonDocument<MAX_MESSAGE_SEND_SIZE> doc;
    for (auto &data : gps_data_map) {
        doc[data.first] = data.second;
    }
    return doc;
}