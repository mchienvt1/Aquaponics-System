#include "MQTT_Task.h"

const char *RELAY_CONTROL = "Relay_Control";
const char *RELAY_STATUS = "Relay_Status";

WiFiClient espClient;
PubSubClient client(espClient);

void publish_data(String type, String feed, String data) {
    String topic = BOARD_ID + DELIMITER + type + DELIMITER + feed;

    // LOG DEBUG
    Serial.print("Publishing data to topic: ");
    Serial.println(topic);

    if (client.connected()) {
        client.publish(topic.c_str(), data.c_str());
    }
}

void subscribe(String type, String feed)
{
    String topic = BOARD_ID + DELIMITER + type + DELIMITER + feed;

    // LOG DEBUG
    Serial.print("Subscribing to topic: ");
    Serial.println(topic);

    if (client.connected())
    {
        client.subscribe(topic.c_str());
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
    char* token = strtok(topic, DELIMITER);
    char* topic_parts[4];
    uint8_t idx = 0;

    // Process each token until strtok returns NULL (end of string)
    while (token != nullptr && idx < 4) {
        // Output the token
        topic_parts[idx] = token;

        // Get the next token
        token = strtok(nullptr, DELIMITER);

        // Increment index
        idx++;
    }

    // Check if the message is for this board
    if (std::atoi(topic_parts[0]) != BOARD_ID) {
        return;
    }
    // Check if the message is for relay
    if (strcmp(topic_parts[1], "Relay") != 0) {
        return;
    }
    // If the message is for the relay commandment
    if (strcmp(topic_parts[2], "Command") == 0) {
        // TODO: Parsing payload
        return;
    }
    // If the message is for the relay control
    if (strcmp(topic_parts[2], "Control") == 0) {
        // Write data to relay
        // digitalWrite(GPIO_Relay_Pin[std::atoi(topic_parts[3]) - 1], payload[0]);

        // TODO: Send ACK to the sender

        // Publish relay status after writing data to relay
        // publish_data("Relay", topic_parts[3], payload[0]);
        return;
    }
}

static void subscribe_relay_topics() {
    // Create and subscribe to relays' commandment topics
    // Topic: <BOARD_ID>/Relay_Control/<RELAY_ID>
    subscribe(RELAY_CONTROL, "1");
    subscribe(RELAY_CONTROL, "2");
    subscribe(RELAY_CONTROL, "3");
    subscribe(RELAY_CONTROL, "4");
    subscribe(RELAY_CONTROL, "5");
    subscribe(RELAY_CONTROL, "6");

    // Create and publish relays' status topics
    // Topic: <BOARD_ID>/Relay_Status/<RELAY_ID>
    publish_data(RELAY_STATUS, "1", String(digitalRead(RELAY_CH1)));
    publish_data(RELAY_STATUS, "2", String(digitalRead(RELAY_CH2)));
    publish_data(RELAY_STATUS, "3", String(digitalRead(RELAY_CH3)));
    publish_data(RELAY_STATUS, "4", String(digitalRead(RELAY_CH4)));
    publish_data(RELAY_STATUS, "5", String(digitalRead(RELAY_CH5)));
    publish_data(RELAY_STATUS, "6", String(digitalRead(RELAY_CH6)));
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
        if (client.connect("ESP32Client", IO_USERNAME, IO_KEY)) {
            Serial.println("Connected to MQTT server");

            // Subscribe to relay topics
            subscribe_relay_topics();

            // Publish IP address
            // Topic: <BOARD_ID>/IPv4/Address
            String local_ip = WiFi.localIP().toString();
            publish_data("IPv4", String("Address"), local_ip);

            Serial.println(local_ip);
            Serial.println("Started");

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