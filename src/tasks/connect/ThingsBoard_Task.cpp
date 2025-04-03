#include "ThingsBoard_Task.h"

String sensor_data_str = "";
String gps_data_str = "";
String wifi_data_str = "";
String env_data_str = "";
String process_data_str = "";

bool ota_subscribed = false;
bool rpc_subscribed = false;
bool shared_attributes_subscribed = false;
bool requested_client_attributes = false;


void sendAttribute(const String&data){
    tbClient.sendAttributeString(data.c_str());
}
void sendTelemetry(const String&data){
    tbClient.sendTelemetryString(data.c_str());
}
void update_sensor_data(const String &data) {
    sensor_data_str = data;
}

void update_gps_data(const String &data) {
    gps_data_str = data;
}

void update_wifi_data(const String &data) {
    wifi_data_str = data;
}

void update_env_data(const String &data) {
    env_data_str = data;
}

void update_process_data(const String &data) {
    process_data_str = data;
}
void thingsboard_task(void *pvParameters) {
    while (true) {
        delay(THINGSBOARD_LOOP_TIMER);
        while (WiFi.status() != WL_CONNECTED) {
            delay(WIFI_TIMER);
        }
        if (!tbClient.connected()) {
            while (!tbClient.connect(THINGSBOARD_SERVER, DEVICE_TOKEN, THINGSBOARD_PORT)) {
                ESP_LOGI("TB", "Unable to connect to ThingsBoard MQTT server");
                delay(THINGSBOARD_CONNECT_TIMER);
            }
        }
        if (!rpc_subscribed) rpc_subscribed = rpc_setup();
        if (!shared_attributes_subscribed) shared_attributes_subscribed = shared_attributes_setup();
        if (!ota_subscribed) ota_subscribed = ota_setup();
        if(!requested_client_attributes) {
            request_client_attributes();
            requested_client_attributes = true;
        }
        // TODO: Make a Data Structure to store these more efficient
        // if (strcmp(sensor_data_str.c_str(), "") != 0) {
        //     tbClient.sendTelemetryString(sensor_data_str.c_str());
        //     sensor_data_str = "";
        // }
        // if (strcmp(gps_data_str.c_str(), "") != 0) {
        //     tbClient.sendAttributeString(gps_data_str.c_str());
        //     gps_data_str = "";
        // }
        // if (strcmp(wifi_data_str.c_str(), "") != 0) {
        //     tbClient.sendAttributeString(wifi_data_str.c_str());
        //     wifi_data_str = "";
        // }
        // if (strcmp(env_data_str.c_str(), "") != 0) {
        //     tbClient.sendTelemetryString(env_data_str.c_str());
        //     env_data_str = "";
        // }
        // if (strcmp(process_data_str.c_str(), "") != 0) {
        //     tbClient.sendTelemetryString(process_data_str.c_str());
        //     process_data_str = "";
        // }
        // set_rgb_color(GREEN_RGB);
        tbClient.loop();
    }
}

void thingsboard_task_init() {
    xTaskCreate(thingsboard_task, "ThingsBoard_Task", 4096, NULL, 1, NULL);
}