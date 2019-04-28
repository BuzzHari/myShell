#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#include"myShell.h"

char currentPathName[PATH_MAX];
gCommand globalCommand; 

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
    printf("exit\n");
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


void initializeGlobalCommand(){
    
    //Prints the prompt..put this in main();
    getcwd(currentPathName, sizeof(currentPathName));
    printf("%sbuzzShell%s@%s>", "\x1B[32m","\x1B[33m",currentPathName);
    printf("%s","\x1b[37m");

    globalCommand.numberOfSimpleCommands = 0; 
    globalCommand.background = 0;
    globalCommand.outfile = NULL;
    globalCommand.infile = NULL;
    globalCommand.errfile = NULL;
    
    //Intially just allocating mem for one command.
    //If there's pipe, will reallocate to accomadate accordingly.
    globalCommand.sCommands = (sCommand*) malloc(sizeof(sCommand)*1);
    if(!globalCommand.sCommands){
        printf("Malloc error\n");
        exit(EXIT_FAILURE);
    }
    globalCommand.sCommands[0].numberOfArguments = 0; 
    globalCommand.sCommands[0].bufsize = TOK_BUFSIZE;
    globalCommand.sCommands[0].arguments = (char**) malloc(sizeof(char*) * TOK_BUFSIZE);
    if(!globalCommand.sCommands[0].arguments){
        printf("Malloc error\n");
        exit(EXIT_FAILURE);
    }
}

void freeGlobalCommand(){
    for(int i=0; i<=globalCommand.numberOfSimpleCommands; i++)
        free(globalCommand.sCommands[i].arguments);
    free(globalCommand.sCommands);
    free(globalCommand.outfile);
    free(globalCommand.infile);
    free(globalCommand.errfile);
}

void insertArguments(char *args){
      
    /*  If from parser, we get the IO redirection symbols, 
     *  then these flags, are set, so the next token from the 
     *  parser, will be considered as a file name and 
     *  stored properly.
     */
    static int INPUTREDIR_FLAG = 0;
    static int OUTPUTREDIR_FLAG = 0;
    
    //If pipe, then increase the number of sCommands by 1, and intialize it.
    if(args != NULL && strcmp(args, "PIPE") == 0){
        globalCommand.numberOfSimpleCommands+=1;
        globalCommand.sCommands = realloc(globalCommand.sCommands, sizeof(sCommand)*(globalCommand.numberOfSimpleCommands+1));
        
        globalCommand.sCommands[globalCommand.numberOfSimpleCommands].numberOfArguments = 0; 
        globalCommand.sCommands[globalCommand.numberOfSimpleCommands].bufsize = TOK_BUFSIZE;
        globalCommand.sCommands[globalCommand.numberOfSimpleCommands].arguments = (char**) malloc(sizeof(char*) * TOK_BUFSIZE);
    }
    //if LESS, that means redirect the input from the file given,
    //this sets the flag INPUTREDIR_FLAG, so the next token can
    //get stored in globalCommand.infile.
    else if(args != NULL && strcmp(args, "LESS") == 0)
        INPUTREDIR_FLAG = 1;
    //if GREAT, that means redirect the output from the file given,
    //this sets the flag OUTPUTREDIR_FLAG, so the next token can
    //get stored in globalCommand.outfile.
    else if(args != NULL && strcmp(args, "GREAT") == 0)
        OUTPUTREDIR_FLAG = 1;
    //the token is part of the command, and stores it in gloabalCommands.sCommnads[].arguments.
    else{ 
        int noOfSCmd=globalCommand.numberOfSimpleCommands;
        if(args != NULL){
            if(INPUTREDIR_FLAG){
                //storing the input file
                globalCommand.infile = strdup(args);
            }
            if(OUTPUTREDIR_FLAG){
                //storing the output file
                globalCommand.outfile = strdup(args);
            }
            if(!OUTPUTREDIR_FLAG && !INPUTREDIR_FLAG){
                // to take care of overflow of arguments.
                if(globalCommand.sCommands[noOfSCmd].numberOfArguments >= globalCommand.sCommands[noOfSCmd].bufsize){
                    globalCommand.sCommands[noOfSCmd].bufsize += TOK_BUFSIZE;
                    globalCommand.sCommands[noOfSCmd].arguments = realloc(globalCommand.sCommands[noOfSCmd].arguments, sizeof(char*) * globalCommand.sCommands[noOfSCmd].bufsize); 
                }
                //storing the arguments
                globalCommand.sCommands[noOfSCmd].arguments[(globalCommand.sCommands[noOfSCmd].numberOfArguments)++] = strdup(args);
            }
            //setting these flags to 0.
            INPUTREDIR_FLAG = 0;
            OUTPUTREDIR_FLAG = 0;
        }
        else{
            //ending the command with NULL, for execvp().
            globalCommand.sCommands[noOfSCmd].arguments[(globalCommand.sCommands[noOfSCmd].numberOfArguments)++] = NULL;
        }
    }
}

void printCurrentCommand(){
    for(int i=0; i<=globalCommand.numberOfSimpleCommands; i++){
        printf("Command %d:\n", i+1);
        for(int j=0; globalCommand.sCommands[i].arguments[j] != NULL; j++)
            printf("%s ", globalCommand.sCommands[i].arguments[j]);
        printf("\n");
    }
    printf("OUTFILE %s\n", globalCommand.outfile);
    printf("INFILE %s\n", globalCommand.infile);
}

int executeShellCommand(void){
    int status;
    //checking if the command is an inbuilt.
    int builtinLen = getBuiltinLen();
    /*
     * dup() is a system call which creates a copy of the file descriptor, in this case 1, 
     * which is the file descriptor for output. It uses the lowest-numbered unused file descriptor 
     * for the new descriptor which it return.
     * I am creating a temp now, so I can revert it to systemOut afterwrards.
     */
    int tmpout = dup(1);
    int tmpin  = dup(0);
    //file descriptor for the changing the outfile.
    int fdout; 
    int fdin;

    if(globalCommand.infile)
        fdin = open(globalCommand.infile, O_RDONLY);
    else
        fdin = dup(tmpin);
    
    for(int i = 0; i<=globalCommand.numberOfSimpleCommands; i++){

        //Setting the input for the first command.
        dup2(fdin, 0);
        close(fdin);
        
        if (i==globalCommand.numberOfSimpleCommands){
            if(globalCommand.outfile)
                /*
                 * so open() work like this
                 * int open(const char *pathname, int flags, mode_t mode);
                 * pathname is the output file
                 * flags, the set file accourdingly
                 *  O_CREAT: creates the file if not present
                 *  O_RDWR: read/write
                 * mode: so read:    4
                 *          wirte:   2
                 *          execute: 1  
                 *
                 */
                fdout = open(globalCommand.outfile, O_CREAT|O_RDWR, 0644); 
            else
                fdout = dup(tmpout);

        }
        else{
            /*
             * If it's not the last command, we create a pipe b/w the commmands.
             *  pipe(int pipefd[2])
             *      creates a unidirectional data channel, used for interprocess communication.
             *      pipefd array is used to retunr two file descriptors referring to the ends of the pipe.
             *      pipefd[0] - input end.
             *      pipefd[1] - output end.
             * 
             * on success - 0
             * on error - -1
             */
            int pipefd[2];
            if(pipe(pipefd) != 0){
                perror("myShell: pipe");
                return 0;
            }
            fdin = pipefd[0];
            fdout = pipefd[1];
        }

        //Now to redirect the output.
        dup2(fdout,1);
        close(fdout);
        
        int BUILTIN_FLAG = 0;
        printf("command:%s\n", globalCommand.sCommands[i].arguments[0]);
        for(int j = 0; j < builtinLen; j++){
            if(globalCommand.sCommands[i].arguments[0] != NULL &&strcmp(globalCommand.sCommands[i].arguments[0], builtin_commands[j]) == 0){
                BUILTIN_FLAG = 1;
                status = (*builtin_cmd_funcs[j])(globalCommand.sCommands[i].arguments);
                printf("status: %d\n", status);
            }
        }
        if(!BUILTIN_FLAG)
            status = execute(i);

    }

    dup2(tmpin,0);
    dup2(tmpin,1);
    close(tmpin);
    close(tmpout);
    freeGlobalCommand();
    return status;
}


int execute(int cmdNumber){
    pid_t pid;
    int status;

    pid = fork();

    if(pid == 0){
        if(execvp(globalCommand.sCommands[cmdNumber].arguments[0], globalCommand.sCommands[cmdNumber].arguments) == -1){
            perror("myShell");
            //exit(EXIT_FAILURE);
        }
    }
    else if(pid < 0){
        //Error forking.
        perror("myShell");
    }
    else{
            do{
                /* pid: If > 0, then waits for the child with process id value equal to pid.
                 * WUNTRACED: also return if a child has stopped 
                 * status: the integer is returned by waitpid,
                 *        and can be inspected my macros like WIFEXITED, WIFSIGNALED, etc.
                 */ 
                waitpid(pid, &status, WUNTRACED);

               /* 
                * WIFEXITED: returns true fi the child terminated normally.
                * WIFSIGNALED: returns true if the chidl process was teminated by a signal.
                *
                */ 
            }while(!WIFEXITED(status) && !WIFSIGNALED(status) && !globalCommand.background);
    }
    return 1;
}
    
