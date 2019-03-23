%{
#include"myshell.h"
#include<string.h>
#include<stdio.h>
int yylex();
void yyerror(char const *s);
%}


%token NEWLINE WORD

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
command_list:/*empty*/
            | command_list command_line{
                printf("myShell > ");
            }
            ;

command_line:
            cmd_and_args NEWLINE 
            | NEWLINE

cmd_and_args:
             WORD arg_list {
                SIMPLE_COMMAND.insertArguments($1);
                SIMPLE_COMMAND.printCommand();
                free($1);
             }
            ;

arg_list:/*empty*/
        | arg_list WORD{
            printf("Args: %s\n", $2);
        }
        ;
%%


void yyerror(char const *s){
    fprintf(stderr, "%s\n", s);
}
