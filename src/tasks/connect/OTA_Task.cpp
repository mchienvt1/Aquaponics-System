#include "OTA_Task.h"

Espressif_Updater<> updater;

void update_starting_callback() {
    ESP_LOGI("OTA", "Starting update...");
    set_rgb_color(WHITE_RGB);
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
    ESP_LOGI("OTA", "Progress %.2f%%", static_cast<float>(current * 100U) / total);
}

const OTA_Update_Callback callback(
    CURRENT_FIRMWARE_TITLE, 
    CURRENT_FIRMWARE_VERSION,
    &updater, 
    &finished_callback, 
    &progress_callback, 
    &update_starting_callback, 
    FIRMWARE_FAILURE_RETRIES, 
    FIRMWARE_PACKET_SIZE
);

bool ota_setup() {
    // if (!currentFWSent) {
    ESP_LOGI("OTA", "Current FW: %s %s", CURRENT_FIRMWARE_TITLE, CURRENT_FIRMWARE_VERSION);
    ota.Firmware_Send_Info(CURRENT_FIRMWARE_TITLE, CURRENT_FIRMWARE_VERSION);
    // }

    // See https://thingsboard.io/docs/user-guide/ota-updates/
    // to understand how to create a new OTA pacakge and assign it to a device so it can download it.
    // Sending the request again after a successfull update will automatically send the UPDATED firmware state,
    // because the assigned firmware title and version on the cloud and the firmware version and title we booted into are the same.
    if (!ota.Subscribe_Firmware_Update(callback)) {
        ESP_LOGI("OTA", "Failed to subscribe to firmware update");
        return false;
    }
    ESP_LOGI("OTA", "Subscribed to firmware update");
    return true;
}
