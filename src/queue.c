#include "queue.h"

struct queue *queue_init(void)
{
    struct queue *res = malloc(sizeof(struct queue));
    res->head = NULL;
    res->tail = NULL;
    res->size = 0;
    return res;
}

void queue_push(struct queue *queue, struct token *elt)
{
    struct stack *st = queue->tail;
    struct stack *new = new_stack_q(elt);
    if (st)
    {
        st->next = new;
        queue->tail = new;
    }
    else
    {
        queue->head = new;
        queue->tail = new;
    }
    queue->size++;
}

void queue_pop(struct queue *queue)
{
    struct stack *h = queue->head;
    if (!h)
        return;
    struct stack *tmp = h->next;
    free(h->token);
    free(h);
    queue->head = tmp;
    if (!tmp)
        queue->tail = NULL;
    queue->size--;
}

void queue_clear(struct queue *queue)
{
    if (!queue->head)
        return;
    while (queue->head)
        queue_pop(queue);
    queue->size = 0;
}

void queue_destroy(struct queue *queue)
{
    queue_clear(queue);
    free(queue);
}
