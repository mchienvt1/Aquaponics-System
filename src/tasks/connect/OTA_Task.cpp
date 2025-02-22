#include "OTA_Task.h"

void update_starting_callback() {
    ESP_LOGI("OTA", "Starting update...");
}

void finished_callback(const bool &success) {
    if (success) {
        ESP_LOGI("OTA", "Done, Reboot now");
        esp_restart();
        return;
    }
    ESP_LOGE("OTA", "Downloading firmware failed");
}

void progress_callback(const size_t &current, const size_t &total) {
    ESP_LOGI("OTA", "Progress %.2f%%\n", static_cast<float>(current * 100U) / total);
}
