#include "MQTT_Task.h"

WiFiClient espClient;
PubSubClient psClient(espClient);

void publish_data(String type, String feed, String data) {
    // String topic = BOARD_ID + DELIMITER + type + DELIMITER + feed;
    String topic =  "2003." + type + "." + feed;
    if (psClient.connected()) {
        ESP_LOGI("MQTT", "Publishing to topic: %s", topic.c_str());
        psClient.publish(topic.c_str(), data.c_str());
    }
}

void subscribe(String type, String feed) {
    // String topic = BOARD_ID + DELIMITER + type + DELIMITER + feed;
    String topic = "2003." + type + "." + feed;
    if (psClient.connected())
    {
        ESP_LOGI("MQTT", "Subscribing to topic: %s", topic.c_str());
        psClient.subscribe(topic.c_str());
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
    // Print message
    ESP_LOGI("MQTT", "Message arrived [%s]", topic);

    // Print payload
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    return;
    
    // Parsing message
    char* token = strtok(topic, DELIMITER);
    char* topic_parts[4];
    uint8_t idx = 0;

    // // Process each token until strtok returns NULL (end of string)
    // while (token != nullptr && idx < 4) {
    //     // Output the token
    //     topic_parts[idx] = token;

    //     // Get the next token
    //     token = strtok(nullptr, DELIMITER);

    //     // Increment index
    //     idx++;
    // }
    // for (unsigned int i = 0; i < length; i++) payload[i] = 0;
    // // Check if the message is for this board
    // if (std::atoi(topic_parts[0]) != BOARD_ID) {
    //     return;
    // }
    // // If the message is for the relay status
    // if (strcmp(topic_parts[1], RELAY_STATUS) == 0) {
    //     // TODO: Parsing payload
    //     return;
    // }
    // // If the message is for the relay control
    // if (strcmp(topic_parts[1], RELAY_CONTROL) == 0) {
    //     // Write data to relay
    //     uint8_t mode = payload[0] - '0';
    //     write_relay_pin(topic_parts[2], mode);

    //     // TODO: Send ACK to the sender

    //     // Publish relay status after writing data to relay
    //     return;
    // }
}

static void subscribe_relay_topics() {
    // Create and subscribe to relays' status topics
    // Topic: <BOARD_ID>/Relay_Status/<RELAY_ID>
    subscribe("relay", "ack");

    // Create and publish relays' control topics
    // Topic: <BOARD_ID>/Relay_Control/<RELAY_ID>
    subscribe("relay.controller", "1");
    // publish_data(RELAY_CONTROL, "2", String(digitalRead(RELAY_CH2)));
    // publish_data(RELAY_CONTROL, "3", String(digitalRead(RELAY_CH3)));
    // publish_data(RELAY_CONTROL, "4", String(digitalRead(RELAY_CH4)));
    // publish_data(RELAY_CONTROL, "5", String(digitalRead(RELAY_CH5)));
    // publish_data(RELAY_CONTROL, "6", String(digitalRead(RELAY_CH6)));
}

void mqtt_task(void *pvParameters) {
    
    // Await WiFi connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(WIFI_TIMER);
    }

    // Setup server connection
    IPAddress server(3, 106, 55, 238);
    psClient.setServer(server, MQTT_PORT);
    psClient.setCallback(callback);

    // Connect to server
    while (!psClient.connected()) {
        ESP_LOGI("MQTT", "Connecting to MQTT server");

        // Attempt to connect
        String psClientName = "Ambatukam" + String(random(0xffff), HEX);
        if (psClient.connect(psClientName.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
            ESP_LOGI("MQTT", "Connected to MQTT server");
            // Subscribe to relay topics
            subscribe_relay_topics();

            // Publish IP address
            // Topic: <BOARD_ID>/IPv4/Address
            String local_ip = WiFi.localIP().toString();
            publish_data("IPv4", String("Address"), local_ip);

            // Maintain connection
            while (true) {
                psClient.loop();
                delay(MQTT_LOOP_TIMER);
            }
        }
        
        // Failed to connect 
        else {
            ESP_LOGE("MQTT", "Failed, (rc = %d)", psClient.state());
            delay(MQTT_RECONNECT_TIMER);
        }
    }
}

void mqtt_task_init() {
    xTaskCreate(mqtt_task, "MQTT_Task", 4096, NULL, 1, NULL);
}