#ifndef WIFI_H_
#define WIFI_H_

#include "global.h"
#include "../configs/wifi_config.h"

extern const char *ssid;
extern const char *password;

void WifiTask_INIT();

#endif