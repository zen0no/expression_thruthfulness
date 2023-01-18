%{
#include <stdio.h>
#include <string.h>


#include "expression.h"
#include "var_table.h"

int yylex(void);
void yyerror(const char *);

struct var_table table;
struct expression* result = NULL;
%}

%union {    
struct expression* e;
name name;
}

%token <name> NAME
%token IMP OR AND NEG
%token LEFT RIGHT
%token END

%right IMP
%left OR
%left AND
%left NEG

%type <e> Input Expression

%start Input

%%
Input
    : Expression { result = $1; }
    ;

Expression
    : Expression IMP Expression { $$ = implication($1, $3); }
    | Expression OR Expression  { $$ = disjunction($1, $3); }
    | Expression AND Expression { $$ = conjunction($1, $3); }
    | NEG Expression            { $$ = negatiation($2);}
    | LEFT Expression RIGHT     { $$ = $2;}
    | NAME {
            $$ = variable(var_table_var_for_name(&table, $1));

    }
    ;

%%