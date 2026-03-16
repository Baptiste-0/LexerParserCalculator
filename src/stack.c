#include "stack.h"

struct stack *new_stack_q(struct token *token)
{
    struct stack *new = malloc(sizeof(struct stack));
    new->token = token;
    new->next = NULL;
    return new;
}

struct stack *stack_push(struct stack *s, struct token *token)
{
    struct stack *new = malloc(sizeof(struct stack));
    if (!new)
        return NULL;
    new->token = token;
    new->next = s;
    return new;
}

struct stack *stack_pop(struct stack **s)
{
    if (!*s)
        return NULL;
    struct stack *tmp = *s;
    *s = (*s)->next;
    return tmp;
}

struct token *new_token(int val, enum type type)
{
    struct token *new = malloc(sizeof(struct token));
    new->type = type;
    new->val = val;
    return new;
}

void stack_free_one(struct stack *s)
{
    free(s->token);
    free(s);
}

void stack_destroy(struct stack *s)
{
    while (s)
    {
        free(s->token);
        struct stack *tmp = s;
        s = s->next;
        free(tmp);
    }
}
