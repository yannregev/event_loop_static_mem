#ifndef INCLUDES_H
#define INCLUDES_H

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "events.h"
#include "queue.h"

typedef void (*Function_t)(uint16_t, const void *);

typedef struct {
    Function_t func;
    uint16_t size;
}Node_t;

#include "delayedfunctions.h"
#include "periodicfunctions.h"
#include "eventqueue.h"

void Events_Init(void);
void Events_Tick(void);

#endif //INCLUDES_H

