// #include "OTA_Task.h"

// HTTPClient http;

// const String OTA_DATABASE_SERVER_URL = FIREBASE_DATABASE_URL;
// const char* TAG = "OTA";

// static void print_sha256(const uint8_t *image_hash, const char *label)
// {
//     char hash_print[HASH_LEN * 2 + 1];
//     hash_print[HASH_LEN * 2] = 0;
//     for (int i = 0; i < HASH_LEN; ++i) {
//         sprintf(&hash_print[i * 2], "%02x", image_hash[i]);
//     }
//     Serial.println(label);
//     Serial.println(hash_print);
// }

// static void get_sha256_of_partitions(void)
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

// static esp_err_t validate_image_header(esp_app_desc_t *new_app_info)
// {
//     if (new_app_info == NULL) {
//         return ESP_ERR_INVALID_ARG;
//     }

//     const esp_partition_t *running = esp_ota_get_running_partition();
//     esp_app_desc_t running_app_info;
//     if (esp_ota_get_partition_description(running, &running_app_info) == ESP_OK) {
//         ESP_LOGI(TAG, "Running firmware version: %s", running_app_info.version);
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
//         ESP_LOGW(TAG, "New firmware security version is less than eFuse programmed, %"PRIu32" < %"PRIu32, new_app_info->secure_version, hw_sec_version);
//         return ESP_FAIL;
//     }
// #endif

//     return ESP_OK;
// }

// void perform_OTA(const String &firmware_url) {
//     ESP_LOGI(TAG, "Starting Advanced OTA example");

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
//         ESP_LOGI(TAG, "ESP HTTPS OTA Begin failed");
//         vTaskDelete(NULL);
//     }

//     esp_app_desc_t app_desc;
//     err = esp_https_ota_get_img_desc(https_ota_handle, &app_desc);
//     if (err != ESP_OK) {
//         ESP_LOGI(TAG, "esp_https_ota_get_img_desc failed");
//         goto ota_end;
//     }
//     err = validate_image_header(&app_desc);
//     if (err != ESP_OK) {
//         ESP_LOGI(TAG, "image header verification failed");
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
//         ESP_LOGD(TAG, "Image bytes read: %d", esp_https_ota_get_image_len_read(https_ota_handle));
//     }

//     if (esp_https_ota_is_complete_data_received(https_ota_handle) != true) {
//         // the OTA image was not completely received and user can customise the response to this situation.
//         ESP_LOGI(TAG, "Complete data was not received.");
//     } 
//     else {
//         ota_finish_err = esp_https_ota_finish(https_ota_handle);
//         if ((err == ESP_OK) && (ota_finish_err == ESP_OK)) {
//             ESP_LOGI(TAG, "ESP_HTTPS_OTA upgrade successful. Rebooting ...");
//             vTaskDelay(1000 / portTICK_PERIOD_MS);
//             esp_restart();
//         } 
//         else {
//             if (ota_finish_err == ESP_ERR_OTA_VALIDATE_FAILED) {
//                 ESP_LOGI(TAG, "Image validation failed, image is corrupted");
//             }
//             ESP_LOGI(TAG, "ESP_HTTPS_OTA upgrade failed 0x%x", ota_finish_err);
//             vTaskDelete(NULL);
//         }
//     }

// ota_end:
//     esp_https_ota_abort(https_ota_handle);
//     ESP_LOGI(TAG, "ESP_HTTPS_OTA upgrade failed");
//     vTaskDelete(NULL);
// }

// void check_for_update() {
//     http.begin(OTA_DATABASE_SERVER_URL);
//     int httpCode = http.GET();
//     if (httpCode == HTTP_CODE_OK)
//     {   
//         // Get version and url of the latest firmware
//         String payload = http.getString();
//         String firmware_version = get_firmware_version(payload);
//         String firmware_url = get_firmware_url(payload);

//         Serial.print("New version: ");
//         Serial.println(firmware_version.c_str());
//         Serial.println("\n");

//         int ver1 = 1;
//         int ver2 = std::stoi(firmware_url.c_str());

//         // Compare versions
//         if (ver1 < ver2)
//         {
//             perform_OTA(firmware_url);
//         }
//     }
//     else
//     {
//         Serial.print("Failed to retrieve firmware info, HTTP code:");
//         Serial.println(httpCode);
//     }
//     http.end();
// }

// void OTA_task(void *pvParameters) {
//     // Initialize NVS.
//     esp_err_t err = nvs_flash_init();
//     if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
//         // 1.OTA app partition table has a smaller NVS partition size than the non-OTA
//         // partition table. This size mismatch may cause NVS initialization to fail.
//         // 2.NVS partition contains data in new format and cannot be recognized by this version of code.
//         // If this happens, we erase NVS partition and initialize NVS again.
//         ESP_ERROR_CHECK(nvs_flash_erase());
//         err = nvs_flash_init();
//     }
//     ESP_ERROR_CHECK(err);

//     get_sha256_of_partitions();

//     // ESP_ERROR_CHECK(esp_netif_init());
//     // ESP_ERROR_CHECK(esp_event_loop_create_default());
//     // ESP_ERROR_CHECK(esp_event_handler_register(ESP_HTTPS_OTA_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));

//     /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
//      * Read "Establishing Wi-Fi or Ethernet Connection" section in
//      * examples/protocols/README.md for more information about this function.
//     */
//     // ESP_ERROR_CHECK(example_connect());

//     /**
//      * We are treating successful WiFi connection as a checkpoint to cancel rollback
//      * process and mark newly updated firmware image as active. For production cases,
//      * please tune the checkpoint behavior per end application requirement.
//      */
//     const esp_partition_t *running = esp_ota_get_running_partition();
//     esp_ota_img_states_t ota_state;
//     if (esp_ota_get_state_partition(running, &ota_state) == ESP_OK) {
//         if (ota_state == ESP_OTA_IMG_PENDING_VERIFY) {
//             if (esp_ota_mark_app_valid_cancel_rollback() == ESP_OK) {
//                 ESP_LOGI(TAG, "App is valid, rollback cancelled successfully");
//             } 
//             else {
//                 ESP_LOGI(TAG, "Failed to cancel rollback");
//             }
//         }
//     }

//     /* Ensure to disable any WiFi power save mode, this allows best throughput
//      * and hence timings for overall OTA operation.
//      */
//     esp_wifi_set_ps(WIFI_PS_NONE);
//     while (true) {
//         check_for_update();
//         vTaskDelay(10000 / portTICK_PERIOD_MS);
//     }
// }

// void OTA_task_init() {
//     xTaskCreate(OTA_task, "OTA_Task", 8192, NULL, 5, NULL);
// }

// static esp_err_t _http_client_init_cb(esp_http_client_handle_t http_client)
// {
//     esp_err_t err = ESP_OK;
//     /* Uncomment to add custom headers to HTTP request */
//     // err = esp_http_client_set_header(http_client, "Custom-Header", "Value");
//     return err;
// }
