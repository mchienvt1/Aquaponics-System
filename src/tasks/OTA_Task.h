#ifndef OTA_TASK_H_
#define OTA_TASK_H_

#include "global.h"

void print_sha256(const uint8_t *image_hash, const char *label);

void get_sha256_of_partitions(void);

esp_err_t validate_image_header(esp_app_desc_t *new_app_info);

void OTA_task_init();

#endif // OTA_H