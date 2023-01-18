#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>


#include "expression.h"
#include "hash_map.h"
#include "parse/expression.lexer.h"
#include "parse/expression.tab.h"
#include "var_table.h"


extern struct expression* result;
extern struct var_table table;

void yyerror(const char* error){
    if (!strcmp(error, "Success")) perror(error);
}

int yywrap() {
    return 1;
}

int main() {
    char expression[256];
    if (!fgets(expression, sizeof expression, stdin)) {
        return -1;
    }



    table = var_table_create();
    yy_scan_string(expression);
    yyparse();

    size_t t = 0;
    size_t f = 0;

    var_id size = var_table_size(&table);
    for (state i = 0; i < (1 << size); i++){
        var_table_set_state(&table, i);
        bool res = calculate(result);
        if (res) t++;
        else f++;
    }

    if (f == 0) printf("Valid\n");
    else if (t == 0) printf("Unsatisfiable\n");
    else printf("Satisfiable and invalid, %zu true and %zu false cases\n", t, f);

    return 0;
}