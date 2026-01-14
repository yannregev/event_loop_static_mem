#ifndef QUEUE_H
#define QUEUE_H

#include <inttypes.h>

#define QEUEUE_SIZE 256
typedef struct {
    uint16_t head;
    uint16_t tail;
    uint8_t buf[QEUEUE_SIZE];
} Queue_t;
void Enqueue(Queue_t *queue, uint8_t data);
uint8_t Dequeue(Queue_t *queue);
uint8_t IsQueueEmpty(Queue_t *queue);
uint16_t QueueSize(Queue_t *queue);
void QueueInit(Queue_t *queue);
uint16_t QueueFreeSpace(Queue_t *queue);
#endif //QUEUE_H