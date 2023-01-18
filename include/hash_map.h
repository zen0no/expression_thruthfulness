#ifndef EXPRESSION_HASH_MAP_H
#define EXPRESSION_HASH_MAP_H

#include <inttypes.h>
#include <string.h>
#include <stdbool.h>


#include "var_table.h"

#define CAPACITY 2000

struct hm_item;

struct hash_map{
    uint16_t size;
    struct hm_item **items;
};

struct hash_map* hash_map_create(size_t size);

bool hash_map_insert_item(struct hash_map* hmap, name key, var_id value);

bool hash_map_get(struct hash_map* hmap, name key, var_id *dist);

bool hash_map_free(struct hash_map *hmap);

#endif