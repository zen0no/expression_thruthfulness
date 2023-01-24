#ifndef EXPRESSION_VAR_TABLE_H
#define EXPRESSION_VAR_TABLE_H

#include <inttypes.h>
#include <stdbool.h>

#define VAR_TABLE_CAPACITY 16

typedef uint8_t var_id;
typedef char* name;
typedef uint64_t state;


struct var_table{
    state _state;
    var_id _next_id;
    struct hash_map *varmap;
};

struct var_table_entry{
    var_id  _id;
    struct var_table *_parent;
};
const struct var_table var_table_create();

bool var_table_set_state(struct var_table *table, state _state);

struct var_table_entry* var_table_var_for_name(struct var_table *table, name _name);

bool var_table_entry_value(struct var_table_entry *entry);
size_t var_table_size(struct var_table *table);

void var_table_print(struct var_table *table);

#endif