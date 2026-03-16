CC=gcc
CFLAGS=-std=c99 -pedantic -Werror -Wall -Wextra -Wvla
SRCS=src/stack.c src/queue.c src/lexer.c src/calcul.c src/main.c
OBJS=${SRCS:.c=.o}

all: ${OBJS}
	${CC} -o evalexpr ${OBJS}

check:
	cd tests && ./tests.sh

clean:
	${RM} evalexpr ${OBJS}

.PHONY: clean
