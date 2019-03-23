#define SHELL_COMMAND_SIZE 1024



struct SimpleCommand{
    //int _numberOfAvailableArguments;
    
    /*Number of Argumnets*/
    int _numberOfArguments;

    /*Array of commands and arguments*/
    char **_arguments;
    
    /*function to populate insertArguments*/
    void insertArguments(char *arguments);
    
    SimpleCommand();
    static SimpleCommand _currentSimpleCommand;

}SIMPLE_COMMAND;


void insertArguments(char *args){
    /*
     * Malloc **_arguments. Fill in the tokens we get from
     * parser.
     * The parser, will send tokens from two rules,
     * cmd_and_args and arg_list. 
     * So, this malloc has to be done in another function,
     * before all this. Something like a constructor.
     * If the it reaches the size limit, then there has to 
     * be a function to realloc.
     *
     * Figure what exactly static is in structure, most probably
     * it won't be accessible from outside the structure
     * functions.
     *
     * Figure out the constructor function. Which sets up
     * the structure.
     *
     * I think all this can be done in the main function.
     *
     * So when I have the the arguments filled, 
     * I'll have to figure out if it's a built-in function 
     * or not. A for loop should suffice, 
     *
     * If it's not a builtin func, I'll pass it to execute func,
     * which will in turn fork a new process, and execute it.
     *
     * It it is, I will have this function pointer to call the
     * respective functions. Figure this out properly.
     *
     */
}









