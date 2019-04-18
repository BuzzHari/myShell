#include"myShell.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

void initializeSimpleCommand(){
    SIMPLE_COMMAND.arguments=(char**)malloc(sizeof(char*)*TOK_BUFSIZE);
    if(!SIMPLE_COMMAND.arguments){
        printf("Malloc error\n");
        exit(EXIT_FAILURE);
    }
    
    SIMPLE_COMMAND.numberOfArguments = 0;
    SIMPLE_COMMAND.bufsize = TOK_BUFSIZE;
}

void insertArguments(char *args){
     
    //to take care of overflow.
    if(SIMPLE_COMMAND.numberOfArguments >= SIMPLE_COMMAND.bufsize){
        SIMPLE_COMMAND.bufsize+=SIMPLE_COMMAND.bufsize;
        SIMPLE_COMMAND.arguments = realloc(SIMPLE_COMMAND.arguments, SIMPLE_COMMAND.bufsize*sizeof(char*));
    }
    
    /*If the args from the parser is not null, append it in the list
     * of args.
     * A null, means end of the command so, if null, insert NULL in the end of the
     * args list.*/
    if(args!=NULL)
        SIMPLE_COMMAND.arguments[(SIMPLE_COMMAND.numberOfArguments)++] = strdup(args);
    else 
        SIMPLE_COMMAND.arguments[(SIMPLE_COMMAND.numberOfArguments)++] = NULL;
}

void printCurrentCommand(){
    for(int i=0; SIMPLE_COMMAND.arguments[i] != NULL; i++)
        printf("%s ", SIMPLE_COMMAND.arguments[i]);
    printf("\n");
}


