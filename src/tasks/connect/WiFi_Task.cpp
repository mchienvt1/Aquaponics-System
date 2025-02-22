#include "WiFi_Task.h"

volatile bool wifi_need_reconnect = false;

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
        delay(WIFI_TIMER);
    }

    set_rgb_color(GREEN_RGB);
    ESP_LOGI("WIFI", "Connected to SSID: %s", WIFI_SSID);
    ESP_LOGI("WIFI", "IP Address: %s", WiFi.localIP().toString().c_str());
    
    // Check if need to reconnect
    while (true) {
        if (WiFi.status() != WL_CONNECTED) {
            if (!wifi_need_reconnect) {
                wifi_need_reconnect = true;
                Serial.println("WiFi Disconnected");
                WiFi.disconnect();
                WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
                Serial.println("Reconnecting to SSID: " + String(WIFI_SSID));
                set_rgb_color(RED_RGB);
            }
        }
        if (wifi_need_reconnect && WiFi.status() == WL_CONNECTED) {
            wifi_need_reconnect = false;
        }
        // ESP_LOGI("WIFI", "WiFi mode: %d ", WiFi.getMode());
        delay(WIFI_TIMER);
    }
}

// Function to initialize the Wi-Fi task
void wifi_task_init() {
    xTaskCreate(wifi_task, "WiFi_Task", 4096, NULL, 1, NULL);
}   