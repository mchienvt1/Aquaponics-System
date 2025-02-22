#include "OTA_Task.h"

Espressif_Updater<> updater;

bool currentFWSent = false;
bool updateRequestSent = false;

void update_starting_callback() {
    ESP_LOGI("OTA", "Starting update...");
}

void finished_callback(const bool & success) {
    if (success) {
        ESP_LOGI("OTA", "Done, Reboot now");
        esp_restart();
        return;
    }
    ESP_LOGE("OTA", "Downloading firmware failed");
}

void progress_callback(const size_t & current, const size_t & total) {
    ESP_LOGI("OTA", "Progress %.2f%%\n", static_cast<float>(current * 100U) / total);
}

void ota_task(void *pvParameters) {
    while (WiFi.status() != WL_CONNECTED) {
        delay(WIFI_TIMER);
    }
    while (!tb_connected) {
        delay(THINGSBOARD_CONNECT_TIMER);
    }
    if (!currentFWSent) {
        ESP_LOGI("OTA", "Current FW: %s %s\n", CURRENT_FIRMWARE_TITLE, CURRENT_FIRMWARE_VERSION);
        currentFWSent = ota.Firmware_Send_Info(CURRENT_FIRMWARE_TITLE, CURRENT_FIRMWARE_VERSION);
    }

    if (!updateRequestSent) {
        const OTA_Update_Callback ota_callback(CURRENT_FIRMWARE_TITLE, CURRENT_FIRMWARE_VERSION, &updater, &finished_callback, &progress_callback, &update_starting_callback, FIRMWARE_FAILURE_RETRIES, FIRMWARE_PACKET_SIZE);
        // See https://thingsboard.io/docs/user-guide/ota-updates/
        // to understand how to create a new OTA pacakge and assign it to a device so it can download it.
        // Sending the request again after a successfull update will automatically send the UPDATED firmware state,
        // because the assigned firmware title and version on the cloud and the firmware version and title we booted into are the same.
        updateRequestSent = ota.Subscribe_Firmware_Update(ota_callback);
        ESP_LOGI("OTA", "Subscribed to firmware update");
    }
    // tbClient.loop();
    vTaskDelete(NULL);
}

void ota_task_init() {
    xTaskCreate(ota_task, "OTA_Task", 4096, NULL, 2, NULL);
}
