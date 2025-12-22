/*
Map to list of functions
*/

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>

CRITICAL_SECTION cs;

#define BEGIN_CRITICAL_SECTION EnterCriticalSection(&cs);
#define END_CRITICAL_SECTION LeaveCriticalSection(&cs);
#define INITIALIZE_CRITICAL_SECTION InitializeCriticalSection(&cs);

#else
#include <pthread.h>

pthread_mutex_t cs = PTHREAD_MUTEX_INITIALIZER;

#define BEGIN_CRITICAL_SECTION pthread_mutex_lock(&cs);
#define END_CRITICAL_SECTION   pthread_mutex_unlock(&cs);
#define INITIALIZE_CRITICAL_SECTION
#endif
#include "eventqueue.h"
#include "events.h"

#define PENDING_CLOSURE_SIZE 1024





#define CLOSURE_MEM_SIZE NUMBER_OF_EVENTS
#define MAX_EVENT_LISTENERS 10

Function_t closures[CLOSURE_MEM_SIZE][MAX_EVENT_LISTENERS];

typedef struct {
    uint8_t pendingClosures[PENDING_CLOSURE_SIZE];
    uint8_t head;
    uint8_t tail;
} Queue_t;

Queue_t eventQueue;
static uint16_t closoureIndex;



void Run_Closures(void) {
    if (eventQueue.head == eventQueue.tail) { return; }
    while (eventQueue.head != eventQueue.tail) {
        uint8_t buffer[sizeof(Node_t)];
        for (int i = 0; i < sizeof(buffer); i++) {
            buffer[i] =  eventQueue.pendingClosures[eventQueue.tail];
            eventQueue.tail = (eventQueue.tail + 1) & (PENDING_CLOSURE_SIZE - 1);
        }
        Node_t *node = (Node_t*)buffer;
        uint8_t data[node->size];
        for (int i = 0; i < node->size; i++) {
            data[i] =  eventQueue.pendingClosures[eventQueue.tail];
            eventQueue.tail = (eventQueue.tail + 1) & (PENDING_CLOSURE_SIZE - 1);
        }
        node->func(node->size, &data);
    }
}

void EventActivate(uint16_t event, const uint16_t size, const void *data) {
    if (event == EVENT_NONE || event >= NUMBER_OF_EVENTS) { return; }

    size_t nodeSize = sizeof(Node_t) + size;

    uint16_t entryIndex = 0;
    Function_t *entry = closures[event];
    while (entry[entryIndex] != NULL && entryIndex < MAX_EVENT_LISTENERS) {
        BEGIN_CRITICAL_SECTION
        size_t used = (eventQueue.head - eventQueue.tail) & (PENDING_CLOSURE_SIZE - 1);
        size_t free = PENDING_CLOSURE_SIZE - used - 1;
        if (nodeSize > free) {
            fprintf(stderr, "Out of memory for closures\n");
            END_CRITICAL_SECTION
            return;
        }
        uint8_t buffer[nodeSize];
        Node_t *node = (Node_t*)buffer;
        node->data = (void*)&buffer[sizeof(Node_t)];
        node->size = size;
        node->func = entry[entryIndex];
        memcpy(node->data, data, size);
        for (int i = 0; i < sizeof(Node_t) + size; i++) {
            eventQueue.pendingClosures[eventQueue.head] = ((uint8_t*)node)[i];
            eventQueue.head = (eventQueue.head + 1) & (PENDING_CLOSURE_SIZE - 1);
        }
        entryIndex++;
        END_CRITICAL_SECTION
    }
}


void EventAddCallback(uint16_t event, Function_t func) {
    if (func == NULL || event >= NUMBER_OF_EVENTS) { return; }

    Function_t *entry = closures[event];
    while (*entry != NULL && *entry != func) {  // Find fresh memory
        entry++;
    }

    if (*entry == func) { return; } // event and func already registered

    if (entry == &closures[event][MAX_EVENT_LISTENERS]) {
        fprintf(stderr, "Out of closure mem");
        return;
    }
    *entry = func;
}

void RemoveClosure(uint16_t event, Function_t func) {
    if (func == NULL || event >= NUMBER_OF_EVENTS) { return; }
    Function_t *entry = closures[event];

    while (*entry != func && *entry != NULL) {
        entry++;
    }
    if (*entry == NULL) { return; } // Closure does not exist in list

    while (*entry != NULL) {    // Shrink array
        *entry = *(entry + 1);
        entry++;
    }
}




void InitEventQueue(void) {
    memset(closures, 0, sizeof(void*) * CLOSURE_MEM_SIZE * MAX_EVENT_LISTENERS);
    INITIALIZE_CRITICAL_SECTION
    closoureIndex = 0;
    eventQueue.head = eventQueue.tail = 0;
}