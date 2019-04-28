%{
#include<string.h>
#include<stdio.h>
#include"myShell.h"
int yylex();
void yyerror(char const *s);
%}


%define parse.error verbose

%token WORD
%token NEWLINE
%token IO_NUMBER
%token GREAT LESS PIPE 
%token CTRL
/*Bison basically works by asking flex to get the next token
, which it returns as an object of type "YYSTYPE". By default,
YYSTYPE, is just a typedef of "int".
So, here we need to overide the default typedef to a C union
instead. Unions can hold all of the types of tokens that Flex
could return.*/

%union{
    char *str;
}

%type <str> WORD CTRL

%start command_list
%%
command_list: command_line{  
                //printCurrentCommand();
                //printf("success\n");
                //EXITS WITH SUCCESS AFTER GOOD PARSE.
                YYACCEPT;
            }
            ;

command_line: command NEWLINE{
                //printf("NULL\n");
                insertArguments((char*)NULL);       
            }
            | NEWLINE{
                //printf("NULL\n");
                insertArguments((char*)NULL);       
            }
            | error NEWLINE {
                yyerrok;
            }
            | CTRL NEWLINE{
                printf("Pressed ctrl-c\n");
                YYABORT;
                yyerrok;
                //yyerror($1);
            }
            ;
command: simple_command
       | command PIPE{
            //printf("PIPE\n");
            insertArguments("PIPE");
       } simple_command
       ;

simple_command: cmd_prefix cmd_word cmd_suffix
              | cmd_prefix cmd_word
              | cmd_prefix
              | cmd_name cmd_suffix
              | cmd_name
              ;

cmd_name: WORD{
            //printf("CMD_NAME: %s\n", $1);
            insertArguments($1);
            free($1);
        }
        ;

cmd_word: WORD{
            //printf("CMD_WORD: %s\n", $1);
            insertArguments($1);
            free($1);
        }
        ;

cmd_prefix: cmd_prefix io_file
          | io_file
          ;

cmd_suffix: cmd_suffix io_file
          | io_file
          | cmd_suffix cmd_word
          | cmd_word
          ;

io_file: LESS{
            //printf("Redirection: <\n");
            insertArguments("LESS");
       } cmd_word
       | GREAT{
            //printf("Redirectrion: >\n");
            insertArguments("GREAT");
       } cmd_word
       ;
%%


void yyerror(char const *s){
    printf("in here\n");
    fprintf(stderr, "%s\n", s);
}

int yywrap(void){
    return 1; }

/*int main(){
    printf("Starting to parse!\n");
    while(1){
        if(!yyparse())
            printf("Success\n");
        else{
            printf("FAIL\n");
            break;
        }
    }
    return 0;
}*/
