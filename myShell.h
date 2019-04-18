#define TOK_BUFSIZE 64 

struct SimpleCommand{
    //int _numberOfAvailableArguments;
    
    /*Number of Argumnets*/
    int numberOfArguments;

    /*Array of commands and arguments*/
    char **arguments;

    int bufsize;

}SIMPLE_COMMAND;

void initializeSimpleCommand();
void insertArguments(char *args);
void printCurrentCommand();
    











