#ifndef GPIO_H_
#define GPIO_H_

#include "global.h"
#include "../configs/esp32_relay_6ch_config.h"


void GPIO_Write(uint8_t gpio_pin, uint8_t value);
void GPIO_Toggle(uint8_t gpio_pin);
void GPIOTask_INIT();

#endif