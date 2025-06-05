#include "global.h"

extern "C" void app_main(void)
{
  setup();
  while (true)
  {
    loop();
  }
}

void setup()
{
  Serial.begin(MAIN_BAUDRATE);
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);
  dispatch();
}

void loop()
{
  // Do nothing
}
