#ifndef __QUEUE_H__
#define __QUEUE_H__

struct queue_t {
    int head;
    int tail;
    int size;
};

struct queue_t *queue_init(int size);

void queue_free(struct queue_t *q);

int queue_enqueue(struct queue_t *q);

int queue_dequeue(struct queue_t *q);

bool queue_is_empty(struct queue_t *q);

bool queue_is_full(struct queue_t *q);

#endif /* __QUEUE_H__ */