// tests/fakes/fake_queue.c
#include "queue.h"
#include "eventqueue.h"
#include <string.h>

#define QUEUE_CAPACITY (QUEUE_SIZE - 1)

void QueueInit(Queue_t *q) {
    q->head = q->tail = 0;
};

uint8_t IsQueueEmpty(Queue_t *q) { return q->head == q->tail; }

uint8_t Enqueue(Queue_t *q, uint8_t* data, uint16_t size) {
    for (size_t i = 0; i < size; i++) {
        q->buf[q->tail] = data[i];
        q->tail = (q->tail + 1) % QUEUE_CAPACITY;
    }
    return 0;
}

uint8_t Dequeue(Queue_t *q, void* data, uint16_t size) {
    for (uint16_t i = 0; i < size; i++) {
        ((uint8_t*)data)[i] = q->buf[q->head];
        q->head = (q->head + 1) % QUEUE_CAPACITY;
    }
    return 0;
}

uint16_t QueueSize(Queue_t *q)       { return (q->head - q->tail) % QUEUE_CAPACITY; }
uint16_t QueueFreeSpace(Queue_t *q)  { return QUEUE_CAPACITY - QueueSize(q); }