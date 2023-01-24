#include <malloc.h>

#include "hash_map.h"

struct hm_item{
    uint64_t key;
    var_id value;
    struct hm_item* next;
};

static uint64_t hash_function(char* str)
{
    uint64_t i = 5381;

    for (int j = 0; str[j]; j++)
        i = ((i << 5) + i) + str[j];

    return i;
}

static struct hm_item* hm_item_create(name key, var_id value){
    struct hm_item* item = (struct hm_item*) malloc(sizeof(struct hm_item));
    item->key = hash_function(key);
    item->value = value;
    item->next = NULL;
    
    return item;
}

static bool hm_item_find_by_key(struct hm_item *item, uint64_t hash, struct hm_item **dist){    
    struct hm_item *next = item;
    struct hm_item *cur_item;

    
    while (next){
        cur_item = next;
        next = cur_item->next;

        if (item->key == hash) 
        {
            *dist = cur_item;
            return true;
        }
    }
    return false;
}


struct hash_map* hash_map_create(size_t size){
    struct hash_map* map = (struct hash_map*) malloc(sizeof(struct hash_map));
    map->size = size;
    map->items = (struct hm_item**) calloc(map->size, sizeof(struct hm_item*));

    for (int i = 0; i < map->size; i++)
        map->items[i] = NULL;

    return map;
}

bool hash_map_insert_item(struct hash_map* hmap, name key, var_id value){

    struct hm_item *item = hm_item_create(key, value);
    if (!item) return false;

    uint64_t hash = item->key % (hmap->size);
    item->next = hmap->items[hash];
    hmap->items[hash] = item;
    
    return true;
}

static bool hm_item_free(struct hm_item* item){
    if (!item) return false;
    struct hm_item *next = item;
    struct hm_item *cur_item;

    while(next){
        cur_item = next;
        next = cur_item->next;
        free(cur_item);
    }
    return true;
}

bool hash_map_get(struct hash_map *hmap, name key, var_id *dist){
    size_t hash = hash_function(key);
    size_t index = hash  % (hmap->size);
    struct hm_item *item;

    if (!hm_item_find_by_key(hmap->items[index], hash, &item)) return false;

    *dist = item->value;
    return true;
}



bool hash_map_free(struct hash_map *hmap){
    if (!hmap) return false;
    for (size_t i = 0; i < hmap->size; i++){
        hm_item_free(hmap->items[i]);
    }
    free(hmap->items);
    free(hmap);
    return true;
}


void hash_map_print(struct hash_map *hmap){
    printf("---------------------hash_map-----------------------\n");
    for (size_t i = 0; i < hmap->size; i++){
        printf("i: %zu; ", i);
        for (struct hm_item *item = hmap->items[i]; item; item = item->next){
            printf("hash: %"PRIu64" id: %d", item->key, item->value);
        }
        printf("\n");
    }
}
