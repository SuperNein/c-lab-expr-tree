#ifndef TOK_VECTOR_H
#define TOK_VECTOR_H

#include <stddef.h>

#include "token.h"

typedef struct {
    Token *data;

    size_t size;
    size_t capacity;
} TokVector;

TokVector tok_vector_create();
void tok_vector_destroy(TokVector *vector);

size_t tok_vector_size(const TokVector *vector);
bool tok_vector_is_empty(const TokVector *vector);

bool tok_vector_push(TokVector* vector, Token item);
Token tok_vector_get(const TokVector* vector, size_t index);

void tok_vector_set(TokVector* vector, size_t index, Token item);

#endif
