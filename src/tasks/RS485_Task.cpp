#include "RS485_Task.h"
HardwareSerial RS485Serial(1);

Sensor sensorISHC(
    ISHC_ID, 
    {
        {MEASURE_PH, {ISHC_ID, 0x03, 0x00, 0x01, 0x00, 0x02}},                       
        {MEASURE_TEMP, {ISHC_ID, 0x03, 0x00, 0x03, 0x00, 0x02}}
    }
);

Sensor sensorISDC(
    ISDC_ID, 
    {
        {MEASURE_DO, {ISDC_ID, 0x03, 0x00, 0x01, 0x00, 0x02}},
        {MEASURE_TEMP, {ISDC_ID, 0x03, 0x00, 0x03, 0x00, 0x02}}
    }
);

Sensor sensorISEC(
    ISEC_ID, 
    {
        {MEASURE_CONDUCT, {ISEC_ID, 0x03, 0x00, 0x00, 0x00, 0x02}},
        {MEASURE_RESIS, {ISEC_ID, 0x03, 0x00, 0x02, 0x00, 0x02}},
        {MEASURE_SALI, {ISEC_ID, 0x03, 0x00, 0x08, 0x00, 0x02}},
        {MEASURE_TDS, {ISEC_ID, 0x03, 0x00, 0x06, 0x00, 0x02}},
        {MEASURE_TEMP, {ISEC_ID, 0x03, 0x00, 0x04, 0x00, 0x02}}
    }
);

void CleanBuffer() {
    size_t bytes_to_read = RS485Serial.available();
    if (bytes_to_read > 0)
    {
        byte out[bytes_to_read];
        RS485Serial.readBytes(out, bytes_to_read);
    }
}

void WriteToSensor(Sensor &sensor, std::string command_name) {
    std::array<uint8_t, 8> command = sensor.GetCommand(command_name);
    uint8_t cmd[8];
    for (int i = 0; i < 8; ++i)
    {
        cmd[i] = command[i];
    }
    RS485Serial.write(cmd, sizeof(cmd));
}

void ReadFromSensor(Sensor &sensor, size_t buffer_size, std::string command_name, float *value) {
    uint8_t buffer[buffer_size];

    int avail = RS485Serial.available();
    if (avail > 0) {
        size_t bytes_read = RS485Serial.read(buffer, sizeof(buffer));
        *value = sensor.ProcessValue(buffer);
    }
    else {
        Serial.println("Lỗi cảm biến");
    }
    memset(buffer, 0, sizeof(buffer));
}

float ProcessValue(Sensor &sensor, std::string command_name, size_t buffer_size)
{
    float value = -1.0;
    CleanBuffer();
    WriteToSensor(sensor, command_name);
    vTaskDelay(50 / portTICK_PERIOD_MS);
    ReadFromSensor(sensor, buffer_size, command_name, &value);
    vTaskDelay(50 / portTICK_PERIOD_MS);
    return value;
}

void RS485Task(void *pvParameters) {
    while (1) {
#ifdef ISHC
        float pH_ISHC = ProcessValue(sensorISHC, MEASURE_PH, 9);
        float temp_ISHC = ProcessValue(sensorISHC, MEASURE_TEMP, 9);
#endif
#ifdef ISDC
        float dO_ISDC = ProcessValue(sensorISDC, MEASURE_DO, 9);
        float temp_ISDC = ProcessValue(sensorISDC, MEASURE_TEMP, 9);
#endif
#ifdef ISEC
        float conduct_ISEC = ProcessValue(sensorISEC, MEASURE_CONDUCT, 9);
        float resis_ISEC = ProcessValue(sensorISEC, MEASURE_RESIS, 9);
        float sali_ISEC = ProcessValue(sensorISEC, MEASURE_SALI, 9);
        float tds_ISEC = ProcessValue(sensorISEC, MEASURE_TDS, 9);
        float temp_ISEC = ProcessValue(sensorISEC, MEASURE_TEMP, 9);
#endif
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void RS485_INIT() {
    RS485Serial.begin(9600, SERIAL_8N1, RXD, TXD);
    xTaskCreate(RS485Task, "RS485_Task", 4096, NULL, 1, NULL);
}