#ifndef GPS_DATA_H_
#define GPS_DATA_H_

#include "global.h"

class GPSData 
{
protected:
    std::pair<std::string, double> gps_data_map[GPS_DATA_COUNT];
    double default_value = 0.0;

public:
    GPSData();
    ~GPSData();
    
    void set_data(const std::string &data_name, const float &value);
    float get_data(const std::string &data_name);
    String format_data();
};

#endif