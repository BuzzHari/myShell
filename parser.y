%{
#include<string.h>
#include<stdio.h>
#include"myShell.h"
int yylex();
void yyerror(char const *s);
%}


%token NEWLINE WORD NOTOKEN
/*Bison basically works by asking flex to get the next token
, which it returns as an object of type "YYSTYPE". By default,
YYSTYPE, is just a typedef of "int".
So, here we need to overide the default typedef to a C union
instead. Unions can hold all of the types of tokens that Flex
could return.*/

%union{
    char *str;
}

%type <str> WORD
%%
command_list: command_line{  
                //printCurrentCommand();
                //EXITS WITH SUCCESS AFTER GOOD PARSE.
                //The thing is any command will be a good pares.
                //my lex and grammer is buggy af...lul.
                YYACCEPT;
            }
            ;

command_line: simple_command NEWLINE{
                //printf("NULL\n");
                insertArguments((char*)NULL);       
            }
            | NEWLINE{
                //printf("NULL\n");
                insertArguments((char*)NULL);       
            }
            ;

simple_command: simple_command words
              | WORD{
                //printf("CMD: %s\n", $1);
                insertArguments($1);
                free($1);
              }
              ;

words: WORD{
        //printf("Args: %s\n", $1);
        insertArguments($1);
        free($1);
     }
%%


void yyerror(char const *s){
    fprintf(stderr, "%s\n", s);
}

int yywrap(void){
    return 1;
}
