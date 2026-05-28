#ifndef TOK_VECTOR_H
#define TOK_VECTOR_H

#include <stddef.h>

#include "expr_tree.h"


typedef struct {
    ExprTree *data;

    size_t size;
    size_t capacity;
} ExprVector;

ExprVector expr_vector_create();
void expr_vector_destroy(ExprVector *vector);

size_t expr_vector_size(const ExprVector *vector);
bool expr_vector_is_empty(const ExprVector *vector);

bool expr_vector_push(ExprVector* vector, ExprTree item);
ExprTree expr_vector_get(const ExprVector* vector, size_t index);

void expr_vector_set(ExprVector* vector, size_t index, ExprTree item);

#endif
