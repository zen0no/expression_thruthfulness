#include <inttypes.h>
#include <malloc.h>

#include "expression.h"
struct expression expression_create_empty(){
    return (struct expression) {._cached_state=0, ._cached_value=0, ._is_cached=0};
}

static struct expression* expression_binop_create(char* sym, struct expression* left, struct expression* right, enum expression_binop_type action){
    struct expression_binop* binop = (struct expression_binop*) malloc(sizeof(struct expression_binop));
    binop->left = left;
    binop->right = right;
    binop->action = action;
    binop->sym = sym;

    struct expression* expression = (struct expression*) malloc(sizeof(struct expression));
    expression->type=EXPRESSION_BINOP;
    expression->as_binop = binop;

    return expression;

}

static struct expression* expression_unop_create(struct expression* child, enum expression_unop_type action){
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


struct expression* disjunction(struct expression *left, struct expression *right){
    return expression_binop_create("|", left, right, BINOP_OR);
}

struct expression* conjunction(struct expression *left, struct expression *right){
    return expression_binop_create("&", left, right, BINOP_AND);
}

struct expression* implication(struct expression *left, struct expression *right){
    return expression_binop_create("->", left, right, BINOP_IMP);
}

struct expression* negatiation(struct expression *child){
    return expression_unop_create(child, UNOP_NEG);
}

struct expression* variable(struct var_table_entry *entry){
    return expression_var_create(entry);
}

static uint8_t calculate_binop(struct expression_binop *binop){
    switch (binop->action)
    {
    case BINOP_AND:
        return calculate(binop->left) & calculate(binop->right);

    case BINOP_OR:
        return calculate(binop->left) | calculate(binop->right);
    
    case BINOP_IMP:
        return (!calculate(binop->left)) | calculate(binop->right);
        
    default:
        return 0;
    };
}


static uint8_t calculate_unop(struct expression_unop *unop){
    switch (unop->action)
    {
    case UNOP_NEG:
        return !(calculate(unop->child));
    default:
        return 0;
    }
}

static uint8_t calculate_var(struct expression_var *var){
    return var_table_entry_value(var->entry);
}

uint8_t calculate(struct expression* expression){
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


/* PRINT EXPRESSION */

static void binop_print(struct expression *expression, int level);
static void unop_print(struct expression *expression, int level);
static void var_print(struct expression *expression, int level);

static void expression_print_tab(struct expression *expression, int level){
    switch (expression->type)
    {
    case EXPRESSION_BINOP:
        binop_print(expression, level);
        break;
    case EXPRESSION_UNOP:
        unop_print(expression, level);
        break;
    case EXPRESSION_VAR:
        var_print(expression, level);
        break;
    default:
        break;
    }
}

static void binop_print(struct expression *expression, int level){
    for (int i = 0; i < level; i++) printf("\t");
    printf("%s:\n",  expression->as_binop->sym);
    expression_print_tab(expression->as_binop->left, level + 1);
    expression_print_tab(expression->as_binop->right, level + 1);

}

static void unop_print(struct expression *expression, int level){
    for (int i = 0; i < level; i++) printf("\t");
    printf("!:\n");
    expression_print_tab(expression->as_unop->child, level + 1);
}

static void var_print(struct expression *expression, int level){
    for (int i = 0; i < level; i++) printf("\t");
    printf("(id: %d)\n", expression->as_var->entry->_id);

}

void expression_print(struct expression *expression){
    expression_print_tab(expression, 0);
}