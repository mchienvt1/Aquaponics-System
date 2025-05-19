#include "Sensor_Task.h"

bool need_processing = false;

HardwareSerial RS485Serial(1);
SensorData sensorData;

// Initial Sensors
#ifdef ISHC
Sensor_ISHC sensorISHC(
    "ISHC",
    ISHC_ID,
    {{MEASURE_PH, {ISHC_ID, 0x03, 0x00, 0x01, 0x00, 0x02}},
     {MEASURE_TEMP, {ISHC_ID, 0x03, 0x00, 0x03, 0x00, 0x02}}});
#endif

#ifdef ISDC
Sensor_ISDC sensorISDC(
    "ISDC",
    ISDC_ID,
    {{MEASURE_DO, {ISDC_ID, 0x03, 0x00, 0x01, 0x00, 0x02}},
     {MEASURE_TEMP, {ISDC_ID, 0x03, 0x00, 0x03, 0x00, 0x02}}});
#endif

#ifdef ISEC
Sensor_ISEC sensorISEC(
    "ISEC",
    ISEC_ID,
    {{MEASURE_CONDUCT, {ISEC_ID, 0x03, 0x00, 0x00, 0x00, 0x02}},
     {MEASURE_RESIS, {ISEC_ID, 0x03, 0x00, 0x02, 0x00, 0x02}},
     {MEASURE_SALI, {ISEC_ID, 0x03, 0x00, 0x08, 0x00, 0x02}},
     {MEASURE_TDS, {ISEC_ID, 0x03, 0x00, 0x06, 0x00, 0x02}},
     {MEASURE_TEMP, {ISEC_ID, 0x03, 0x00, 0x04, 0x00, 0x02}}});
#endif

void send_processed_data()
{
    // if (data_list.size() <= 0) return;
    // SensorData avg_data;
    // ESP_LOGI("SENSOR", "Collected %d data points\n", data_list.size());
    // for (uint8_t index = 0; index < SENSOR_DATA_COUNT; ++index) {
    //     float singular_data = 0.00f;
    //     uint16_t list_idx = 1;
    //     for (auto it = data_list.begin(); it != data_list.end(); ++it) {
    //         // TODO: Detect and ignore the anomaly data in list
    //         if ((*it).get_data(index) >= -1)
    //             singular_data = singular_data + ((*it).get_data(index) - singular_data) / (float)list_idx;
    //         ++list_idx;
    //     }
    //     avg_data.set_data(index, singular_data);
    // }
    // ESP_LOGI("SENSOR", "Publish Sensor Data %s", avg_data.format_processed_data().c_str());
    // data_list.clear();
    // update_process_data(avg_data.format_processed_data());
}

// Read and save sensor values to SensorData mapping
float read_sensor_value(Sensor &sensor, std::string target_command, size_t buffer_size = 9)
{
    float result = -1.0;
    // 1. Clean rs485 buffer
    size_t bytes_to_read = RS485Serial.available();
    if (bytes_to_read > 0) // check if there is any remain bytes
    {
        byte out[bytes_to_read];
        RS485Serial.readBytes(out, bytes_to_read);
    }
    vTaskDelay(SENSOR_CLEAR_TIMEOUT / portTICK_PERIOD_MS);

    // 2. Send command to sensor
    std::array<uint8_t, 8> command = sensor.get_command(target_command);
    uint8_t cmd[8];
    for (int i = 0; i < 8; ++i)
    {
        cmd[i] = command[i];
    }
    RS485Serial.write(cmd, sizeof(cmd));
    vTaskDelay(SENSOR_ISX_READ_WRITE_TIMER / portTICK_PERIOD_MS);

    // 3. Read data
    uint8_t buffer[buffer_size];
    if (RS485Serial.available() > 0)
    {
        RS485Serial.read(buffer, sizeof(buffer));
        result = sensor.process_value(buffer);
    }
    else
    {
        ESP_LOGE("SENSOR", "No data received from sensor %s", sensor.get_sensor_name().c_str());
    }
    memset(buffer, 0, sizeof(buffer));
    return result;
}
void temp_load_data()
{
    // Tạo số thực ngẫu nhiên trong khoảng [min, max)
    auto randomFloatInRange = [](float min, float max) -> float
    {
        return min + static_cast<float>(random(0, 10001)) / 10000.0f * (max - min);
    };

    sensorData.set_data(MEASURE_PH, randomFloatInRange(6.6f, 6.65f));
    sensorData.set_data(MEASURE_DO, randomFloatInRange(0.5f, 0.55f));
    sensorData.set_data(MEASURE_CONDUCT, randomFloatInRange(1000.0f, 3000.0f)); // ví dụ
    sensorData.set_data(MEASURE_SALI, randomFloatInRange(350.0f, 357.0f));
    sensorData.set_data(MEASURE_TDS, randomFloatInRange(515.0f, 517.0f));   // ví dụ
    sensorData.set_data(MEASURE_RESIS, randomFloatInRange(967.0f, 972.0f)); // ví dụ
    sensorData.set_data(MEASURE_TEMP, randomFloatInRange(29.2f, 29.3f));
    sensorData.set_data(MEASURE_TEMP, randomFloatInRange(29.2f, 29.3f));
    sensorData.set_data(MEASURE_TEMP, randomFloatInRange(29.2f, 29.3f));
}
// Read all available sensor data
static void load_sensor_data()
{
    float temp_ISHC = -1.0, temp_ISDC = -1.0, temp_ISEC = -1.0;
    float total_temp = -1.0;
    float avail_sensor = 0.0;
#ifdef ISHC
    temp_ISHC = read_sensor_value(sensorISHC, MEASURE_TEMP);
    if (temp_ISHC != -1.0)
    {
        total_temp += temp_ISHC;
        avail_sensor += 1.0;
    }
    sensorData.set_data(MEASURE_PH, read_sensor_value(sensorISHC, MEASURE_PH));
#endif

#ifdef ISDC
    temp_ISDC = read_sensor_value(sensorISDC, MEASURE_TEMP);
    if (temp_ISDC != -1.0)
    {
        total_temp += temp_ISDC;
        avail_sensor += 1.0;
    }
    sensorData.set_data(MEASURE_DO, read_sensor_value(sensorISDC, MEASURE_DO));
#endif

#ifdef ISEC
    temp_ISEC = read_sensor_value(sensorISEC, MEASURE_TEMP);
    if (temp_ISEC != -1.0)
    {
        total_temp += temp_ISEC;
        avail_sensor += 1.0;
    }
    sensorData.set_data(MEASURE_CONDUCT, read_sensor_value(sensorISEC, MEASURE_CONDUCT));
    sensorData.set_data(MEASURE_SALI, read_sensor_value(sensorISEC, MEASURE_SALI));
    sensorData.set_data(MEASURE_TDS, read_sensor_value(sensorISEC, MEASURE_TDS));
    sensorData.set_data(MEASURE_RESIS, read_sensor_value(sensorISEC, MEASURE_RESIS));
#endif
    if (avail_sensor > 0.0)
    {
        sensorData.set_data(MEASURE_TEMP, total_temp / avail_sensor);
    }
}

// 2 process send to 2 different type of data
void sensor_task(void *pvParameters)
{
    int count = 0;
    while (1)
    {
#ifdef DEVELOP
        temp_load_data();
#else
        load_sensor_data();
#endif

        // if (handleAutoencoder(sensorData.get_latest_data(MEASURE_TEMP), sensorData.get_latest_data(MEASURE_PH), sensorData.get_latest_data(MEASURE_DO)))
        // {
        //     // Anomaly detected
        //     sendTelemetry("{\"anomaly\": true}");
        //     handleAutoencoder2(sensorData.get_latest_data(MEASURE_TEMP), sensorData.get_latest_data(MEASURE_PH), sensorData.get_latest_data(MEASURE_DO));
        // }
        // else
        // {
        //     // No anomaly
        //     sendTelemetry("{\"anomaly\": false}");
        // }

        // if (handleAutoencoder(24.5, 6.19, 10.4))
        // {
        //     // Anomaly detected
        //     sendTelemetry("{\"anomaly\": true}");
        //     abnormalClassification(24.5, 6.19, 10.4);
        // }
        // else
        // {
        //     // No anomaly
        //     sendTelemetry("{\"anomaly\": false}");
        // }

        // ESP_LOGI("SENSOR", "%s", sensorData.process_sensor_data().c_str());
        if (count % 5 == 0)
        {
            std::string values = sensorData.process_sensor_data();
            sendTelemetry(String(values.c_str()));
            // TODO: send telemetry
            // String json = "{\"ph_predict_lower\": true}";
            // sendTelemetry(json);
        }
        else if (count % 300 == 0)
        {
            count = 0;
            std::string values = sensorData.process_sensor_data();
            sendTelemetry(String(values.c_str()));
            sensorData.clear_all_sensor_data();
        }
        count++;
        vTaskDelay(SENSOR_ISX_PROCESS_TIMER / portTICK_PERIOD_MS);
    }
}

void sensor_task_init()
{
    RS485Serial.begin(RS485_BAUDRATE, SERIAL_8N1, RXD_RS485, TXD_RS485);
    xTaskCreate(sensor_task, "Sensor_Task", 4096, NULL, 1, &sensor_task_handle);
}