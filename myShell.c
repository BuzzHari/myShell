#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"parser.tab.h"
#include"myShell.h"


int main(){
    initializeShell();
    myShellLoop(); 
    return 0;
}

void myShellLoop(){
    int shellStatus = 1;
    int parserStatus;
    do{
        
        //Initialize a new command, to parse and execute.
        initializeSimpleCommand();
        //scans and parses the command from STDIN, and populates the command structure.
        //returns 0 on success and
        //1 on failure.
        parserStatus = yyparse();
        if(parserStatus == 0)
            shellStatus = executeShellCommand();
        else
            printf("Not a valid shell command\n");
    }while(shellStatus);
}


















