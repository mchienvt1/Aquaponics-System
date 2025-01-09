#include "MQTT_Task.h"

WiFiClient espClient;
PubSubClient client(espClient);

void publish_data(const char* topic, const char* payload) {
    if (client.connected()) {
        client.publish(topic, payload);
    }
}

void subscribe(const char* topic)
{
    if (client.connected())
    {
        client.subscribe(topic);
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
    // Print message
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");

    // Print payload
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();

    // Parsing message
}

void mqtt_task(void *pvParameters) {
    
    // Await WiFi connection
    while (WiFi.status() != WL_CONNECTED) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    // Setup server connection
    client.setServer(MQTT_SERVER, MQTT_PORT);
    client.setCallback(callback);

    // Connect to server
    while (!client.connected()) {
        Serial.println("Connecting to MQTT server");

        // Attempt to connect
        if (client.connect("ESP32Client", MQTT_USER, MQTT_PASSWORD)) {
            Serial.println("Connected to MQTT server");

            // Subscribe to topics
            subscribe("mqtt_test");

            // Maintain connection
            while (1) {
                client.loop();
                vTaskDelay(100 / portTICK_PERIOD_MS);
            }
        }
        
        // Failed to connect 
        else {
            Serial.println("Failed to connect to MQTT server");
            vTaskDelay(5000 / portTICK_PERIOD_MS);
        }
    }
}

void mqtt_task_init() {
    xTaskCreate(mqtt_task, "MQTT_Task", 4096, NULL, 1, NULL);
}