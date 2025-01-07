#ifndef SENSOR_H_
#define SENSOR_H_

#include "global.h"
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
            std::array<uint8_t, 8> command = ProcessCRC(raw_command.second);
            command_map[raw_command.first] = command;
        }
    }
    virtual ~Sensor() {};
    
    virtual void AddCommand(const std::string &command_name, std::array<uint8_t, 6> &raw_command_map);
    virtual std::array<uint8_t, 8> GetCommand(const std::string &command_name);
    virtual float ProcessValue(const uint8_t *buffer);

    virtual void PrintCommand();
};

// class Sensor_ISHC : public Sensor {

// public:
//     Sensor_ISHC(uint8_t id, const std::map<std::string, std::array<uint8_t, 6>> &raw_command_map);
//     ~Sensor_ISHC() {};
// };


// class Sensor_ISEC : public Sensor {

// public:
//     Sensor_ISEC(uint8_t id, const std::map<std::string, std::array<uint8_t, 6>> &raw_command_map);
//     ~Sensor_ISEC() {};
// };

// class Sensor_ISDC : public Sensor {

// public:
//     Sensor_ISDC(uint8_t id, const std::map<std::string, std::array<uint8_t, 6>> &raw_command_map);
//     ~Sensor_ISDC() {};
// };

#endif
