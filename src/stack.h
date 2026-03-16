#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include <stdlib.h>

// priority order
enum type // mod 6 for priority order
{
    PARANTHESIS_O = 11,
    PARANTHESIS_C = 5,
    UNARY = 4,
    POW = 3,
    DIV = 14,
    MUL = 8,
    MOD = 2,
    SUB = 7,
    ADD = 1,
    INT = 0
};
// way to stock each data
struct token // si qqln lis ca, je me suis complique de fou,
             // le token aurait du faire stack
{
    enum type type;
    int val;
};
// stock every data
struct stack
{
    struct token *token;
    struct stack *next;
};

struct stack *new_stack_q(struct token *token);
struct stack *stack_push(struct stack *s, struct token *e);
struct stack *stack_pop(struct stack **s);
struct token *new_token(int val, enum type type);
void stack_free_one(struct stack *s);
void stack_destroy(struct stack *s);

#endif /* ! STACK_H */
