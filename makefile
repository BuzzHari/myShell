CC = gcc
CFLAGS= -g -Wall


myshell: myShell.o myShellFunc.o parser.tab.o lex.yy.o
	$(CC) $(CFLAGS) myShell.o myShellFunc.o parser.tab.o lex.yy.o -lfl -o myshell

myShell.o: myShell.c myShell.h parser.tab.h
myShellFunc.o: myShellFunc.c myShell.h

%.tab.c %.tab.h: %.y 
	bison -d $<
lex.yy.c: shell.l
	flex shell.l

.PHONY: clean
clean:
	rm *.o

