#ifndef SENSOR_H_
#define SENSOR_H_

#include "global.h"
#include "../tasks/MQTT_Task.h"
#include "../utils/ConvertToFloat.h"
#include "../utils/CRC16.h"

class Sensor {

protected:
    std::map<std::string, std::array<uint8_t, 8>> command_map;
    uint8_t id;

public:
    Sensor() {};
    Sensor(uint8_t id, const std::map<std::string, std::array<uint8_t, 6>> &raw_command_map) {
        this->id = id;
        for (auto &raw_command : raw_command_map) {
            std::array<uint8_t, 8> command = process_CRC(raw_command.second);
            command_map[raw_command.first] = command;
        }
    }
    virtual ~Sensor() {};
    
    virtual void add_command(const std::string &command_name, std::array<uint8_t, 6> &raw_command_map);
    virtual std::array<uint8_t, 8> get_command(const std::string &command_name);
    virtual float process_value(const uint8_t *buffer);
    virtual void publish(const std::string &command_name, const float &value) {};

    virtual void print_command();
};

class Sensor_ISHC : public Sensor {

public:
    Sensor_ISHC() {};
    Sensor_ISHC(uint8_t id, const std::map<std::string, std::array<uint8_t, 6>> &raw_command_map) : Sensor(id, raw_command_map) {};
    ~Sensor_ISHC() {};

    void publish(const std::string &command_name, const float &value) override;
};


class Sensor_ISEC : public Sensor {

public:
    Sensor_ISEC() {};
    Sensor_ISEC(uint8_t id, const std::map<std::string, std::array<uint8_t, 6>> &raw_command_map) : Sensor(id, raw_command_map) {};
    ~Sensor_ISEC() {};

    void publish(const std::string &command_name, const float &value) override;
};

class Sensor_ISDC : public Sensor {

public:
    Sensor_ISDC() {};
    Sensor_ISDC(uint8_t id, const std::map<std::string, std::array<uint8_t, 6>> &raw_command_map) : Sensor(id, raw_command_map) {};
    ~Sensor_ISDC() {};

    void publish(const std::string &command_name, const float &value) override;
};

#endif
