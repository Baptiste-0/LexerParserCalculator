#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

#include "stack.h"

struct queue
{
    struct stack *head;
    struct stack *tail;
    size_t size;
};

struct queue *queue_init(void);
void queue_push(struct queue *queue, struct token *elt);
void queue_pop(struct queue *queue);
void queue_clear(struct queue *queue);
void queue_destroy(struct queue *queue);

#endif /* ! QUEUE_H */
