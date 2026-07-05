#include "queue.h"

#define QUEUE_CAPACITY (QUEUE_SIZE - 1)

uint8_t Enqueue(Queue_t *queue, uint8_t *data, uint16_t size) {
    if (size == 0 ) {return 0;}
    for (uint16_t i = 0; i < size; i++) {
        if (QueueFreeSpace(queue) == 0) {
            return -1;
        }
        queue->buf[queue->head] = data[i];
        queue->head = (queue->head + 1) & QUEUE_CAPACITY;
    }
    return 0;
}

uint8_t Dequeue(Queue_t *queue, void* buf, uint16_t size) {
    for (uint16_t i = 0; i < size; i++) {
        if (IsQueueEmpty(queue) == 1) {
            return -1;
        }
        ((uint8_t*)buf)[i] = queue->buf[queue->tail];
        queue->tail = (queue->tail + 1) & QUEUE_CAPACITY;
    }

    return 0;
}

uint8_t IsQueueEmpty(Queue_t *queue) {
    return queue->head == queue->tail;
}

uint16_t QueueSize(Queue_t *queue) {
    return (queue->head - queue->tail) & QUEUE_CAPACITY;
}

uint16_t QueueFreeSpace(Queue_t *queue) {
    return QUEUE_CAPACITY - QueueSize(queue);
}

void QueueInit(Queue_t *queue) {
    queue->head = queue->tail = 0;
}