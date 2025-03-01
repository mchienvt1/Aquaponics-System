#include "Sensor_Task.h"

HardwareSerial RS485Serial(1);
SensorData data;

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
        ESP_LOGE("SENSOR", "No data received from sensor");
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
    // if (value != -1.0) {
    //     sensor.publish(command_name, value);
    // }
    return value;
}

static void load_sensor_data() {
    float temp_ISHC = -1.0, temp_ISDC = -1.0, temp_ISEC = -1.0;

#ifdef ISHC
    temp_ISHC = process_value(sensorISHC, MEASURE_TEMP, 9);
    // float pH = process_value(sensorISHC, MEASURE_PH, 9);
    data.set_data(MEASURE_PH, process_value(sensorISHC, MEASURE_PH, 9));
#endif

#ifdef ISDC
    temp_ISDC = process_value(sensorISDC, MEASURE_TEMP, 9);
    // float oxygen = process_value(sensorISDC, MEASURE_DO, 9);
    data.set_data(MEASURE_DO, process_value(sensorISDC, MEASURE_DO, 9));
#endif

#ifdef ISEC
    temp_ISEC = process_value(sensorISEC, MEASURE_TEMP, 9);
    // float conduct = process_value(sensorISEC, MEASURE_CONDUCT, 9);
    data.set_data(MEASURE_CONDUCT, process_value(sensorISEC, MEASURE_CONDUCT, 9));
    // float sali = process_value(sensorISEC, MEASURE_SALI, 9);
    data.set_data(MEASURE_SALI, process_value(sensorISEC, MEASURE_SALI, 9));
    // float tds = process_value(sensorISEC, MEASURE_TDS, 9);
    data.set_data(MEASURE_TDS, process_value(sensorISEC, MEASURE_TDS, 9));
    // float resis = process_value(sensorISEC, MEASURE_RESIS, 9);
    data.set_data(MEASURE_RESIS, process_value(sensorISEC, MEASURE_RESIS, 9));
#endif

    float temp = (temp_ISHC + temp_ISDC + temp_ISEC) / count_sensor();
    data.set_data(MEASURE_TEMP, temp);
}

static void send_sensor_data() {
    String message = data.format_data();
    // ESP_LOGI("SENSOR", "Publish Sensor Data %s", message.c_str());
    update_sensor_data(message);
}

void sensor_task(void *pvParameters) {
    while (1) {
        load_sensor_data();
        send_sensor_data();
        vTaskDelay(RS485_PROCESS_TIMER / portTICK_PERIOD_MS);
    }
}

void sensor_task_init() {
    RS485Serial.begin(RS485_BAUDRATE, SERIAL_8N1, RXD_RS485, TXD_RS485);
    xTaskCreate(sensor_task, "Sensor_Task", 4096, NULL, 1, NULL);
}