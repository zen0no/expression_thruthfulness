#ifndef EXPRESSION_VECTOR_H
#define EXPRESSION_VECOTR_H

#include <inttypes.h>
#include <stdbool.h>

struct vector;

struct vector* vector_create(uint16_t sz);
bool vector_add(struct vector* vec, uint64_t value);
bool vector_get(struct vector* vec, uint16_t id, uint64_t *dist);
bool get_id_by_value(struct vector *vec, uint64_t id_value, uint16_t *dist);
bool vector_id_by_value(struct vector *vec, uint64_t value, uint16_t *dist);
bool vector_delete(struct vector *vec);
uint16_t vector_get_count(struct vector* vec);

#endif