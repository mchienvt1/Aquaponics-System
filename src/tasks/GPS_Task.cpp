#include "GPS_Task.h"

TinyGPSPlus gps;
HardwareSerial GPSSerial(2);
gps_location location;

bool satellite_status = false;

// void display_info_init() {
//     Serial.println(F("Sats HDOP  Latitude   Longitude   Fix  Date       Time     Date Alt    Course Speed Card  Chars Sentences Checksum"));
//     Serial.println(F("           (deg)      (deg)       Age                      Age  (m)    --- from GPS ----  RX    RX        Fail"));
//     Serial.println(F("----------------------------------------------------------------------------------------------------------------------------------------"));
// }

void displayInfo()
{
    if (!gps.location.isUpdated()) {
        ESP_LOGI("GPS", "Finding Satellites...");
        satellite_status = false;
        return;
    }

    satellite_status = true;

    if (gps.satellites.isValid() && gps.hdop.isValid()) {
        Serial.printf("Found %02d satellites with %03.1f hdop\n", gps.satellites.value(), gps.hdop.hdop());
    }

    if (gps.location.isValid()) {
        location.lat = gps.location.lat();
        location.lng = gps.location.lng();
        Serial.printf("Location: (%03.6f, %03.6f) updated %dms ago\n", location.lat, location.lng, gps.location.age());
    }
    else {
        Serial.println("Unknown Location");
    }

    if (gps.date.isValid() && gps.time.isValid()) {
        Serial.print("Date and time: ");
        char sz1[32];
        sprintf(sz1, "%02d/%02d/%02d ", gps.date.month(), gps.date.day(), gps.date.year());
        Serial.print(sz1);
        char sz2[32];
        sprintf(sz2, "%02d:%02d:%02d", gps.time.hour(), gps.time.minute(), gps.time.second());
        Serial.println(sz2);
    }
    else {
        Serial.println("Unknown Date/Time");
    }
    // printInt(gps.charsProcessed(), true, 6);
    // printInt(gps.sentencesWithFix(), true, 10);
    // printInt(gps.failedChecksum(), true, 9);
    Serial.println();
}

void publish_data_task(void *pvParameters) {
    while (true) {
        if (WiFi.status() == WL_CONNECTED && satellite_status) {
            String location_data = String(location.lat, 6) + "," + String(location.lng, 6);
            // publish_data(GPS_LOCATION_TOPIC, location_data);
            delay(INSANELY_LONG_TIMER);
        }
        delay(GPS_TIMER);
    }
}

void GPS_task(void *pvParameters) {
    // display_info_init();
    while (true) {
        // Read data from AT6668
        while (GPSSerial.available() > 0) {
            gps.encode(GPSSerial.read());
        }
        displayInfo();
        delay(GPS_TIMER);
    }

}

void GPS_task_init() {
    GPSSerial.begin(GPS_BAUDRATE, SERIAL_8N1, RXD_GPS, TXD_GPS);
    xTaskCreate(GPS_task, "GPS_Task", 4096, NULL, 1, NULL);
    xTaskCreate(publish_data_task, "Publish_GPS_Task", 4096, NULL, 2, NULL);
};