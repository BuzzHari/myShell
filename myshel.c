#include<stdio.h>
#include"parser.tab.h"


struct SimpleCommand{
    int _numberOfAvailableArguments;
    int _numberOfArguments;
    char **_arguments;
};


int main(){
    printf("myShell > ");
    yyparse();
}
