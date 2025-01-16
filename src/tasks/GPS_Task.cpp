// #include "GPS_Task.h"

// TinyGPSPlus gps;
// HardwareSerial AT6668(0);
// SoftwareSerial ss(RXD_GPS, TXD_GPS);

// struct GPSData {
//     float lat;
//     float lng;
//     uint8_t hour;
//     uint8_t minute;
//     uint8_t second;
//     uint8_t day;
//     uint8_t month;
//     uint16_t year;
// };

// GPSData gpsData;

// // void displayInfo()
// // {
// //     Serial.print(F("Location: "));
// //     Serial.printf("satellites:%d\n", gps.satellites.value());
// //     if (gps.location.isUpdated()) {
// //         Serial.print(gps.location.lat(), 6);
// //         Serial.print(F(","));
// //         Serial.print(gps.location.lng(), 6);
// //         Serial.print(F("\n"));

// //     } else {
// //         Serial.print(F("INVALID\n"));
// //     }

// //     Serial.print(F("  Date/Time: "));
// //     if (gps.date.isUpdated()) {
// //         Serial.print(gps.date.month());
// //         Serial.print(F("/"));
// //         Serial.print(gps.date.day());
// //         Serial.print(F("/"));
// //         Serial.print(gps.date.year());
// //     } else {
// //         Serial.print(F("INVALID"));
// //     }

// //     Serial.print(F(" "));
// //     if (gps.time.isUpdated()) {
// //         if (gps.time.hour() < 10) Serial.print(F("0"));
// //         Serial.print(gps.time.hour());
// //         Serial.print(F(":"));
// //         if (gps.time.minute() < 10) Serial.print(F("0"));
// //         Serial.print(gps.time.minute());
// //         Serial.print(F(":"));
// //         if (gps.time.second() < 10) Serial.print(F("0"));
// //         Serial.print(gps.time.second());
// //         Serial.print(F("."));
// //         if (gps.time.centisecond() < 10) Serial.print(F("0"));
// //         Serial.print(gps.time.centisecond());
// //     } else {
// //         Serial.print(F("INVALID"));
// //     }

// //     Serial.println();
// //     vTaskDelay(1000 / portTICK_PERIOD_MS);
// // }

// void get_data() {
//     if (gps.location.isUpdated()) {
//         gpsData.lat = gps.location.lat();
//         gpsData.lng = gps.location.lng();
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

// void publish_data_task(void *pvParameters) {
//     while (true) {
//         if (WiFi.status() == WL_CONNECTED) {
//             String location = String(gpsData.lat, 6) + "," + String(gpsData.lng, 6);
//             publish_data("GPS", "Location", location);
//             vTaskDelay(INSANELY_LONG_TIMER / portTICK_PERIOD_MS);
//         }
//         vTaskDelay(GPS_TIMER / portTICK_PERIOD_MS);
//     }
// }

// void GPS_task(void *pvParameters) {
//     while (true) {
//         // AT6668.flush();
//         // int size = 0;
//         // if ((size = AT6668.available()) > 0) {
//         //     ESP_LOGI("GPS", "Buffer size to read %d", size);
//         //     uint8_t out[size];
//         //     AT6668.read(out, size);
//         //     for(uint8_t i = 0; i < size; ++i) {
//         //         Serial.printf("%d ", out[i]);
//         //         // gps.encode(c);
//         //     }
//         //     Serial.println("");
//         //     memset(out, 0, sizeof(out));
//         // };
//         // ESP_LOGI("GPS", "SoftwareSerial available %d", ss.available());

//         // get_data();
//         // ESP_LOGD("GPS", "Data: (%f,%f), (%d:%d:%d), (%d/%d), %d", gpsData.lat, gpsData.lng, gpsData.hour, gpsData.minute, gpsData.second, gpsData.day, gpsData.month, gpsData.year);
//         while (Serial1.available() > 0){
//             Serial.write(Serial1.read());
//             // gps.encode(Serial1.read());
//             // if (gps.location.isUpdated()) {
//             //     // Latitude in degrees (double)
//             //     Serial.print("Latitude= ");
//             //     Serial.print(gps.location.lat(), 6);
//             //     // Longitude in degrees (double)
//             //     Serial.print(" Longitude= ");
//             //     Serial.println(gps.location.lng(), 6);
//             // }
//         }
//         vTaskDelay(GPS_TIMER / portTICK_PERIOD_MS);
//     }

// }


// void GPS_task_init() {
//     // AT6668.begin(GPS_BAUDRATE, SERIAL_8N1, RXD_GPS, TXD_GPS);
//     Serial1.begin(GPS_BAUDRATE, SERIAL_8N1, RXD_GPS, TXD_GPS);
//     // ESP_LOGI("GPS", "TinyGPSPlus library ver %s", String(gps.libraryVersion()).c_str());
//     xTaskCreate(GPS_task, "GPS_Task", 4096, NULL, 1, NULL);
//     xTaskCreate(publish_data_task, "Publish_GPS_Task", 4096, NULL, 2, NULL);
// };