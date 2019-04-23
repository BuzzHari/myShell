#define TOK_BUFSIZE 64 
#define PATH_MAX 4096

struct SimpleCommand{
    //int _numberOfAvailableArguments;
    
    /*Number of Argumnets*/
    int numberOfArguments;

    /*Array of commands and arguments*/
    char **arguments;

    int bufsize;

}command;

void myShellLoop(void);
void initializeShell(void);
void initializeSimpleCommand(void);
void insertArguments(char *args);
void printCurrentCommand(void);
int executeShellCommand(void);
int execute(void);   









