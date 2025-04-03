#include "GPS_Task.h"

HardwareSerial GPSSerial(2);
TinyGPSPlus gps;
GPSData gps_data;

bool satellite_status = false;

void displayInfo()
{
    if (!gps.location.isUpdated()) {
        ESP_LOGI("GPS", "Finding Satellites...");
        satellite_status = false;
        return;
    }

    satellite_status = true;

    if (gps.location.isValid()) {
        gps_data.set_data(GPS_LATITUDE, gps.location.lat());
        gps_data.set_data(GPS_LONGITUDE, gps.location.lng());
        ESP_LOGI("GPS", "Location: (%03.6f, %03.6f) updated %dms ago", gps_data.get_data(GPS_LATITUDE), gps_data.get_data(GPS_LONGITUDE), gps.location.age());
    }
    else {
        ESP_LOGW("GPS", "Unknown Location");
    }
}

void GPS_task(void *pvParameters) {
    while (true) {
        if (GPSSerial.available() > 0) {
            gps.encode(GPSSerial.read());
            displayInfo();
            if (WiFi.status() == WL_CONNECTED && satellite_status) {
                // update_gps_data(gps_data.format_data());
                sendTelemetry(gps_data.format_data());
            }
        }
        else{
            ESP_LOGE("GPS", "Fail to connect to GPS");
        }
        vTaskDelay(GPS_TIMER / portTICK_PERIOD_MS);
    }

}

void gps_task_init() {
    GPSSerial.begin(GPS_BAUDRATE, SERIAL_8N1, RXD_GPS, TXD_GPS);
    xTaskCreate(GPS_task, "GPS_Task", 4096, NULL, 1, NULL);
};