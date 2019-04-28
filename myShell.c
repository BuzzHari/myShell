#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include<setjmp.h>
#include"parser.tab.h"
#include"myShell.h"
/*static sigjmp_buf env_1;

void sigint_handler_two(int signo){
    siglongjmp(env_1, 42);
}*/

int main(){
    initializeShell();
    myShellLoop(); 
    return 0;
}

void myShellLoop(){
    int parserStatus;
    
    signal(SIGINT, SIG_IGN);
    //signal(SIGINT, sigint_handler_two);
    while(1){
        /*if(sigsetjmp(env_1,1)==42){*/
            /*continue;*/
        /*}*/
        //Initialize a new command, to parse and execute.
        initializeGlobalCommand();
        //scans and parses the command from STDIN, and populates the command structure.
        //returns 0 on success and
        //1 on failure.
        parserStatus = yyparse();
        if(parserStatus == 0)
            executeShellCommand();
        else
            printf("Not a valid shell command\n");
    }
}


















