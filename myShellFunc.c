#include<unistd.h>
#include<sys/wait.h>
#include"myShell.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>


char currentPathName[PATH_MAX];

/* Function declrations for the in-built commands*/
int myShell_cd(char **args);
int myShell_pwd(char **args);
int myShell_exit(char **args);

/*list of builtin_commands supported by myshell*/
char *builtin_commands[]={
    "cd",
    "pwd",
    "exit"
};


int (*builtin_cmd_funcs[])(char**) = {
    &myShell_cd,
    &myShell_pwd,
    &myShell_exit
};

//for cd.
int myShell_cd(char **args){
    //If the arg to cd is NULL, then go to home.
    if(args[1] == NULL){
        args[1] = getenv("HOME");
    }

    //if the cmd is "cd -", go back to the old directory.
    if(strcmp(args[1],"-") == 0)
        args[1] = getenv("OLDPWD");
    
    printf("%s\n", args[1]);
    
    if(getcwd(currentPathName, sizeof(currentPathName)) == NULL)
        perror("myShell");
        
    //updating the OLDPWD, with the current directory.
    setenv("OLDPWD", currentPathName, 1);
    
    //changing to the new directory. 
    if(chdir(args[1]) != 0){
        perror("myShell");
    }
    return 1;
}

//for pwd.
int myShell_pwd(char **args){
    if(args[1] == NULL){
        if (getcwd(currentPathName, sizeof(currentPathName)) != NULL) {
            printf("%s\n",currentPathName);
        }
        else {
            perror("getcwd() error");
        }
    }
    return 1;
}

//exits.. lol
int myShell_exit(char **args){
  return 0;
}


int getBuiltinLen(){
    return sizeof(builtin_cmd_funcs)/sizeof(char *);
}

void initializeShell(void){
    /* TODO: implement bashrc kinda thing.
     * 
     * atm it just initializes the command structure.
     * and sets up some env vars.
     */
    
    if(setenv("HOME", "/home/hari", 1) != 0)
        perror("myShell");
    if(setenv("PWD", "/home/hari", 1) != 0)
        perror("myShell");
    if(setenv("OLDPWD", "/home/hari", 1) != 0)
        perror("myShell");
    chdir("/home/hari");
    getcwd(currentPathName, sizeof(currentPathName));
}    


void initializeSimpleCommand(){
    
    //Prints the prompt
    getcwd(currentPathName, sizeof(currentPathName));
    printf("%sbuzzShell%s@%s>", "\x1B[32m","\x1B[33m",currentPathName);
    printf("%s","\x1b[37m");
    
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

int executeShellCommand(void){
    int status;
    //checking if the command is an inbuilt.
    int builtinLen = getBuiltinLen();
    for(int i = 0; i < builtinLen; i++){
        if(strcmp(command.arguments[0],builtin_commands[i])==0)
            //run the builtin func
            return (*builtin_cmd_funcs[i])(command.arguments);
    }
    status = execute();
    return status;
}

int execute(){
    pid_t pid, wpid;
    int status;

    pid = fork();
    if(pid == 0){
        //this is the child process, executing the command.
        if(execvp(command.arguments[0], command.arguments) == -1){
            perror("myShell");
            exit(EXIT_FAILURE);
        }
    }
    else if(pid < 0){
        //Error forking
        perror("myShell");
    }
    else{
        //parent process waits til the child is done.
        do{
            /*
             * pid: If > 0, then waits for the child with process id value equal to pid.
             * WUNTRACED: also return if a child has stopped 
             * status: the integer is returned by waitpid,
             *        and can be inspected my macros like WIFEXITED, WIFSIGNALED, etc.
             */
            wpid = waitpid(pid, &status, WUNTRACED);

           /*
            * WIFEXITED: returns true fi the child terminated normally.
            * WIFSIGNALED: returns true if the chidl process was teminated by a signal.
            *
            */
        }while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}
