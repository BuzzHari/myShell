#define TOK_BUFSIZE 64 
#define PATH_MAX 4096


typedef struct SimpleCommand{
    int numberOfArguments;
    //size of the command.  
    int bufsize;
    /*Array of commands and arguments*/
    char **arguments;
}sCommand;

typedef struct Command{
    
    int numberOfSimpleCommands; 
    //to run the cmd in the background or not. 
    int background;
    char *outfile;
    char *infile;
    char *errfile;
    struct SimpleCommand *sCommands; 
}gCommand;

extern gCommand globalCommand;

void myShellLoop(void);
void initializeShell(void);
void initializeGlobalCommand(void);
void insertArguments(char *args);
void printCurrentCommand(void);
void freeGlobalCommand(void);
int executeShellCommand(void);
int execute(int cmdNumber);   









