#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "common.h"
#include "switch.h"
#include "queue.h"

struct queue_t *queue_init(int size)
{
    struct queue_t *q = (struct queue_t *)malloc(sizeof(struct queue_t));

    if (!q) {
        SWITCH_SIM_ERR("Failed to allocate memory for queue\n");
        return NULL;
    }

    q->size = size;
    q->head = 0;
    q->tail = 0;

    return q;
}

void queue_free(struct queue_t *q)
{
    SWITCH_SIM_FREE_IF_NOT_NULL(q);
}

int queue_enqueue(struct queue_t *q)
{
    if ((q->tail + 1) % q->size == q->head) {
        return -1;
    }

    q->tail = (q->tail + 1) % q->size;

    return 0;
}

int queue_dequeue(struct queue_t *q)
{
    if (q->head == q->tail) {
        return -1;
    }

    q->head = (q->head + 1) % q->size;

    return 0;
}

bool queue_is_empty(struct queue_t *q)
{
    return q->head == q->tail;
}

bool queue_is_full(struct queue_t *q)
{
    return (q->tail + 1) % q->size == q->head;
}   