#include "GPS_Task.h"

TinyGPSPlus gps;
HardwareSerial GPSSerial(2);

void displayInfo()
{
    Serial.print(F("Location: "));
    Serial.printf("satellites:%d\n", gps.satellites.value());
    if (gps.location.isUpdated()) {
        Serial.print(gps.location.lat(), 6);
        Serial.print(F(","));
        Serial.print(gps.location.lng(), 6);
        Serial.print(F("\n"));

    } else {
        Serial.print(F("INVALID\n"));
    }

    Serial.print(F("  Date/Time: "));
    if (gps.date.isUpdated()) {
        Serial.print(gps.date.month());
        Serial.print(F("/"));
        Serial.print(gps.date.day());
        Serial.print(F("/"));
        Serial.print(gps.date.year());
    } else {
        Serial.print(F("INVALID"));
    }

    Serial.print(F(" "));
    if (gps.time.isUpdated()) {
        if (gps.time.hour() < 10) Serial.print(F("0"));
        Serial.print(gps.time.hour());
        Serial.print(F(":"));
        if (gps.time.minute() < 10) Serial.print(F("0"));
        Serial.print(gps.time.minute());
        Serial.print(F(":"));
        if (gps.time.second() < 10) Serial.print(F("0"));
        Serial.print(gps.time.second());
        Serial.print(F("."));
        if (gps.time.centisecond() < 10) Serial.print(F("0"));
        Serial.print(gps.time.centisecond());
    } else {
        Serial.print(F("INVALID"));
    }

    Serial.println();
}

// void get_data() {
//     if (gps.location.isUpdated()) {
//         ESP_LOGI("GPS", "LAT = %.6f, LNG = %.6f", gps.location.lat(), gps.location.lng());
//     }
//     if (gps.time.isUpdated()) {
//         gpsData.hour = gps.time.hour();
//         gpsData.minute = gps.time.minute();
//         gpsData.second = gps.time.second();
//     }
//     if (gps.date.isUpdated()) {
//         gpsData.day = gps.date.day();
//         gpsData.month = gps.date.month();
//         gpsData.year = gps.date.year();
//     }
// }

void publish_data_task(void *pvParameters) {
    while (true) {
        if (WiFi.status() == WL_CONNECTED) {
            // String location = String(gpsData.lat, 6) + "," + String(gpsData.lng, 6);
            // publish_data("GPS", "Location", location);
            vTaskDelay(INSANELY_LONG_TIMER / portTICK_PERIOD_MS);
        }
        vTaskDelay(GPS_TIMER / portTICK_PERIOD_MS);
    }
}

void GPS_task(void *pvParameters) {
    while (true) {
        while (GPSSerial.available() > 0) {
            // char c = (char)GPSSerial.read();
            Serial.write(GPSSerial.read());
            // gps.encode(c);
        }
        Serial.println("");
        // ESP_LOGI("GPS", "SoftwareSerial available %d", ss.available());

        // displayInfo();
        vTaskDelay(GPS_TIMER / portTICK_PERIOD_MS);
        
        // while (GPSSerial.available() >= 1){
        //     Serial.write(GPSSerial.read());
        //     gps.encode(Serial1.read());
        //     if (gps.location.isUpdated()) {
        //         // Latitude in degrees (double)
        //         Serial.print("Latitude= ");
        //         Serial.print(gps.location.lat(), 6);
        //         // Longitude in degrees (double)
        //         Serial.print(" Longitude= ");
        //         Serial.println(gps.location.lng(), 6);
        //     }
        // }
    }

}


void GPS_task_init() {
    GPSSerial.begin(GPS_BAUDRATE, SERIAL_8N1, RXD_GPS, TXD_GPS);
    xTaskCreate(GPS_task, "GPS_Task", 4096, NULL, 1, NULL);
    xTaskCreate(publish_data_task, "Publish_GPS_Task", 4096, NULL, 2, NULL);
};