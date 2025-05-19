#include "sensor_data.h"
#include <cmath>
#include <sstream>
#include <iomanip>

SensorData::SensorData() {}

SensorData::~SensorData() {}

std::string format_float(float value)
{
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << value;
    return out.str();
}

float calculate_average(const std::vector<float> &values)
{
    float sum = 0;
    for (float v : values)
        sum += v;

    if (values.empty())
        return 0;

    float average = sum / values.size();
    return std::round(average * 100.0f) / 100.0f;
}

std::string SensorData::process_sensor_data()
{
    for (std::map<std::string, std::vector<float>>::iterator i = sensor_data_mp.begin(); i != sensor_data_mp.end(); i++)
    {
        float avg = calculate_average(i->second);
        this->processed_data[i->first] = format_float(avg); // Chuyển float sang chuỗi đã làm tròn
    }

    nlohmann::json j = this->processed_data;
    return j.dump();
}

void SensorData::set_data(const std::string &sensor_name, const float &value)
{
    this->sensor_data_mp[sensor_name].push_back(value);
}

std::vector<float> SensorData::get_data(const std::string &sensor_name)
{
    return this->sensor_data_mp[sensor_name];
}
float SensorData::get_latest_data(const std::string &sensor_name)
{
    const std::vector<float> &data = this->sensor_data_mp[sensor_name];
    if (!data.empty())
    {
        return data.back(); // Trả về phần tử cuối cùng (mới nhất)
    }
    return -1.0f; // Giá trị mặc định nếu không có dữ liệu
}
void SensorData::clear_all_sensor_data()
{
    std::map<std::string, std::vector<float>>::iterator it;
    for (std::map<std::string, std::vector<float>>::iterator i = sensor_data_mp.begin(); i != sensor_data_mp.end(); i++)
    {
        this->processed_data[i->first] = -1.0;
        this->sensor_data_mp[i->first].clear();
    }
}
