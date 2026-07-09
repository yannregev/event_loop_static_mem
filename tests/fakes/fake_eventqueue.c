
#include "includes.h"


void QueueFunctionCallback(Function_t func, const uint16_t size, const void *data) {
    func(size, data);
}