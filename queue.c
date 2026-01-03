#include "queue.h"

void Enqueue(Queue_t *queue, uint8_t data) {
    queue->buf[queue->head] = data;
    queue->head = (queue->head + 1) & (QEUEUE_SIZE - 1);
}

uint8_t Dequeue(Queue_t *queue) {
    uint8_t data =  queue->buf[queue->tail];
    queue->tail = (queue->tail + 1) & (QEUEUE_SIZE - 1);
    return data;
}

uint8_t IsQueueEmpty(Queue_t *queue) {
    return queue->head == queue->tail;
}

uint16_t QueueSize(Queue_t *queue) {
    return (queue->head - queue->tail) & (QEUEUE_SIZE - 1);
}

uint16_t QueueFreeSpace(Queue_t *queue) {
    return QEUEUE_SIZE - QueueSize(queue) - 1;
}

void QueueInit(Queue_t *queue) {
    queue->head = queue->tail = 0;
}