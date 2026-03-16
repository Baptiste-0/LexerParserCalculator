#include "calcul.h"

int my_pow(int a, int b)
{
    if (b == 0)
        return 1;
    return a * my_pow(a, b - 1);
}

int operation(struct token *op, struct token *first, struct token *second,
              int *res)
{
    switch (op->type)
    {
    case ADD:
        *res = second->val + first->val;
        return 1;
    case SUB:
        *res = second->val - first->val;
        return 1;
    case MUL:
        *res = second->val * first->val;
        return 1;
    case MOD:
        if (first->val == 0)
            return 0;
        *res = second->val % first->val;
        return 1;
    case DIV:
        if (first->val == 0)
            return 0;
        *res = second->val / first->val;
        return 1;
    case POW:
        if (first->val < 0)
            return 0;
        *res = my_pow(second->val, first->val);
        return 1;
    default:
        return 0;
    }
}

int calcul_rpn(struct queue *queue, int *res)
{
    struct stack *stack = NULL;
    struct stack *i = queue->head;
    while (i)
    {
        if (i->token->type == INT)
        {
            stack = stack_push(stack, new_token(i->token->val, i->token->type));
        }
        else
        {
            if (!stack)
                return 2;
            int res_op = 0;
            struct stack *pop_1 = stack_pop(&stack);
            if (!stack)
            {
                if (i->token->type == ADD || i->token->type == SUB)
                {
                    struct token zero = { .type = INT, .val = 0 };
                    if (!operation(i->token, pop_1->token, &zero, &res_op))
                    {
                        stack_free_one(pop_1);
                        stack_destroy(stack);
                        return 3;
                    }
                }
                else
                {
                    stack_free_one(pop_1);
                    stack_destroy(stack);
                    return 2;
                }
            }
            else
            {
                struct stack *pop_2 = stack_pop(&stack);
                if (!pop_2)
                {
                    stack_free_one(pop_1);
                    stack_destroy(stack);
                    return 2;
                }
                if (!operation(i->token, pop_1->token, pop_2->token, &res_op))
                {
                    stack_free_one(pop_2);
                    stack_free_one(pop_1);
                    stack_destroy(stack);
                    return 3;
                }
                stack_free_one(pop_2);
                /*if (!res)
                {
                    stack_free_one(pop_1);
                    stack_destroy(stack);
                    return 0;
                }*/
            }
            pop_1->token->val = res_op;
            stack = stack_push(stack, pop_1->token);
            free(pop_1);
        }
        i = i->next;
    }
    if (!stack)
        return 2;
    if (stack->next)
    {
        stack_destroy(stack);
        return 2;
    }
    memcpy(res, &stack->token->val, sizeof(int));
    stack_destroy(stack);
    return 1;
}
