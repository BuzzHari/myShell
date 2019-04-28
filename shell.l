%{
#include<string.h>
#include<stdio.h>
#include<setjmp.h>
#include<signal.h>
#include"parser.tab.h"    

extern void my_yy_input(char*, int *, int);

#undef YY_INPUT
#define YY_INPUT(b,r,s) my_yy_input(b, &r, s)

%}

%option nounput
%option noinput

%%
\n      {return NEWLINE;}

[ \t]   {/* Discards spaces and tabs */}

">"     { return GREAT;}

"<"     {return LESS;}

"|"     {return PIPE;}

"@"     {return CTRL;}

[^ \t\n][^ \t\n]* {
    yylval.str = strdup(yytext); 
    return WORD;
}
%%

/*YYSTYPE yylval; 
int main(void){
    int token;
    while((token = yylex()) != 0)
        printf("Token: %d (%s)\n", token, yytext);
    return 0;
}*/

/*static sigjmp_buf env;*/

/*void sigint_handler(int signo){*/
    /*siglongjmp(env, '@');*/
/*}*/

void my_yy_input(char *buff, int *nRead, int maxToRead){
    
    *nRead = 1;

    
    //signal(SIGINT, sigint_handler);

    /*if(sigsetjmp(env,1) == '@'){
        *buff = '@';
        return;
    }*/
     
    *buff = getchar();
}

