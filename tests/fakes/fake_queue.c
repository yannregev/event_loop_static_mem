// tests/fakes/fake_queue.c
#include "queue.h"
#include "eventqueue.h"
#include <string.h>

#define QUEUE_CAPACITY (QUEUE_SIZE - 1)

void QueueInit(Queue_t *q) {
    q->head = q->tail = 0;
};

int IsQueueEmpty(Queue_t *q) { return q->head == q->tail; }

void Enqueue(Queue_t *q, uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        q->buf[q->tail] = data[i];
        q->tail = (q->tail + 1) % QUEUE_CAPACITY;
    }
}

uint8_t Dequeue(Queue_t *q, uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        data[i] = q->buf[q->head];
        q->head = (q->head + 1) % QUEUE_CAPACITY;
    }
    return 0;
}

size_t QueueSize(Queue_t *q)       { return (q->head - q->tail) % QUEUE_CAPACITY; }
size_t QueueFreeSpace(Queue_t *q)  { return QUEUE_CAPACITY - QueueSize(q); }