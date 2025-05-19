#include "Attribute_Task.h"

void requestTimedOut()
{
    Serial.printf("Attribute request timed out did not receive a response in (%llu) microseconds. Ensure client is connected to the MQTT broker and that the keys actually exist on the target device\n", REQUEST_TIMEOUT_MICROSECONDS);
}

void processSharedAttributeUpdate(const JsonObjectConst &data)
{
    int relayPins[] = {RELAY_CH1, RELAY_CH2, RELAY_CH3};
    bool stateChanged = false;
    nlohmann::json j = {};

    // Process relay state changes from shared attributes
    if (data.containsKey("deviceState1"))
    {
        bool state = data["deviceState1"].as<bool>();
        digitalWrite(relayPins[0], 1 - state);
        j["relay_1_status"] = state;
        stateChanged = true;
    }

    if (data.containsKey("deviceState2"))
    {
        bool state = data["deviceState2"].as<bool>();
        digitalWrite(relayPins[1], 1 - state);
        j["relay_2_status"] = state;
        stateChanged = true;
    }

    if (data.containsKey("deviceState3"))
    {
        bool state = data["deviceState3"].as<bool>();
        digitalWrite(relayPins[2], 1 - state);
        j["relay_3_status"] = state;
        stateChanged = true;
    }

    // Handle automate_prediction_button attribute
    if (data.containsKey("automate_prediction_button"))
    {
        bool automate = data["automate_prediction_button"].as<bool>();
        j["automate_prediction_status"] = automate;
        stateChanged = true;

        if (automate)
        {
            if (!ai_tasks_running)
            {
                Serial.println("Starting AI prediction tasks");
                AI_Task_Init();
            }
        }
        else
        {
            if (ai_tasks_running)
            {
                Serial.println("Stopping AI prediction tasks");
                AI_Task_Stop();
            }
        }
    }

    // Send attribute update if any state changed
    if (stateChanged)
    {
        sendAttribute(String(j.dump().c_str()));
    }

    // Debug print
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

// Update the SHARED_ATTRIBUTES_LIST macro definition in your header file to include "automate_prediction_button"
// Example: #define SHARED_ATTRIBUTES_LIST "deviceState1", "deviceState2", "deviceState3", "automate_prediction_button"

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
    request_shared_attributes();
    return true;
}

void processRelay(const JsonObjectConst &data)
{
    bool stateChanged = false;
    nlohmann::json j = {};

    // Check for deviceState keys in client attributes
    if (data.containsKey("deviceState1"))
    {
        bool state = data["deviceState1"].as<bool>();
        digitalWrite(RELAY_CH1, 1 - state);
        j["relay_1_status"] = 1 - state;
        stateChanged = true;
    }

    if (data.containsKey("deviceState2"))
    {
        bool state = data["deviceState2"].as<bool>();
        digitalWrite(RELAY_CH2, 1 - state);
        j["relay_2_status"] = 1 - state;
        stateChanged = true;
    }

    if (data.containsKey("deviceState3"))
    {
        bool state = data["deviceState3"].as<bool>();
        digitalWrite(RELAY_CH3, 1 - state);
        j["relay_3_status"] = 1 - state;
        stateChanged = true;
    }

    // Also check for automate_prediction_button in client attributes
    if (data.containsKey("automate_prediction_button"))
    {
        bool automate = data["automate_prediction_button"].as<bool>();
        j["automate_prediction_status"] = automate;
        stateChanged = true;

        if (automate)
        {
            if (!ai_tasks_running)
            {
                Serial.println("Starting AI prediction tasks from client attributes");
                AI_Task_Init();
            }
        }
        else
        {
            if (ai_tasks_running)
            {
                Serial.println("Stopping AI prediction tasks from client attributes");
                AI_Task_Stop();
            }
        }
    }

    // Send attribute update if any relay state changed
    if (stateChanged)
    {
        sendAttribute(String(j.dump().c_str()));
    }

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

    // Update the REQUESTED_CLIENT_ATTRIBUTES macro in your header file to include "automate_prediction_button"
    // Example: #define REQUESTED_CLIENT_ATTRIBUTES "deviceState1", "deviceState2", "deviceState3", "automate_prediction_button"

    const Attribute_Request_Callback<MAX_ATTRIBUTES> clientCallback(&processClientAttributeRequest, REQUEST_TIMEOUT_MICROSECONDS, &requestTimedOut, REQUESTED_CLIENT_ATTRIBUTES);
    bool requestedClient = attr_request.Client_Attributes_Request(clientCallback);
    if (!requestedClient)
    {
        Serial.println("Failed to request client-side attributes");
    }
}

void processSharedAttributeRequest(const JsonObjectConst &data)
{
    // Process any relay states found in shared attributes
    processRelay(data);

    // Also explicitly check for automate_prediction_button in the initial request
    if (data.containsKey("automate_prediction_button"))
    {
        bool automate = data["automate_prediction_button"].as<bool>();
        if (automate)
        {
            if (!ai_tasks_running)
            {
                Serial.println("Starting AI prediction tasks from initial shared attributes");
                AI_Task_Init();
            }
        }
        else
        {
            if (ai_tasks_running)
            {
                Serial.println("Stopping AI prediction tasks from initial shared attributes");
                AI_Task_Stop();
            }
        }
    }

    // Debug print
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

    // Update the REQUESTED_SHARED_ATTRIBUTES macro in your header file to include "automate_prediction_button"
    // Example: #define REQUESTED_SHARED_ATTRIBUTES "deviceState1", "deviceState2", "deviceState3", "automate_prediction_button"

    const Attribute_Request_Callback<MAX_ATTRIBUTES> sharedCallback(&processSharedAttributeRequest, REQUEST_TIMEOUT_MICROSECONDS, &requestTimedOut, REQUESTED_SHARED_ATTRIBUTES);
    bool requestedShared = attr_request.Shared_Attributes_Request(sharedCallback);
    if (!requestedShared)
    {
        Serial.println("Failed to request shared attributes");
    }
}