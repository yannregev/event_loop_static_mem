#ifndef INCLUDES_H
#define INCLUDES_H

#include <inttypes.h>
#include <stdlib.h>

typedef const void (*Function_t)(uint16_t, const void *);

struct Node_t {
    Function_t func;
    uint16_t size;
    void *data;
};

typedef struct {
    struct Node_t *first;
    struct Node_t *last;
}List_t;



void Enqueue(struct Node_t *node, List_t *list);
struct Node_t* Dequeue(List_t *list);
void RemoveNode(struct Node_t *node, List_t *list);

#endif //INCLUDES_H