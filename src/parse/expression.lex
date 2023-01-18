%{
#include "expression.h"
#include "parse/expression.tab.h"
%}


%option outfile="lex.c" header-file="lex.h" noinput nounput

white [ \t]+
digit [0-9]
letter [A-Z`'’]
var {letter}({letter}|{digit})*

%%
{var} {
    yylval.name = yytext;
    return NAME;
}
"&"  return AND;
"|"  return OR;
"->" return IMP;
"!"  return NEG;
"("  return LEFT;
")"  return RIGHT;
"\n" return END;
%%