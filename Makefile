OBJS = lex.yy.o C.tab.o symbol_table.o nodes.o main.o interpret.o value.o frame.o gentac.o
SRCS = lex.yy.c C.tab.c symbol_table.c nodes.c main.c interpret.c value.c frame.c gentac.c
CC = gcc

all:	mycc

clean:
	rm ${OBJS}

mycc:	${OBJS}
	${CC} -g -o mycc ${OBJS} 

lex.yy.c: C.flex
	flex C.flex

C.tab.c:	C.y
	bison -d -t -v C.y

.c.o:
	${CC} -g -c $*.c

depend:	
	${CC} -M $(SRCS) > .deps
	cat Makefile .deps > makefile

dist:	symbol_table.c nodes.c main.c Makefile C.flex C.y nodes.h token.h
	tar cvfz mycc.tgz symbol_table.c nodes.c main.c Makefile C.flex C.y \
		nodes.h token.h interpret.h interpret.c value.h value.c gentac.h gentac.c frame.o frame.c
