#include "WiFi_Task.h"

void wifi_connect() {
    WiFi.mode(WIFI_STA);    
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    // Loop if not connected
    while (WiFi.status() != WL_CONNECTED) {
        // Print status
        ESP_LOGI("WIFI", "WiFi Status: %d ", WiFi.status());
        delay(WIFI_TIMER);
    }

    ESP_LOGI("WIFI", "Connected to SSID: %s", WIFI_SSID);
    ESP_LOGI("WIFI", "IP Address: %s", WiFi.localIP().toString().c_str());
}

bool wifi_need_reconnect() {
    if (WiFi.status() == WL_CONNECTED) return false;
    wifi_connect();
    return false;
}

// Task to handle Wi-Fi connection
void wifi_task(void *pvParameters) {
    while (true) {
        if (wifi_need_reconnect()) continue;
        String wifi_data =  "{\"rssi\":\"" + String(WiFi.RSSI()) + 
                            "\",\"channel\":\"" + String(WiFi.channel()) +
                            "\",\"bssid\":\"" + WiFi.BSSIDstr() +
                            "\",\"localIp\":\"" + WiFi.localIP().toString() +
                            "\",\"ssid\":\"" + WiFi.SSID() + "\"}";
        // ESP_LOGI("WIFI", "Data: %s", wifi_data.c_str());
        update_wifi_data(wifi_data);
        delay(WIFI_TIMER);
    }
}

// Function to initialize the Wi-Fi task
void wifi_task_init() {
    wifi_connect();
    xTaskCreate(wifi_task, "WiFi_Task", 4096, NULL, 1, NULL);
}   