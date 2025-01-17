#ifndef GPS_H_
#define GPS_H_

#include "global.h"

struct gps_location {
    double lat, lng;
};

void GPS_task_init();

#endif