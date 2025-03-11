#include "NTP_Task.h"

constexpr const char TIMEZONE[] = "UTC-7";
constexpr const char NTP_SERVER[]= "pool.ntp.org";

void initialize_sntp() {
    // Set timezone (optional, adjust as needed)
    setenv("TZ", TIMEZONE, 1);
    tzset();

    // Initialize SNTP
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, NTP_SERVER); // Use a reliable NTP server
    sntp_init();
}

void print_current_time() {
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    ESP_LOGI("NTP", "Current time: %s", asctime(&timeinfo));
}

void ntp_task(void *pvParameters) {
    initialize_sntp();
    while (WiFi.status() != WL_CONNECTED) {
        delay(WIFI_TIMER);
    }
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET) {
        ESP_LOGI("NTP", "Waiting for system time to be set... ");
        delay(NTP_CONNECT_TIMER);
    }
    while (true) {
        print_current_time();
        delay(NTP_LOOP_TIMER);
    }
}

void ntp_task_init() {
    xTaskCreate(ntp_task, "NTP_Task", 4096, 0, 1, 0);
}