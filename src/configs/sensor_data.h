#ifndef SENSOR_DATA_H_
#define SENSOR_DATA_H_

#include "global.h"

class SensorData {
private:
    std::pair<std::string, float> data_map[SENSOR_DATA_COUNT];
public:
    SensorData();
    ~SensorData();
    
    void set_data(const std::string &data_name, const float &value);
    float get_data(const std::string &data_name);
    String format_data();
};

#endif