#include<unistd.h>
#include<sys/wait.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"parser.tab.h"
#include"myShell.h"



int myShell_cd(char **args);
int myShell_pwd(char **args);
int myShell_exit(char **args);
/*list of builtin_commands supported by myshell*/
char *builtin_commands[]={
    "cd",
    "pwd",
    "help",
    "exit"
};

int (*builtin_cmd_funcs[])(char**) = {
    //&myShell_cd,
    //&myShell_pwd,
    //&myShell_exit
};

int getBuiltinLen(){
    return sizeof(builtin_cmd_funcs)/sizeof(char *);
}


int main(){
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















