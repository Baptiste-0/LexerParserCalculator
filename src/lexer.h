#ifndef LEXER_H
#define LEXER_H

#include <ctype.h>

#include "queue.h"
#include "stack.h"

int is_correct(char *str, size_t j);
struct token *create_token(char *str, size_t i);
struct queue *lexer_rpn(char *str);
struct queue *lexer_base(char *str);
struct queue *shunting_yard(struct queue *lex);

#endif /* ! LEXER_H */
