#include "wifi_Task.h"

const char* SSID = PROJECT_WIFI_SSID;
const char* PASSWORD = PROJECT_WIFI_PASSWORD;

// Task to handle Wi-Fi connection
void wifi_task(void *pvParameters) {
    // Connecting attempts
    Serial.print("Connect to SSID: ");
    Serial.println(SSID);

    // Wifi Station Mode
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASSWORD);

    // Loop if not connected
    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("Connecting to WiFi..");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    // Successfully connected
    Serial.print("Successfully Connected to SSID: ");
    Serial.println(SSID);

    // Print Local Address
    Serial.print("Local address: http://");
    Serial.println(WiFi.localIP());

    // Terminate Task since the connection is established
    vTaskDelete(NULL);
}

void wifi_task_init() {
    xTaskCreate(wifi_task, "WiFi_Task", 4096, NULL, 1, NULL);
}