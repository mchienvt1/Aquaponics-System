#include "OTA_Task.h"

Espressif_Updater<> updater;

bool currentFWSent = false;
bool updateRequestSent = false;

/// @brief Update starting callback method that will be called as soon as the shared attribute firmware keys have been received and processed
/// and the moment before we subscribe the necessary topics for the OTA firmware update.
/// Is meant to give a moment were any additional processes or communication with the cloud can be stopped to ensure the update process runs as smooth as possible.
/// To ensure that calling the ThingsBoardSized::Cleanup_Subscriptions() method can be used which stops any receiving of data over MQTT besides the one for the OTA firmware update,
/// if this method is used ensure to call all subscribe methods again so they can be resubscribed, in the method passed to the finished_callback if the update failed and we do not restart the device
void update_starting_callback() {
    ESP_LOGI("OTA", "Starting update...");
}

/// @brief End callback method that will be called as soon as the OTA firmware update, either finished successfully or failed.
/// Is meant to allow to either restart the device if the udpate was successfull or to restart any stopped services before the update started in the subscribed update_starting_callback
/// @param success Either true (update successful) or false (update failed)
void finished_callback(const bool & success) {
    if (success) {
        ESP_LOGI("OTA", "Done, Reboot now");
        esp_restart();
        return;
    }
    ESP_LOGE("OTA", "Downloading firmware failed");
}

/// @brief Progress callback method that will be called every time our current progress of downloading the complete firmware data changed,
/// meaning it will be called if the amount of already downloaded chunks increased.
/// Is meant to allow to display a progress bar or print the current progress of the update into the console with the currently already downloaded amount of chunks and the total amount of chunks
/// @param current Already received and processs amount of chunks
/// @param total Total amount of chunks we need to receive and process until the update has completed
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
        const OTA_Update_Callback callback(CURRENT_FIRMWARE_TITLE, CURRENT_FIRMWARE_VERSION, &updater, &finished_callback, &progress_callback, &update_starting_callback, FIRMWARE_FAILURE_RETRIES, FIRMWARE_PACKET_SIZE);
        // See https://thingsboard.io/docs/user-guide/ota-updates/
        // to understand how to create a new OTA pacakge and assign it to a device so it can download it.
        // Sending the request again after a successfull update will automatically send the UPDATED firmware state,
        // because the assigned firmware title and version on the cloud and the firmware version and title we booted into are the same.
        updateRequestSent = ota.Subscribe_Firmware_Update(callback);
        ESP_LOGI("OTA", "Subscribed to firmware update");
    }
    // tbClient.loop();
    vTaskDelete(NULL);
}

void ota_task_init() {
    xTaskCreate(ota_task, "OTA_Task", 4096, NULL, 2, NULL);
}
