%{
#include<string.h>
#include<stdio.h>
#include"parser.tab.h"    
%}

%option nounput
%option noinput

%%
\n      {return NEWLINE;}

[ \t]   {/* Discards spaces and tabs */}

">"     { return GREAT;}

"<"     {return LESS;}

"|"     {return PIPE;}

[^ \t\n][^ \t\n]* {
    yylval.str = strdup(yytext); // Don't forget to free it!!!
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
