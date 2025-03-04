#include "DHT_Task.h"

DHT20 dht20(&Wire1);

void dht_task(void *pvParameters) {
    Wire1.begin(DHT_SDA, DHT_SCL);

    while (true) {
        Serial.print("DHT20, \t");
        int status = dht20.read();
        switch (status)
        {
        case DHT20_OK:
          Serial.print("OK,\t");
          break;
        case DHT20_ERROR_CHECKSUM:
          Serial.print("Checksum error,\t");
          break;
        case DHT20_ERROR_CONNECT:
          Serial.print("Connect error,\t");
          break;
        case DHT20_MISSING_BYTES:
          Serial.print("Missing bytes,\t");
          break;
        case DHT20_ERROR_BYTES_ALL_ZERO:
          Serial.print("All bytes read zero");
          break;
        case DHT20_ERROR_READ_TIMEOUT:
          Serial.print("Read time out");
          break;
        case DHT20_ERROR_LASTREAD:
          Serial.print("Error read too fast");
          break;
        default:
          Serial.print("Unknown error,\t");
          break;
        }
      
        //  DISPLAY DATA, sensor has only one decimal.
        Serial.print(dht20.getHumidity(), 1);
        Serial.print(",\t");
        Serial.println(dht20.getTemperature(), 1);
        delay(2000);
    }
}

void dht_task_init() {
    xTaskCreate(dht_task, "DHT_Task", 2048, 0, 1, 0);
}