#ifndef SENSOR_DATA_H_
#define SENSOR_DATA_H_

#include "global.h"

class SensorData 
{
protected:
    std::pair<std::string, float> sensor_data_map[SENSOR_DATA_COUNT];
    float default_value = -1.0;

public:
    SensorData();
    ~SensorData();
    
    void set_data(const std::string &data_name, const float &value);
    void set_data(uint8_t index, const float &value);
    
    float get_data(const std::string &data_name);
    float get_data(uint8_t index);
    String format_raw_data();
    String format_processed_data();
};

#endif