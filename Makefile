# My 1st Makefile
# ter ago 13 21:02:30 -03 2019

CFLAGS=-g -I.

expr: main.o lexer.o parser-sem.o vmachine.o
	$(CC) -o $@ $^

clean:
	$(RM) *.o

mostlyclean: clean
	$(RM) *~

ident:
	ident -bap -ts4 -nut -orig -lexer.c main.c parser-sem_v3.c vmachine.c