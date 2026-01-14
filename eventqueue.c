/*
Map to list of functions
*/

#include "eventqueue.h"
#include "criticalsection.h"

#define CLOSURE_MEM_SIZE NUMBER_OF_EVENTS
#define MAX_EVENT_LISTENERS 10

static Function_t closures[CLOSURE_MEM_SIZE][MAX_EVENT_LISTENERS];
static Queue_t eventQueue;

void Run_EventQueue(void) {
    if (IsQueueEmpty(&eventQueue)) { return; }
    while (!IsQueueEmpty(&eventQueue)) {
        BEGIN_CRITICAL_SECTION
        uint8_t buffer[sizeof(Node_t)];
        for (int i = 0; i < sizeof(buffer); i++) {
            buffer[i] =  Dequeue(&eventQueue);
        }
        Node_t *node = (Node_t*)buffer;
        uint8_t data[node->size];
        for (int i = 0; i < node->size; i++) {
            data[i] = Dequeue(&eventQueue);
        }
	    Function_t func = node->func;
        END_CRITICAL_SECTION
        func(node->size, &data);
    }
}

void QueueFunctionCallback(Function_t func) {
    if (func == NULL) { return; }
    const size_t nodeSize = sizeof(Node_t);
    uint8_t buffer[nodeSize];
    Node_t *node = (Node_t*)buffer;
    node->func = func;
    node->size = 0;
    BEGIN_CRITICAL_SECTION
    for (int i = 0; i < nodeSize; i++) {
        Enqueue(&eventQueue, buffer[i]);
    }
    END_CRITICAL_SECTION
}

void EventActivate(uint16_t event, const uint16_t size, const void *data) {
    if (event == EVENT_NONE || event >= NUMBER_OF_EVENTS) { return; }

    size_t nodeSize = sizeof(Node_t) + size;

    uint16_t entryIndex = 0;
    BEGIN_CRITICAL_SECTION
    Function_t *entry = closures[event];
    while (entry[entryIndex] != NULL && entryIndex < MAX_EVENT_LISTENERS) {

        size_t free = QueueFreeSpace(&eventQueue);
        if (nodeSize > free) {
            fprintf(stderr, "Out of memory for closures\n");
            END_CRITICAL_SECTION
            return;
        }
        uint8_t buffer[nodeSize];
        Node_t *node = (Node_t*)buffer;
        node->size = size;
        node->func = entry[entryIndex];
        memcpy(&buffer[sizeof(Node_t)], data, size);
        for (int i = 0; i < nodeSize; i++) {
            Enqueue(&eventQueue, buffer[i]);
        }
        entryIndex++;
    }
    END_CRITICAL_SECTION
}


void EventAddCallback(uint16_t event, Function_t func) {
    if (func == NULL || event >= NUMBER_OF_EVENTS) { return; }

    BEGIN_CRITICAL_SECTION
    Function_t *entry = closures[event];
    while (*entry != NULL && *entry != func) {  // Find fresh memory
        entry++;
    }

    if (*entry == func) { return; } // Event and func already registered

    if (entry == &closures[event][MAX_EVENT_LISTENERS]) {
        fprintf(stderr, "Out of closure mem");
        return;
    }
    *entry = func;
    END_CRITICAL_SECTION
}

void EventRemoveCallback(uint16_t event, Function_t func) {
    if (func == NULL || event >= NUMBER_OF_EVENTS) { return; }
    Function_t *entry = closures[event];

    while (*entry != func && *entry != NULL) {
        entry++;
    }
    if (*entry == NULL) { return; } // Closure does not exist in list
    BEGIN_CRITICAL_SECTION
    while (*entry != NULL) {    // Shrink array
        *entry = *(entry + 1);
        entry++;
    }
    END_CRITICAL_SECTION
}




void EventQueue_Init(void) {
    INITIALIZE_CRITICAL_SECTION
    memset(closures, 0, sizeof(void*) * CLOSURE_MEM_SIZE * MAX_EVENT_LISTENERS);
    QueueInit(&eventQueue);
}
