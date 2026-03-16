#define _POSIX_C_SOURCE 200809L

#include <stdio.h>

#include "calcul.h"
#include "lexer.h"
#include "queue.h"
#include "stack.h"

char *readline(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    nread = getline(&line, &len, stdin);
    if (nread == -1)
    {
        free(line);
        return NULL;
    }
    // Remove trailing newline if present
    if (nread > 0 && line[nread - 1] == '\n')
        line[nread - 1] = '\0';
    return line;
}

int main(int argc, char *argv[])
{
    int rpn = 0;
    if (argc > 1)
    {
        if (argc > 2)
            return 4;
        if (strcmp(argv[1], "-rpn"))
            return 4;
        rpn = 1;
    }

    char *str = readline();
    if (!str)
        return 0;
    struct queue *lexer = NULL;
    if (rpn)
    {
        lexer = lexer_rpn(str);
        free(str);
        if (!lexer)
            return 1;
    }
    else
    {
        lexer = lexer_base(str);
        free(str);
        if (!lexer)
            return 1;
        lexer = shunting_yard(lexer);
        if (!lexer)
            return 2;
    }

    int res = 0;
    int cal = calcul_rpn(lexer, &res);

    if (cal == 2)
    {
        queue_destroy(lexer);
        return 2;
    }
    else if (cal == 3)
    {
        queue_destroy(lexer);
        return 3;
    }

    queue_destroy(lexer);

    printf("%d\n", res);
    return 0;
}
