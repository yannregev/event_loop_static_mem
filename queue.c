#include "queue.h"

uint8_t Enqueue(Queue_t *queue, uint8_t *data, uint16_t size) {
    if (size == 0 ) {return 0;}
    for (uint16_t i = 0; i < size; i++) {
        if (queue->head == queue->tail && !queue->isEmpty) {
            fprintf(stderr, "Queue overflow: Attempted to enqueue to a full queue.\n");
            return -1;
        }
        queue->buf[queue->head] = data[i];
        queue->head = (queue->head + 1) & (QEUEUE_SIZE - 1);
    }
    queue->isEmpty = 0;
    return 0;
}

uint8_t Dequeue(Queue_t *queue, void* buf, uint16_t size) {
    for (uint16_t i = 0; i < size; i++) {
        if (queue->isEmpty == 1) {
            fprintf(stderr, "Queue underflow: Attempted to dequeue from an empty queue.\n");
            return -1;
        }
        ((uint8_t*)buf)[i] = queue->buf[queue->tail];
        queue->tail = (queue->tail + 1) & (QEUEUE_SIZE - 1);
        queue->isEmpty = queue->head == queue->tail;
    }

    return 0;
}

uint8_t IsQueueEmpty(Queue_t *queue) {
    return queue->isEmpty;
}

uint16_t QueueSize(Queue_t *queue) {
    return (queue->head - queue->tail) & (QEUEUE_SIZE - 1);
}

uint16_t QueueFreeSpace(Queue_t *queue) {
    return QEUEUE_SIZE - QueueSize(queue) - 1;
}

void QueueInit(Queue_t *queue) {
    queue->head = queue->tail = 0;
    queue->isEmpty = 1;
}