#include "queue.h"

#define MAX_QUEUE 2048

struct _Queue {
    void *data[MAX_QUEUE];
    void **front;
    void **reaer;
};

Queue *queue_new();

void queue_free(Queue *q);

Bool queue_isEmpty(const Queue *q);

Status queue_push(Queue *q, void *ele);

void *queue_pop(Queue *q);

void *queue_getFront(const Queue *q);

void *queue_getBack(const Queue *q);

size_t queue_size(const Queue *q);

int queue_print(FILE *fp, const Queue *q, p_queue_ele_print f);