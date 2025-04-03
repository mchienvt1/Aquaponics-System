#ifndef SENSOR_H_
#define SENSOR_H_

#include "global.h"

class Sensor {
protected:
    std::map<std::string, std::array<uint8_t, 8>> command_map;
    uint8_t id;
    std::string name;
public:
    Sensor() {};
    Sensor(std::string name, uint8_t id, const std::map<std::string, std::array<uint8_t, 6>> &raw_command_map);
    virtual ~Sensor() {};
    
    virtual void add_command(const std::string &command_name, std::array<uint8_t, 6> &raw_command_map);
    virtual std::array<uint8_t, 8> get_command(const std::string &command_name);
    virtual float process_value(const uint8_t *buffer);

    virtual void print_command();
    virtual std::string get_sensor_name();
};

#ifdef ISHC
class Sensor_ISHC : public Sensor {

public:
    Sensor_ISHC() {};
    Sensor_ISHC(std::string name, uint8_t id, const std::map<std::string, std::array<uint8_t, 6>> &raw_command_map) : Sensor(name, id, raw_command_map) {};
    ~Sensor_ISHC() {};
};
#endif

#ifdef ISEC
class Sensor_ISEC : public Sensor {

public:
    Sensor_ISEC() {};
    Sensor_ISEC(std::string name, uint8_t id, const std::map<std::string, std::array<uint8_t, 6>> &raw_command_map) : Sensor(name, id, raw_command_map) {};
    ~Sensor_ISEC() {};
};
#endif

#ifdef ISDC
class Sensor_ISDC : public Sensor {

public:
    Sensor_ISDC() {};
    Sensor_ISDC(std::string name, uint8_t id, const std::map<std::string, std::array<uint8_t, 6>> &raw_command_map) : Sensor(name, id, raw_command_map) {};
    ~Sensor_ISDC() {};
};
#endif

#endif
