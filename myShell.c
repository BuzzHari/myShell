#include<stdio.h>
#include<string.h>
#include"parser.tab.h"
#include"myShell.h"


int main(){
    initializeSimpleCommand();
    printf("myShell > ");
    yyparse();
    printf("Parseing done\n");
}
