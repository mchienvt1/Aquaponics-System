#include "OTA_Task.h"

// WiFiClient wifiClient;
HTTPClient http;

const String OTA_DATABASE_SERVER_URL = FIREBASE_DATABASE_URL;

void print_sha256(const uint8_t *image_hash, const char *label)
{
    char hash_print[HASH_LEN * 2 + 1];
    hash_print[HASH_LEN * 2] = 0;
    for (int i = 0; i < HASH_LEN; ++i) {
        sprintf(&hash_print[i * 2], "%02x", image_hash[i]);
    }
    Serial.println(label);
    Serial.println(hash_print);
}

void get_sha256_of_partitions(void)
{
    uint8_t sha_256[HASH_LEN] = { 0 };
    esp_partition_t partition;

    // get sha256 digest for bootloader
    partition.address   = ESP_BOOTLOADER_OFFSET;
    partition.size      = ESP_PARTITION_TABLE_OFFSET;
    partition.type      = ESP_PARTITION_TYPE_APP;
    esp_partition_get_sha256(&partition, sha_256);
    print_sha256(sha_256, "SHA-256 for bootloader: ");

    // get sha256 digest for running partition
    esp_partition_get_sha256(esp_ota_get_running_partition(), sha_256);
    print_sha256(sha_256, "SHA-256 for current firmware: ");
}

String get_firmware_version(String payload) {
    return "";
}

String get_firmware_url(String payload) {
    return "";
}

void perform_OTA(const String &firmware_url) {
    ESP_LOGI("OTA", "Starting OTA example task");
// #ifdef CONFIG_EXAMPLE_FIRMWARE_UPGRADE_BIND_IF
//     esp_netif_t *netif = get_example_netif_from_desc(bind_interface_name);
//     if (netif == NULL) {
//         ESP_LOGE(TAG, "Can't find netif from interface description");
//         abort();
//     }
//     struct ifreq ifr;
//     esp_netif_get_netif_impl_name(netif, ifr.ifr_name);
//     ESP_LOGI(TAG, "Bind interface name is %s", ifr.ifr_name);
// #endif
    esp_http_client_config_t config = {
        .url = firmware_url.c_str(),
// #ifdef CONFIG_EXAMPLE_USE_CERT_BUNDLE
//         .crt_bundle_attach = esp_crt_bundle_attach,
// #else
//         .cert_pem = (char *)server_cert_pem_start,
// #endif /* CONFIG_EXAMPLE_USE_CERT_BUNDLE */
//        .event_handler = _http_event_handler,
        .keep_alive_enable = true,
// #ifdef CONFIG_EXAMPLE_FIRMWARE_UPGRADE_BIND_IF
//         .if_name = &ifr,
// #endif
    };

// #ifdef CONFIG_EXAMPLE_FIRMWARE_UPGRADE_URL_FROM_STDIN
//     char url_buf[OTA_URL_SIZE];
//     if (strcmp(config.url, "FROM_STDIN") == 0) {
//         example_configure_stdin_stdout();
//         fgets(url_buf, OTA_URL_SIZE, stdin);
//         int len = strlen(url_buf);
//         url_buf[len - 1] = '\0';
//         config.url = url_buf;
//     } else {
//         ESP_LOGE(TAG, "Configuration mismatch: wrong firmware upgrade image url");
//         abort();
//     }
// #endif

// #ifdef CONFIG_EXAMPLE_SKIP_COMMON_NAME_CHECK
//     config.skip_cert_common_name_check = true;
// #endif

    // esp_https_ota_config_t ota_config = {
    //     .http_config = &config,
    // };
    // ESP_LOGI(TAG, "Attempting to download update from %s", config.url);
    Serial.println("Attempting to download update from " + String(config.url));
    esp_err_t ret = esp_https_ota(&config);
    if (ret == ESP_OK) {
        // ESP_LOGI(TAG, "OTA Succeed, Rebooting...");
        Serial.println("OTA Succeed, Rebooting...");
        esp_restart();
    } else {
        // ESP_LOGE(TAG, "Firmware upgrade failed");
        Serial.println("Firmware upgrade failed");
    }
}

void check_for_update() {

    ESP_LOGI("OTA", "URL: %s", OTA_DATABASE_SERVER_URL.c_str());
    http.begin(espClient, OTA_DATABASE_SERVER_URL);
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK)
    {   
        // Get version and url of the latest firmware
        String payload = http.getString();
        ESP_LOGI("OTA", "Payload: %s", payload.c_str());
        // String firmware_version = get_firmware_version(payload);
        // String firmware_url = get_firmware_url(payload);

        // Serial.print("New version: ");
        // Serial.println(firmware_version.c_str());
        // Serial.println("\n");

        // int ver1 = 1;
        // int ver2 = std::stoi(firmware_url.c_str());

        // // Compare versions
        // if (ver1 < ver2)
        // {
        //     perform_OTA(firmware_url);
        // }
    }
    else
    {
        ESP_LOGE("OTA", "Error on HTTP request %d\n", httpCode);
    }
    http.end();
}

void OTA_task(void *pvParameters) {
    while (true) {
        check_for_update();
        vTaskDelay(OTA_TIMER / portTICK_PERIOD_MS);
    }
}

void OTA_task_init() {
    xTaskCreate(OTA_task, "OTA_Task", 8192, NULL, 5, NULL);
}

// ADVANCED EXAMPLE: https://github.com/espressif/esp-idf/blob/master/examples/system/ota/advanced_https_ota/main/advanced_https_ota_example.c


// #include "OTA_Task.h"

// HTTPClient http;

// const String OTA_DATABASE_SERVER_URL = FIREBASE_DATABASE_URL;

// void print_sha256(const uint8_t *image_hash, const char *label)
// {
//     char hash_print[HASH_LEN * 2 + 1];
//     hash_print[HASH_LEN * 2] = 0;
//     for (int i = 0; i < HASH_LEN; ++i) {
//         sprintf(&hash_print[i * 2], "%02x", image_hash[i]);
//     }
//     Serial.println(label);
//     Serial.println(hash_print);
// }

// void get_sha256_of_partitions(void)
// {
//     uint8_t sha_256[HASH_LEN] = { 0 };
//     esp_partition_t partition;

//     // get sha256 digest for bootloader
//     partition.address   = ESP_BOOTLOADER_OFFSET;
//     partition.size      = ESP_PARTITION_TABLE_OFFSET;
//     partition.type      = ESP_PARTITION_TYPE_APP;
//     esp_partition_get_sha256(&partition, sha_256);
//     print_sha256(sha_256, "SHA-256 for bootloader: ");

//     // get sha256 digest for running partition
//     esp_partition_get_sha256(esp_ota_get_running_partition(), sha_256);
//     print_sha256(sha_256, "SHA-256 for current firmware: ");
// }

// String get_firmware_version(String payload) {
//     return "";
// }

// String get_firmware_url(String payload) {
//     return "";
// }

// esp_err_t validate_image_header(esp_app_desc_t *new_app_info)
// {
//     if (new_app_info == NULL) {
//         return ESP_ERR_INVALID_ARG;
//     }

//     const esp_partition_t *running = esp_ota_get_running_partition();
//     esp_app_desc_t running_app_info;
//     if (esp_ota_get_partition_description(running, &running_app_info) == ESP_OK) {
//         // ESP_LOGI(TAG, "Running firmware version: %s", running_app_info.version);
//         Serial.println("Running firmware version: " + String(running_app_info.version));
//     }

// // #ifndef CONFIG_EXAMPLE_SKIP_VERSION_CHECK
// //     if (memcmp(new_app_info->version, running_app_info.version, sizeof(new_app_info->version)) == 0) {
// //         ESP_LOGW(TAG, "Current running version is the same as a new. We will not continue the update.");
// //         return ESP_FAIL;
// //     }
// // #endif

// #ifdef CONFIG_BOOTLOADER_APP_ANTI_ROLLBACK
//     /**
//      * Secure version check from firmware image header prevents subsequent download and flash write of
//      * entire firmware image. However this is optional because it is also taken care in API
//      * esp_https_ota_finish at the end of OTA update procedure.
//      */
//     const uint32_t hw_sec_version = esp_efuse_read_secure_version();
//     if (new_app_info->secure_version < hw_sec_version) {
//         // ESP_LOGW(TAG, "New firmware security version is less than eFuse programmed, %"PRIu32" < %"PRIu32, new_app_info->secure_version, hw_sec_version);
//         Serial.println("New firmware security version is less than eFuse programmed, " + String(new_app_info->secure_version) + " < " + String(hw_sec_version));
//         return ESP_FAIL;
//     }
// #endif

//     return ESP_OK;
// }

// static esp_err_t _http_client_init_cb(esp_http_client_handle_t http_client)
// {
//     esp_err_t err = ESP_OK;
//     /* Uncomment to add custom headers to HTTP request */
//     // err = esp_http_client_set_header(http_client, "Custom-Header", "Value");
//     return err;
// }

// void perform_OTA(const String &firmware_url) {
//     // ESP_LOGI(TAG, "Starting Advanced OTA example");
//     Serial.println("Starting Advanced OTA example");

//     esp_err_t ota_finish_err = ESP_OK;
//     esp_http_client_config_t config = {
//         .url = firmware_url.c_str(),
//         // .cert_pem = (char *)server_cert_pem_start,
//         .timeout_ms = OTA_RECV_TIMEOUT_MS,
//         .keep_alive_enable = true,
// // #ifdef CONFIG_EXAMPLE_ENABLE_PARTIAL_HTTP_DOWNLOAD
// //         .save_client_session = true,
// // #endif
//     };

// // #ifdef CONFIG_EXAMPLE_FIRMWARE_UPGRADE_URL_FROM_STDIN
// //     char url_buf[OTA_URL_SIZE];
// //     if (strcmp(config.url, "FROM_STDIN") == 0) {
// //         example_configure_stdin_stdout();
// //         fgets(url_buf, OTA_URL_SIZE, stdin);
// //         int len = strlen(url_buf);
// //         url_buf[len - 1] = '\0';
// //         config.url = url_buf;
// //     } else {
// //         ESP_LOGE(TAG, "Configuration mismatch: wrong firmware upgrade image url");
// //         abort();
// //     }
// // #endif

// // #ifdef CONFIG_EXAMPLE_SKIP_COMMON_NAME_CHECK
// //     config.skip_cert_common_name_check = true;
// // #endif

//     esp_https_ota_config_t ota_config = {
//         .http_config = &config,
//         .http_client_init_cb = _http_client_init_cb, // Register a callback to be invoked after esp_http_client is initialized
// // #ifdef CONFIG_EXAMPLE_ENABLE_PARTIAL_HTTP_DOWNLOAD
// //         .partial_http_download = true,
// //         .max_http_request_size = CONFIG_EXAMPLE_HTTP_REQUEST_SIZE,
// // #endif
//     };

//     esp_https_ota_handle_t https_ota_handle = NULL;
//     esp_err_t err = esp_https_ota_begin(&ota_config, &https_ota_handle);
//     if (err != ESP_OK) {
//         // ESP_LOGI(TAG, "ESP HTTPS OTA Begin failed");
//         Serial.println("ESP HTTPS OTA Begin failed");
//         vTaskDelete(NULL);
//     }

//     esp_app_desc_t app_desc;
//     err = esp_https_ota_get_img_desc(https_ota_handle, &app_desc);
//     if (err != ESP_OK) {
//         // ESP_LOGI(TAG, "esp_https_ota_get_img_desc failed");
//         Serial.println("esp_https_ota_get_img_desc failed");
//         goto ota_end;
//     }
//     err = validate_image_header(&app_desc);
//     if (err != ESP_OK) {
//         // ESP_LOGI(TAG, "image header verification failed");
//         Serial.println("image header verification failed");
//         goto ota_end;
//     }

//     while (1) {
//         err = esp_https_ota_perform(https_ota_handle);
//         if (err != ESP_ERR_HTTPS_OTA_IN_PROGRESS) {
//             break;
//         }
//         // esp_https_ota_perform returns after every read operation which gives user the ability to
//         // monitor the status of OTA upgrade by calling esp_https_ota_get_image_len_read, which gives length of image
//         // data read so far.
//         // ESP_LOGD(TAG, "Image bytes read: %d", esp_https_ota_get_image_len_read(https_ota_handle));
//         Serial.println("Image bytes read: " + String(esp_https_ota_get_image_len_read(https_ota_handle)));
//     }

//     if (esp_https_ota_is_complete_data_received(https_ota_handle) != true) {
//         // the OTA image was not completely received and user can customise the response to this situation.
//         // ESP_LOGI(TAG, "Complete data was not received.");
//         Serial.println("Complete data was not received.");
//     } 
//     else {
//         ota_finish_err = esp_https_ota_finish(https_ota_handle);
//         if ((err == ESP_OK) && (ota_finish_err == ESP_OK)) {
//             // ESP_LOGI(TAG, "ESP_HTTPS_OTA upgrade successful. Rebooting ...");
//             Serial.println("ESP_HTTPS_OTA upgrade successful. Rebooting ...");
//             vTaskDelay(1000 / portTICK_PERIOD_MS);
//             esp_restart();
//         } 
//         else {
//             if (ota_finish_err == ESP_ERR_OTA_VALIDATE_FAILED) {
//                 // ESP_LOGI(TAG, "Image validation failed, image is corrupted");
//                 Serial.println("Image validation failed, image is corrupted");
//             }
//             // ESP_LOGI(TAG, "ESP_HTTPS_OTA upgrade failed 0x%x", ota_finish_err);
//             Serial.println("ESP_HTTPS_OTA upgrade failed 0x" + String(ota_finish_err));
//             // vTaskDelete(NULL);
//         }
//     }

// ota_end:
//     esp_https_ota_abort(https_ota_handle);
//     // ESP_LOGI(TAG, "ESP_HTTPS_OTA upgrade failed");
//     Serial.println("ESP_HTTPS_OTA upgrade failed");
//     // vTaskDelete(NULL);
// }

// void check_for_update() {
//     http.begin(OTA_DATABASE_SERVER_URL);
//     int httpCode = http.GET();
//     if (httpCode == HTTP_CODE_OK)
//     {   
//         // Get version and url of the latest firmware
//         String payload = http.getString();
//         ESP_LOGI("OTA", "Payload: %s", payload.c_str());
//         // String firmware_version = get_firmware_version(payload);
//         // String firmware_url = get_firmware_url(payload);

//         // Serial.print("New version: ");
//         // Serial.println(firmware_version.c_str());
//         // Serial.println("\n");

//         // int ver1 = 1;
//         // int ver2 = std::stoi(firmware_url.c_str());

//         // // Compare versions
//         // if (ver1 < ver2)
//         // {
//         //     perform_OTA(firmware_url);
//         // }
//     }
//     else
//     {
//         Serial.print("Failed to retrieve firmware info, HTTP code: " + String(httpCode));
//     }
//     http.end();
// }

// void OTA_task(void *pvParameters) {
//     while (true) {
//         check_for_update();
//         vTaskDelay(OTA_TIMER / portTICK_PERIOD_MS);
//     }
// }

// void OTA_task_init() {
//     xTaskCreate(OTA_task, "OTA_Task", 8192, NULL, 5, NULL);
// }


