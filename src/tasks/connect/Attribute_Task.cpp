#include "Attribute_Task.h"

void requestTimedOut()
{
    Serial.printf("Attribute request timed out did not receive a response in (%llu) microseconds. Ensure client is connected to the MQTT broker and that the keys actually exist on the target device\n", REQUEST_TIMEOUT_MICROSECONDS);
}

void processSharedAttributeUpdate(const JsonObjectConst &data)
{

    for (auto it = data.begin(); it != data.end(); ++it)
    {
        Serial.println(it->key().c_str());
        Serial.println(it->value().as<const char *>());
    }

    const size_t jsonSize = Helper::Measure_Json(data);
    char buffer[jsonSize];
    serializeJson(data, buffer, jsonSize);
    Serial.println(buffer);
}

const Shared_Attribute_Callback<MAX_ATTRIBUTES> callback(
    &processSharedAttributeUpdate,
    SHARED_ATTRIBUTES_LIST);

bool shared_attributes_setup()
{
    if (!shared_attributes.Shared_Attributes_Subscribe(callback))
    {
        ESP_LOGE("SHARED_ATTR", "Failed to subscribe to shared attributes");
        return false;
    }
    ESP_LOGI("SHARED_ATTR", "Subscribed to shared attributes");
    return true;
}

void processRelay(const JsonObjectConst &data)
{
    digitalWrite(RELAY_CH1, data[RELAY_1_STATUS].as<int>());
    digitalWrite(RELAY_CH2, data[RELAY_2_STATUS].as<int>());
    digitalWrite(RELAY_CH3, data[RELAY_3_STATUS].as<int>());
    set_rgb_color(GREEN_RGB);
}
void processClientAttributeRequest(const JsonObjectConst &data)
{
    processRelay(data); 
}

void request_client_attributes()
{
    Serial.println("Requesting client-side attributes...");
    // Client-side attributes we want to request from the server
    set_rgb_color(BLUE_RGB);
    const Attribute_Request_Callback<MAX_ATTRIBUTES> clientCallback(&processClientAttributeRequest, REQUEST_TIMEOUT_MICROSECONDS, &requestTimedOut, REQUESTED_CLIENT_ATTRIBUTES);
    bool requestedClient = attr_request.Client_Attributes_Request(clientCallback);
    if (!requestedClient)
    {
        Serial.println("Failed to request client-side attributes");
    }
}



void processSharedAttributeRequest(const JsonObjectConst &data)
{
    for (auto it = data.begin(); it != data.end(); ++it)
    {
        Serial.println(it->key().c_str());
        // Shared attributes have to be parsed by their type.
        Serial.println(it->value().as<const char *>());
    }
    const size_t jsonSize = Helper::Measure_Json(data);
    char buffer[jsonSize];
    serializeJson(data, buffer, jsonSize);
    Serial.println(buffer);
}

void request_shared_attributes()
{
    Serial.println("Requesting shared attributes...");
    // Shared attributes we want to request from the server
    const Attribute_Request_Callback<MAX_ATTRIBUTES> sharedCallback(&processSharedAttributeRequest, REQUEST_TIMEOUT_MICROSECONDS, &requestTimedOut, REQUESTED_SHARED_ATTRIBUTES);
    bool requestedShared = attr_request.Shared_Attributes_Request(sharedCallback);
    if (!requestedShared)
    {
        Serial.println("Failed to request shared attributes");
    }
}