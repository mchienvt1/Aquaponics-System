#include "wifi_Task.h"

volatile bool need_reconnect = false;

// Task to handle Wi-Fi connection
void wifi_task(void *pvParameters) {
    
    // Wifi Mode
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    // set_rgb_color(RED_RGB);
    // Loop if not connected
    while (WiFi.status() != WL_CONNECTED) {
        // Print status
        set_rgb_color(RED_RGB);
        ESP_LOGI("WIFI", "WiFi Status: %d ", WiFi.status());
        vTaskDelay(WIFI_TIMER / portTICK_PERIOD_MS);
    }

    set_rgb_color(GREEN_RGB);
    // Successfully connected
    Serial.print("Successfully Connected to SSID: ");
    Serial.println(WIFI_SSID);

    // Print Local Address
    Serial.print("Local address: http://"); 
    Serial.println(WiFi.localIP());
    set_rgb_color(BLACK_RGB); 
    
    // Check if need to reconnect
    while (true) {
        if (WiFi.status() != WL_CONNECTED) {
            if (!need_reconnect) {
                need_reconnect = true;
                Serial.println("WiFi Disconnected");
                WiFi.disconnect();
                WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
                Serial.println("Reconnecting to SSID: " + String(WIFI_SSID));
            }
        }
        if (need_reconnect && WiFi.status() == WL_CONNECTED) {
        }
        // ESP_LOGI("WIFI", "WiFi mode: %d ", WiFi.getMode());
        vTaskDelay(WIFI_TIMER / portTICK_PERIOD_MS);
    }
}

// Function to initialize the Wi-Fi task
void wifi_task_init() {
    xTaskCreate(wifi_task, "WiFi_Task", 4096, NULL, 2, NULL);
}   