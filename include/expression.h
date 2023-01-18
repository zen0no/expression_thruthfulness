#ifndef EXPRESSION_EXPRESSION_H
#define EXPRESSION_EXPRESSION_H

#include <stdbool.h>


#include "var_table.h"

enum expression_type{
    EXPRESSION_BINOP,
    EXPRESSION_UNOP,
    EXPRESSION_VAR
};

struct expression_binop{
    struct expression *left;
    struct expression *right;
    
    bool (*action)(bool, bool);

};

struct expression_unop{
    struct expression *child;
    bool (*action)(bool);
};

struct expression_var{
    struct var_table_entry *entry;
};


struct expression{
    enum expression_type type;
    state _cached_state;
    bool _cached_value;
    bool _is_cached;
    union {
        struct expression_binop* as_binop;
        struct expression_unop* as_unop;
        struct expression_var* as_var;
    };
};

struct expression* disjunction(struct expression *left, struct expression *right);
struct expression* conjunction(struct expression *left, struct expression *right);
struct expression* implication(struct expression *left, struct expression *right);
struct expression* negatiation(struct expression *child);
struct expression* variable(struct var_table_entry *entry);

bool calculate(struct expression* expression);


bool expression_free(struct expression* expression);
#endif