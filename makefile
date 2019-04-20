CC = gcc
CFLAGS= -g -Wall


myshell: myShell.o myShellFunc.o parser.tab.o lex.yy.o
	$(CC) $(CFLAGS) myShell.o myShellFunc.o parser.tab.o lex.yy.o -lfl -o myshell

myShell.o: myShell.c myShell.h parser.tab.h
myShellFunc.o: myShellFunc.c myShell.h

parser.tab.c parser.tab.h: parser.y 
	bison -d parser.y
lex.yy.c: shell.l
	flex shell.l

.PHONY: clean
clean:
	rm *.o

