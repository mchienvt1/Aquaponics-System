#include "MQTT_Task.h"

// Initialize used apis
OTA_Firmware_Update<> ota;
const std::array<IAPI_Implementation*, 1U> apis = {
    &ota
};

WiFiClient wifiClient;
Arduino_MQTT_Client mqttClient(wifiClient);
ThingsBoard tb(mqttClient, MAX_MESSAGE_RECEIVE_SIZE, MAX_MESSAGE_SEND_SIZE, Default_Max_Stack_Size, apis);

void publish_data(String data) {
    tb.sendTelemetryString(data.c_str());
    tb.sendAttributeString(data.c_str());
}

void mqtt_task(void *pvParameters) {
    // Await WiFi connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(WIFI_TIMER);
    }
    if (!tb.connected()) {
        // ESP_LOGI("MQTT", "Connecting to ThingsBoard server %s with token %s", THINGSBOARD_SERVER, SENSOR_TOKEN);
        if (tb.connect(THINGSBOARD_SERVER, SENSOR_TOKEN, THINGSBOARD_PORT)) {
            ESP_LOGI("MQTT", "Connected to ThingsBoard MQTT server");

        }
    }
    while (true) {
        tb.loop();
        delay(MQTT_LOOP_TIMER);
    }
}

Espressif_Updater<> updater;

bool currentFWSent = false;
bool updateRequestSent = false;

void update_starting_callback() {
    Serial.println("Starting update...");
}

void finished_callback(const bool & success) {
    if (success) {
        Serial.println("Done, Reboot now");
        esp_restart();
        return;
    }
    Serial.println("Downloading firmware failed");
}

void progress_callback(const size_t & current, const size_t & total) {
    Serial.printf("Progress %.2f%%\n", static_cast<float>(current * 100U) / total);
}

void ota_task(void *pvParameters) {
    while (true) {
        while (WiFi.status() != WL_CONNECTED || !tb.connected()) {
            delay(WIFI_TIMER);
        }
        if (!currentFWSent) {
            Serial.printf("Current FW version %s\n", CURRENT_FIRMWARE_VERSION);
            currentFWSent = ota.Firmware_Send_Info(CURRENT_FIRMWARE_TITLE, CURRENT_FIRMWARE_VERSION);
        }

        if (!updateRequestSent) {
            const OTA_Update_Callback callback(CURRENT_FIRMWARE_TITLE, CURRENT_FIRMWARE_VERSION, &updater, &finished_callback, &progress_callback, &update_starting_callback, FIRMWARE_FAILURE_RETRIES, FIRMWARE_PACKET_SIZE);
            Serial.println("Firwmare Update Subscription...");
            // See https://thingsboard.io/docs/user-guide/ota-updates/
            // to understand how to create a new OTA pacakge and assign it to a device so it can download it.
            // Sending the request again after a successfull update will automatically send the UPDATED firmware state,
            // because the assigned firmware title and version on the cloud and the firmware version and title we booted into are the same.
            updateRequestSent = ota.Subscribe_Firmware_Update(callback);
        }
        // tb.loop();
        delay(OTA_TIMER);
    }
}


void mqtt_task_init() {
    xTaskCreate(mqtt_task, "MQTT_Task", 2048, NULL, 1, NULL);
    xTaskCreate(ota_task, "OTA_Task", 4096, NULL, 2, NULL);
}