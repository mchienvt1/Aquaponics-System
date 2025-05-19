#ifndef SENSOR_DATA_H_
#define SENSOR_DATA_H_

#include "global.h"

class SensorData
{
protected:
    std::map<std::string, std::vector<float>> sensor_data_mp;
    std::map<std::string, std::string> processed_data;

public:
    SensorData();
    ~SensorData();
    void set_data(const std::string &sensor_name, const float &value);
    std::vector<float> get_data(const std::string &sensor_name);
    float get_latest_data(const std::string &sensor_name);
    std::string process_sensor_data();
    void clear_all_sensor_data();
};

#endif