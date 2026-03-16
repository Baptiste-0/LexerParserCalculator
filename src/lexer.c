#include "lexer.h"

int is_correct(char *str, size_t j)
{
    char valid[9] = "+-*/%^()";
    for (int i = 0; i < 9; i++)
    {
        if (str[j] == valid[i])
            return 0;
    }
    if (isspace(str[j]))
        return -1;
    return 1;
}

struct token *create_token(char *str, size_t i)
{
    switch (str[i]) // apres is_correct donc tout doit passer
    {
    case '+': // dans lexer refaire une verif apres pour unary
        return new_token(str[i], ADD);
    case '-':
        return new_token(str[i], SUB);
    case '*':
        return new_token(str[i], MUL);
    case '/':
        return new_token(str[i], DIV);
    case '%':
        return new_token(str[i], MOD);
    case '^':
        return new_token(str[i], POW);
    case '(':
        return new_token(str[i], PARANTHESIS_O);
    default:
        return new_token(str[i], PARANTHESIS_C);
    }
}

struct queue *lexer_rpn(char *str)
{
    struct queue *queue = queue_init();
    size_t i = 0;
    int val = 0;
    while (str[i] != 0)
    {
        if (str[i] - 0 >= '0' && str[i] - '0' <= 9) // gere les chiffres
        {
            val = 0;
            while (str[i] - '0' >= 0
                   && str[i] - '0' <= 9) // str[i] != 0 pas utile, 0 - '0' < 0
            {
                val *= 10;
                val += str[i] - '0';
                i++;
            }
            i--;
            struct token *new = new_token(val, INT);
            queue_push(queue, new);
        }
        else if (is_correct(str, i) == 0) // gere le reste
        {
            struct token *new = create_token(str, i);
            queue_push(queue, new);
        }
        else if (is_correct(str, i) != -1)
        {
            queue_destroy(queue);
            return NULL;
        }
        i++;
    }
    return queue;
}

struct queue *lexer_base(char *str)
{
    struct queue *queue = queue_init();
    size_t i = 0;
    int val = 0;
    int op = 1;
    int unary = 0;
    int last_pc = 0;
    struct token *t_unary = new_token('+', UNARY);
    while (str[i] != 0)
    {
        if (str[i] - 0 >= '0' && str[i] - '0' <= 9) // gere les chiffres
        {
            val = 0;
            while (str[i] - '0' >= 0
                   && str[i] - '0' <= 9) // str[i] != 0 pas utile, 0 - '0' < 0
            {
                val *= 10;
                val += str[i] - '0';
                i++;
            }
            i--;
            if (unary == 1)
            {
                if (t_unary->val == '-')
                    val *= -1;
            }
            struct token *new = new_token(val, INT);
            queue_push(queue, new);
            op = 0;
            unary = 0;
        }
        else if (is_correct(str, i) == 0) // gere le reste
        {
            if (str[i] == '(')
            {
                if (t_unary->val == '-')
                {
                    struct token *tmp_unary = new_token('+', ADD);
                    tmp_unary->val = '-';
                    tmp_unary->type = SUB;
                    queue_push(queue, tmp_unary);
                    unary = 0;
                }
            }
            else if (op == 1 && last_pc == 0)
            {
                if (str[i] == '+' || str[i] == '-')
                {
                    // if(unary == 1)
                    //{
                    if (t_unary->val == str[i])
                        t_unary->val = '+';
                    else
                        t_unary->val = '-';
                    //	}
                    //	else
                    unary = 1;
                }
                else
                {
                    free(t_unary);
                    queue_destroy(queue);
                    return NULL;
                }
            }
            if (unary == 0)
            {
                struct token *new = create_token(str, i);
                queue_push(queue, new);
            }
            op = 1;
            last_pc = 0;
            if (str[i] == ')')
                last_pc = 1;
        }
        else if (is_correct(str, i) != -1)
        {
            free(t_unary);
            queue_destroy(queue);
            return NULL;
        }
        i++;
    }
    free(t_unary);
    return queue;
}

struct queue *shunting_yard(struct queue *lex)
{
    struct queue *res = queue_init();
    struct stack *op = NULL;
    while (lex->head)
    {
        struct stack *s = lex->head;
        lex->head = lex->head->next;
        if (s->token->type == INT)
        {
            queue_push(res, s->token);
            free(s);
        }
        else if (s->token->type == PARANTHESIS_O)
        {
            op = stack_push(op, s->token);
            free(s);
        }
        else if (s->token->type == PARANTHESIS_C)
        {
            free(s->token);
            free(s);
            while (op && op->token->type != PARANTHESIS_O)
            {
                struct stack *tmp = stack_pop(&op);
                queue_push(res, tmp->token);
                free(tmp);
            }
            if (!op)
            {
                queue_destroy(lex);
                queue_destroy(res);
                return NULL;
            }
            else
            {
                struct stack *tmp = stack_pop(&op);
                free(tmp->token);
                free(tmp);
            }
        }
        else
        {
            while (op && op->token->type != PARANTHESIS_O
                   && op->token->type % 6 >= s->token->type % 6)
            {
                struct stack *tmp = stack_pop(&op);
                queue_push(res, tmp->token);
                free(tmp);
            }
            op = stack_push(op, s->token);
            free(s);
        }
    }
    while (op)
    {
        struct stack *tmp = stack_pop(&op);
        queue_push(res, tmp->token);
        free(tmp);
    }
    queue_destroy(lex);
    return res;
}
