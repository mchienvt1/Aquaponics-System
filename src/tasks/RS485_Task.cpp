#include "RS485_Task.h"
HardwareSerial RS485Serial(1);

uint8_t count_sensor() {
    uint8_t count = 0;
#ifdef ISHC
    count++;
#endif
#ifdef ISDC
    count++;
#endif
#ifdef ISEC
    count++;
#endif
    return count;
}

#ifdef ISHC
Sensor_ISHC sensorISHC(
    ISHC_ID, 
    {
        {MEASURE_PH, {ISHC_ID, 0x03, 0x00, 0x01, 0x00, 0x02}},                       
        {MEASURE_TEMP, {ISHC_ID, 0x03, 0x00, 0x03, 0x00, 0x02}}
    }
);
#endif

#ifdef ISDC
Sensor_ISDC sensorISDC(
    ISDC_ID, 
    {
        {MEASURE_DO, {ISDC_ID, 0x03, 0x00, 0x01, 0x00, 0x02}},
        {MEASURE_TEMP, {ISDC_ID, 0x03, 0x00, 0x03, 0x00, 0x02}}
    }
);
#endif

#ifdef ISEC
Sensor_ISEC sensorISEC(
    ISEC_ID, 
    {
        {MEASURE_CONDUCT, {ISEC_ID, 0x03, 0x00, 0x00, 0x00, 0x02}},
        {MEASURE_RESIS, {ISEC_ID, 0x03, 0x00, 0x02, 0x00, 0x02}},
        {MEASURE_SALI, {ISEC_ID, 0x03, 0x00, 0x08, 0x00, 0x02}},
        {MEASURE_TDS, {ISEC_ID, 0x03, 0x00, 0x06, 0x00, 0x02}},
        {MEASURE_TEMP, {ISEC_ID, 0x03, 0x00, 0x04, 0x00, 0x02}}
    }
);
#endif

void clean_rs485_buffer() {
    size_t bytes_to_read = RS485Serial.available();
    if (bytes_to_read > 0)
    {
        byte out[bytes_to_read];
        RS485Serial.readBytes(out, bytes_to_read);
    }
}

void write_to_sensor(Sensor &sensor, std::string command_name) {
    std::array<uint8_t, 8> command = sensor.get_command(command_name);
    // Serial.print("Write ");
    uint8_t cmd[8];
    for (int i = 0; i < 8; ++i)
    {
        cmd[i] = command[i];
        // Serial.print(command[i], HEX);
        // Serial.print(" ");
    }
    // Serial.println("to sensor");
    RS485Serial.write(cmd, sizeof(cmd));
}

void read_from_sensor(Sensor &sensor, size_t buffer_size, std::string command_name, float *value) {
    uint8_t buffer[buffer_size];

    int avail = RS485Serial.available();
    if (avail > 0) {
        RS485Serial.read(buffer, sizeof(buffer));
        *value = sensor.process_value(buffer);
    }
    else {
        ESP_LOGE("RS485", "No data received from sensor");
    }
    memset(buffer, 0, sizeof(buffer));
}

float process_value(Sensor &sensor, std::string command_name, size_t buffer_size)
{
    float value = -1.0;
    clean_rs485_buffer();
    
    write_to_sensor(sensor, command_name);
    vTaskDelay(RS485_READ_WRITE_TIMER / portTICK_PERIOD_MS);

    read_from_sensor(sensor, buffer_size, command_name, &value);
    vTaskDelay(RS485_READ_WRITE_TIMER / portTICK_PERIOD_MS);

    // Publish value to MQTT
    if (value != -1.0) {
        sensor.publish(command_name, value);
    }
    return value;
}

void rs485_task(void *pvParameters) {
    while (1) {
        float temp_ISHC = 0.0, temp_ISDC = 0.0, temp_ISEC = 0.0;
#ifdef ISHC
        // float pH = process_value(sensorISHC, MEASURE_PH, 9);
        temp_ISHC = process_value(sensorISHC, MEASURE_TEMP, 9);
#endif
#ifdef ISDC
        float oxygen = process_value(sensorISDC, MEASURE_DO, 9);
        temp_ISDC = process_value(sensorISDC, MEASURE_TEMP, 9);
#endif
#ifdef ISEC
        float conduct = process_value(sensorISEC, MEASURE_CONDUCT, 9);
        float resis = process_value(sensorISEC, MEASURE_RESIS, 9);
        float sali = process_value(sensorISEC, MEASURE_SALI, 9);
        float tds = process_value(sensorISEC, MEASURE_TDS, 9);
        temp_ISEC = process_value(sensorISEC, MEASURE_TEMP, 9);
#endif
        float temp = (temp_ISHC + temp_ISDC + temp_ISEC) / count_sensor();
        vTaskDelay(RS485_PROCESS_TIMER / portTICK_PERIOD_MS);
    }
}

void RS485_task_init() {
    RS485Serial.begin(RS485_BAUDRATE, SERIAL_8N1, RXD_RS485, TXD_RS485);
    xTaskCreate(rs485_task, "RS485_Task", 4096, NULL, 1, NULL);
}