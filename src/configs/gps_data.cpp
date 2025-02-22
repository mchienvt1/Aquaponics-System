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

String GPSData::format_data() {
    String res = "{";
    for (auto &data : gps_data_map) {
        res += "\"" + String(data.first.c_str()) + "\":" + String(data.second) + ",";
    }
    res[res.length() - 1] = '}';
    return res;
}