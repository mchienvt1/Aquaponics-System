#include "DHT_Task.h"

DHT20 dht20(&Wire1);

void dht_task(void *pvParameters)
{
    while (true)
    {
        float dht_temp = 24.0 + ((float)(esp_random() % 1000) / 1000.0); // 24.000 - 24.999 ~ 25.0
        float dht_humi = 50.0 + ((float)(esp_random() % 2000) / 1000.0); // 50.000 - 51.999 ~ 52.0

        // float dht_temp = 2 * (24.0 + ((float)(esp_random() % 1000) / 1000.0));
        // float dht_humi = 2 * (50.0 + ((float)(esp_random() % 2000) / 1000.0));

        // bool startWireStatus = Wire1.begin(DHT_SDA, DHT_SCL);

        // while (true)
        // {
        //     if (startWireStatus)
        //     {
        //         int status = dht20.read();

        //         switch (status)
        //         {
        //         case DHT20_OK:
        //         {
        //             float dht_temp = dht20.getTemperature();
        //             float dht_humi = dht20.getHumidity();
        //             String env_data = "{\"env_temp\":" + String(20.0) +
        //                               ",\"env_humi\":" + String(19.0) + "}";
        //             sendTelemetry(env_data);
        //             ESP_LOGI("DHT", "TEMP: %.2f HUMI: %.2f", dht_temp, dht_humi);
        //         }
        //         break;
        //         case DHT20_ERROR_CHECKSUM:
        //             ESP_LOGE("DHT", "Checksum error");
        //             break;
        //         case DHT20_ERROR_CONNECT:
        //             ESP_LOGE("DHT", "Connect error");
        //             break;
        //         case DHT20_MISSING_BYTES:
        //             ESP_LOGE("DHT", "Missing bytes");
        //             break;
        //         case DHT20_ERROR_BYTES_ALL_ZERO:
        //             ESP_LOGE("DHT", "All bytes read zero");
        //             break;
        //         case DHT20_ERROR_READ_TIMEOUT:
        //             ESP_LOGE("DHT", "Read time out");
        //             break;
        //         case DHT20_ERROR_LASTREAD:
        //             ESP_LOGE("DHT", "Error read too fast");
        //             break;
        //         default:
        //             ESP_LOGE("DHT", "Unknown error");
        //             break;
        //         }
        //     }
        //     else
        //     {
        //         ESP_LOGE("DHT", "Fail to connect to DHT");
        //     }
        //     vTaskDelay(SENSOR_DHT_TIMER / portTICK_PERIOD_MS);
        // }

        // float dht_temp = dht20.getTemperature();
        // float dht_humi = dht20.getHumidity();
        String env_data = "{\"env_temp\":" + String(dht_temp, 2) +
                          ",\"env_humi\":" + String(dht_humi, 2) + "}";
        sendTelemetry(env_data);
        // ESP_LOGI("DHT", "TEMP: %.2f HUMI: %.2f", dht_temp, dht_humi);
        vTaskDelay(SENSOR_DHT_TIMER / portTICK_PERIOD_MS);
    }
}

void dht_task_init()
{
    xTaskCreate(dht_task, "DHT_Task", 4096, 0, 1, 0);
}