#include <malloc.h>

#include "hash_map.h"
#include "var_table.h"



const struct var_table var_table_create(){
    state _state = 0;
    var_id _next_id = 0;
    struct hash_map* varmap = hash_map_create(VAR_TABLE_CAPACITY);
    
    const struct var_table table = (const struct var_table) {._state=_state, ._next_id=_next_id, .varmap=varmap};
    

    return table;
}

static struct var_table_entry* var_table_entry_create(var_id _id, struct var_table *parent ){
    struct var_table_entry* entry = (struct var_table_entry*) malloc(sizeof(struct var_table_entry));
    entry->_id = _id;
    entry->_parent = parent;

    return entry;
}

struct var_table_entry* var_table_var_for_name(struct var_table *table, name _name){
    var_id _id;
    if (!hash_map_get(table->varmap, _name, &_id)){
        _id = (table->_next_id)++;
        hash_map_insert_item(table->varmap, _name, _id);
    }

    return var_table_entry_create(_id, table);
}

static bool var_table_value_for_id(struct var_table *table, var_id _id){
    return (table->_state) & (1 << _id);
}


bool var_table_entry_value(struct var_table_entry *entry){
    return var_table_value_for_id(entry->_parent, entry->_id);
}


bool var_table_set_state(struct var_table *table, state _state){
    if (!table){
        return false;
    }

    table->_state = _state;
    return true;
}


size_t var_table_size(struct var_table *table){
    return table->_next_id;
}