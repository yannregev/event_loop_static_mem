#ifndef INCLUDES_H
#define INCLUDES_H

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef const void (*Function_t)(uint16_t, const void *);

typedef struct {
    Function_t func;
    uint16_t size;
    void *data;
}Node_t;

#endif //INCLUDES_H

