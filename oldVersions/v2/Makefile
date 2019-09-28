# My 1st Makefile
# ter ago 13 21:02:30 -03 2019

CFLAGS=-g -I.

expr: main.o lexer.o parser.o
	$(CC) -o $@ $^

clean:
	$(RM) *.o

mostlyclean: clean
	$(RM) *~


