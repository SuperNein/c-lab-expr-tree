#include <assert.h>
#include <stdlib.h>

#include "tok_vector.h"


TokVector tok_vector_create() {
    TokVector vector;

    vector.size = 0;
    vector.capacity = 0;
    vector.data = NULL;

    return vector;
}

void tok_vector_destroy(TokVector *vector) {
    assert(vector != NULL);

    free(vector->data);

    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
}

bool tok_vector_is_empty(const TokVector *vector) {
    assert(vector != NULL);

    return vector->size == 0;
}

size_t tok_vector_size(const TokVector *vector) {
    assert(vector != NULL);

    return vector->size;
}

Token tok_vector_get(const TokVector* vector, size_t index) {
    assert(vector != NULL);
    assert(index < vector->size);

    return vector->data[index];
}

bool tok_vector_push(TokVector* vector, Token item) {
    assert(vector != NULL);

    if (vector->size >= vector->capacity) {
        size_t new_capacity;

        if (vector->capacity == 0) {
            new_capacity = 1;
        }
        else {
            new_capacity = vector->capacity * 2;
        }

        Token *new_data =
            (Token*) realloc(vector->data, new_capacity * sizeof(Token));

        if (new_data == NULL) {
            return false;
        }

        vector->data = new_data;
        vector->capacity = new_capacity;
    }

    vector->data[vector->size] = item;
    vector->size++;

    return true;
}

void tok_vector_set(TokVector* vector, size_t index, Token item) {
    assert(vector != NULL);
    assert(index < vector->size);

    vector->data[index] = item;
}
