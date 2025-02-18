#include "Relay_Task.h"

relay_data process_relay_payload(byte* payload) {
    relay_data data;

    // Payload format: field-target-value-interval
    // Example: relay.controller-1-1-10
    // Parsing the field token which we don't need
    char* token = strtok((char*)payload, "-");
    
    // Parsing the target token
    token = strtok(nullptr, "-");
    data.relay_ch = atoi(token);

    // Parsing the value token
    token = strtok(nullptr, "-");
    data.state = atoi(token);

    // Parsing the interval token
    token = strtok(nullptr, "-");
    data.duration = atoi(token);

    // Return data parsed from payload
    return data;
}
