#include "DHT_Task.h"

DHT20 dht20(&Wire1);

void dht_task(void *pvParameters) {
    Wire1.begin(DHT_SDA, DHT_SCL);

    while (true) {
        int status = dht20.read();
        float dht_temp = dht20.getTemperature();
        float dht_humi = dht20.getHumidity();
        switch (status)
        {
            case DHT20_OK:
            ESP_LOGI("DHT", "TEMP: %.2f HUMI: %.2f", dht_temp, dht_humi);
            break;
            case DHT20_ERROR_CHECKSUM:
            ESP_LOGE("DHT", "Checksum error");
            break;
            case DHT20_ERROR_CONNECT:
            ESP_LOGE("DHT", "Connect error");
            break;
            case DHT20_MISSING_BYTES:
            ESP_LOGE("DHT", "Missing bytes");
            break;
            case DHT20_ERROR_BYTES_ALL_ZERO:
            ESP_LOGE("DHT", "All bytes read zero");
            break;
            case DHT20_ERROR_READ_TIMEOUT:
            ESP_LOGE("DHT", "Read time out");
            break;
            case DHT20_ERROR_LASTREAD:
            ESP_LOGE("DHT", "Error read too fast");
            break;
            default:
            ESP_LOGE("DHT", "Unknown error");
            break;
        }
        delay(SENSOR_DHT_TIMER);
        String env_data = "{\"env_temp\":" + String(dht_temp) + 
                          ",\"env_humi\":" + String(dht_humi) + "}";
        update_env_data(env_data);
    }
}

void dht_task_init() {
    xTaskCreate(dht_task, "DHT_Task", 4096, 0, 1, 0);
}