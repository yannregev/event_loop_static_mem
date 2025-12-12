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

#else
#include <pthread.h>

pthread_mutex_t cs = PTHREAD_MUTEX_INITIALIZER;

#define BEGIN_CRITICAL_SECTION pthread_mutex_lock(&cs);
#define END_CRITICAL_SECTION   pthread_mutex_unlock(&cs);
#endif
#include "eventqueue.h"
#include "events.h"

#define PENDING_CLOSURE_SIZE 1024



typedef struct {
    uint16_t event;
    uint16_t size;
    void *data;
}Closure_t;

#define CLOSURE_MEM_SIZE NUMBER_OF_EVENTS
#define MAX_EVENT_LISTENERS 10

Function_t closures[CLOSURE_MEM_SIZE][MAX_EVENT_LISTENERS];
static uint8_t pendingClosures[PENDING_CLOSURE_SIZE];
static uint16_t closoureIndex;



static void EnqueueClosure(const uint16_t event, struct Node_t *node) {
    
}


void Run_Closures(void) {
    if (closoureIndex == 0) { return; }
    while (closoureIndex > 0) {
        struct Node_t *node = (struct Node_t*)pendingClosures;
        uint8_t buffer[sizeof(struct Node_t) + node->size];
        size_t nodeSize = sizeof(struct Node_t) + node->size;
        memcpy(buffer, node, nodeSize);
        size_t remaining = closoureIndex - nodeSize;
        if (remaining > 0) {
            BEGIN_CRITICAL_SECTION
            memmove(pendingClosures, pendingClosures + nodeSize, remaining);
            END_CRITICAL_SECTION
        }
        closoureIndex -= nodeSize;
        struct Node_t *n = (struct Node_t *)buffer;
        n->func(n->size, &n->data);
    }
}

void ActivateClosure(uint16_t event, const uint16_t size, const void *data) {
    if (event == EVENT_NONE || event >= NUMBER_OF_EVENTS) { return; }

    size_t nodeSize = sizeof(struct Node_t) + size;

    Function_t *entry = closures[event];
    
    int i = 0;
    while (entry[i] != NULL && i < MAX_EVENT_LISTENERS) {
        if (closoureIndex + sizeof(struct Node_t) + size > PENDING_CLOSURE_SIZE) {
            fprintf(stderr, "Out of memory for closures\n");
            return;
        }
        BEGIN_CRITICAL_SECTION
        struct Node_t *node = (struct Node_t*)(pendingClosures + closoureIndex);
        node->func = entry[i];
        node->size = size;
        memcpy(&node->data, data, size);
        closoureIndex+= nodeSize;
        i++;
        END_CRITICAL_SECTION
    }
}


void AddClosure(uint16_t event, Function_t func) {
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
    memset(pendingClosures, 0, sizeof(void*) * CLOSURE_MEM_SIZE);
    InitializeCriticalSection(&cs);
    closoureIndex = 0;
}