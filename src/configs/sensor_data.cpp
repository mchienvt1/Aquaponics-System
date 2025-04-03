#include "sensor_data.h"

SensorData::SensorData() {}

SensorData::~SensorData() {}

float calculate_average(const std::vector<float> & values){
    float sum = 0;
    for(float v: values) sum += v;
    return values.empty() ? 0 : sum / values.size();
}

std::string SensorData::process_sensor_data(){
    // process all sensor data
    std::map<std::string, std::vector<float>>::iterator it;
    for(std::map<std::string, std::vector<float>>::iterator i = sensor_data_mp.begin();i!=sensor_data_mp.end();i++){
        this->processed_data[i->first] = calculate_average(i->second);
    }
    nlohmann::json j = this->processed_data; 
    return j.dump();
}

void SensorData::set_data(const std::string &sensor_name, const float &value){
    this->sensor_data_mp[sensor_name].push_back(value);
}

std::vector<float> SensorData::get_data(const std::string &sensor_name){
    return this->sensor_data_mp[sensor_name];
}
void SensorData::clear_all_sensor_data(){
    std::map<std::string, std::vector<float>>::iterator it;
    for(std::map<std::string, std::vector<float>>::iterator i = sensor_data_mp.begin();i!=sensor_data_mp.end();i++){
        this->processed_data[i->first] = -1.0;
        this->sensor_data_mp[i->first].clear();
    }
}
