#include "vector.h"

#include <inttypes.h>
#include <malloc.h>
#include <stdbool.h>

struct vector{
    uint64_t *data;
    uint16_t count;
    uint16_t size;
};


struct vector* vector_create(uint16_t sz){
    struct vector* vec = (struct vector*) malloc(sizeof(struct vector));
    vec->data = malloc(sz  * sizeof(uint64_t));
    vec->size = sz;

    return vec;
}

bool vector_add(struct vector* vec, uint64_t value){
    if (vec->count == vec->size) return false;


    vec->data[vec->count++] = value;

    return true;
}

bool vector_get(struct vector* vec, uint16_t id, uint64_t *dist){
    if (id > vec->count) return false;

    *dist = vec->data[id];

    return true;
}

bool vector_id_by_value(struct vector *vec, uint64_t value, uint16_t *dist){
    for (uint16_t i = 0; i < vec->count; i++){
        if (vec->data[i] == value) {*dist = i; return true;}
    }
    
    return false;
}

bool vector_delete(struct vector *vec){
    free(vec->data);
    free(vec);
    return true;
}

uint16_t vector_get_count(struct vector* vec){
    return vec->count;
}
