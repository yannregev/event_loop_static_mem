#ifndef EVENTS_H
#define EVENTS_H

typedef enum {
    EVENT_NONE,
    EV_AIR_QUALITY_MEASURED,

    NUMBER_OF_EVENTS,
}Events_t;

typedef struct {
    uint16_t pm10, pm25;
}EV_AIR_QUALITY_MEASURED_t;

#endif