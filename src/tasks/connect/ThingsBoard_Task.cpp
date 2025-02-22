#include "ThingsBoard_Task.h"

// volatile bool tb_need_reconnect = false;

Espressif_Updater<> updater;

bool currentFWSent = false;
bool updateRequestSent = false;
bool rpc_subscribed = false;
bool shared_attributes_subscribed = false;

void ota_setup() {
    if (!currentFWSent) {
        ESP_LOGI("OTA", "Current FW: %s %s", CURRENT_FIRMWARE_TITLE, CURRENT_FIRMWARE_VERSION);
        currentFWSent = ota.Firmware_Send_Info(CURRENT_FIRMWARE_TITLE, CURRENT_FIRMWARE_VERSION);
    }

    if (!updateRequestSent) {
        const OTA_Update_Callback callback(
            CURRENT_FIRMWARE_TITLE, 
            CURRENT_FIRMWARE_VERSION,
            &updater, 
            &finished_callback, 
            &progress_callback, 
            &update_starting_callback, 
            FIRMWARE_FAILURE_RETRIES, 
            FIRMWARE_PACKET_SIZE
        );
        // See https://thingsboard.io/docs/user-guide/ota-updates/
        // to understand how to create a new OTA pacakge and assign it to a device so it can download it.
        // Sending the request again after a successfull update will automatically send the UPDATED firmware state,
        // because the assigned firmware title and version on the cloud and the firmware version and title we booted into are the same.
        updateRequestSent = ota.Subscribe_Firmware_Update(callback);
        ESP_LOGI("OTA", "Subscribed to firmware update");
    }
}

void rpc_setup() {
    if (!rpc_subscribed) {
        // ESP_LOGI("RPC", "Subscribing to RPC methods");
        const std::array<RPC_Callback, MAX_RPC_SUBSCRIPTIONS> callbacks = {
            RPC_Callback {RPC_RELAY_STATUS, process_relay_status},
            RPC_Callback {RPC_RGB_STATUS, process_rgb_status}
        };
        if (!rpc.RPC_Subscribe(callbacks.cbegin(), callbacks.cend())) {
            ESP_LOGE("RPC", "Failed to subscribe to RPC methods");
        }   
        ESP_LOGI("RPC", "Subscribed to RPC methods");
        rpc_subscribed = true;
    }
}

void shared_attributes_setup() {
    if (!shared_attributes_subscribed) {
        const Shared_Attribute_Callback<MAX_ATTRIBUTES> callback(
            processSharedAttributeUpdate,
            SHARED_ATTRIBUTES_LIST.cbegin(), 
            SHARED_ATTRIBUTES_LIST.cend()
        );
        if (!shared_attributes.Shared_Attributes_Subscribe(callback)) {
            ESP_LOGE("SHARED_ATTR", "Failed to subscribe to shared attributes");
        }
        ESP_LOGI("SHARED_ATTR", "Subscribed to shared attributes");
        shared_attributes_subscribed = true; 
    }
}

void thingsboard_task(void *pvParameters) {
    while (WiFi.status() != WL_CONNECTED) {
        delay(WIFI_TIMER);
    }
    if (!tbClient.connected()) {
        // ESP_LOGI("MQTT", "Connecting to ThingsBoard server %s with token %s", THINGSBOARD_SERVER, SENSOR_TOKEN);
        while (!tbClient.connect(THINGSBOARD_SERVER, DEVICE_TOKEN, THINGSBOARD_PORT)) {
            ESP_LOGI("TB", "Connecting to ThingsBoard MQTT server");
            delay(THINGSBOARD_CONNECT_TIMER);
        }
        ESP_LOGI("TB", "Connected to ThingsBoard MQTT server with token %s", DEVICE_TOKEN);
        ESP_LOGI("TB", "Setting up Tasks");
        ota_setup();
        rpc_setup();
        shared_attributes_setup();
    }
    while (true) {
        tbClient.loop();
        delay(THINGSBOARD_LOOP_TIMER);
    }
}

void thingsboard_task_init() {
    xTaskCreate(thingsboard_task, "ThingsBoard_Task", 4096, NULL, 1, NULL);
}