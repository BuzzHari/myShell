#include"myShell.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

void initializeSimpleCommand(){
    command.arguments=(char**)malloc(sizeof(char*)*TOK_BUFSIZE);
    if(!command.arguments){
        printf("Malloc error\n");
        exit(EXIT_FAILURE);
    }
    
    command.numberOfArguments = 0;
    command.bufsize = TOK_BUFSIZE;
}

void insertArguments(char *args){
     
    //to take care of overflow.
    if(command.numberOfArguments >= command.bufsize){
        command.bufsize+=command.bufsize;
        command.arguments = realloc(command.arguments, command.bufsize*sizeof(char*));
    }
    
    /*If the args from the parser is not null, append it in the list
     * of args.
     * A null, means end of the command so, if null, insert NULL in the end of the
     * args list.*/
    if(args!=NULL)
        command.arguments[(command.numberOfArguments)++] = strdup(args);
    else 
        command.arguments[(command.numberOfArguments)++] = NULL;
}

void printCurrentCommand(){
    for(int i=0; command.arguments[i] != NULL; i++)
        printf("%s ", command.arguments[i]);
    printf("\n");
}


