#include <inttypes.h>
#include <malloc.h>

#include "expression.h"

struct expression expression_create_empty(){
    return (struct expression) {._cached_state=0, ._cached_value=0, ._is_cached=0};
}

static struct expression* expression_binop_create(struct expression* left, struct expression* right,bool (*action)(bool, bool)){
    struct expression_binop* binop = (struct expression_binop*) malloc(sizeof(struct expression_binop));
    binop->left = left;
    binop->right = right;
    binop->action = action;

    struct expression* expression = (struct expression*) malloc(sizeof(struct expression));
    expression->type=EXPRESSION_BINOP;
    expression->as_binop = binop;

    return expression;

}

static struct expression* expression_unop_create(struct expression* child, bool (*action)(bool)){
    struct expression_unop* unop = (struct expression_unop*) malloc(sizeof(struct expression_unop));
    unop->child = child;
    unop->action = action;


    struct expression* expression = (struct expression*) malloc(sizeof(struct expression));
    expression->type=EXPRESSION_UNOP;
    expression->as_unop = unop;

    return expression;
}

static struct expression* expression_var_create(struct var_table_entry *entry){
    struct expression_var* var = (struct expression_var*) malloc(sizeof(struct expression_var));
    var->entry = entry;


    struct expression* expression = (struct expression*) malloc(sizeof(struct expression));
    expression->type=EXPRESSION_VAR;
    expression->as_var = var;

    return expression;

}


static bool op_or(bool a, bool b){ return a | b ;}
static bool op_and(bool a, bool b){ return a & b;}
static bool op_not(bool a) {return !a;}
static bool op_impl(bool a, bool b) { return (!a) | b;}


struct expression* disjunction(struct expression *left, struct expression *right){
    return expression_binop_create(left, right, &op_or);
}

struct expression* conjunction(struct expression *left, struct expression *right){
    return expression_binop_create(left, right, &op_and);
}

struct expression* implication(struct expression *left, struct expression *right){
    return expression_binop_create(left, right, &op_impl);
}

struct expression* negatiation(struct expression *child){
    return expression_unop_create(child, &op_not);
}

struct expression* variable(struct var_table_entry *entry){
    return expression_var_create(entry);
}

static bool calculate_binop(struct expression_binop *binop){
    return binop->action(calculate(binop->left), calculate(binop->right));
}


static bool calculate_unop(struct expression_unop *unop){
    return unop->action(calculate(unop->child));
}

static bool calculate_var(struct expression_var *var){
    return var_table_entry_value(var->entry);
}

bool calculate(struct expression* expression){
    switch (expression->type)
    {
    case EXPRESSION_BINOP:
        return calculate_binop(expression->as_binop);

    case EXPRESSION_UNOP:
        return calculate_unop(expression->as_unop);

    case EXPRESSION_VAR:
        return calculate_var(expression->as_var);
    
    default:
        return false;
    }
}