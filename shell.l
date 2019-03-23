%{
#include<string.h>
#include<stdio.h>
#include"parser.tab.h"    
%}


%%
\n      {return NEWLINE;}
[ \t]   {/* Discards spaces and tabs */}
[^ \t\n][^ \t\n]* {
    yylval.str = strdup(yytext); // Don't forget to free it!!!
    return WORD;
}
%%
