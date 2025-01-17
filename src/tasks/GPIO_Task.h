#ifndef GPIO_H_
#define GPIO_H_

#include "global.h"

struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

extern int GPIO_Relay_Pin[6];

extern const Color RED_RGB;
extern const Color GREEN_RGB;
extern const Color BLUE_RGB;
extern const Color WHITE_RGB;
extern const Color BLACK_RGB;

void set_rgb_color(Color c);

void write_relay_pin(String pin, uint8_t mode);

void gpio_task_init();

#endif