#include <assert.h>
#include <stdlib.h>

#include "expr_vector.h"


ExprVector expr_vector_create() {
    ExprVector vector;

    vector.size = 0;
    vector.capacity = 0;
    vector.data = NULL;

    return vector;
}

void expr_vector_destroy(ExprVector *vector) {
    assert(vector != NULL);

    free(vector->data);

    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
}

bool expr_vector_is_empty(const ExprVector *vector) {
    assert(vector != NULL);

    return vector->size == 0;
}

size_t expr_vector_size(const ExprVector *vector) {
    assert(vector != NULL);

    return vector->size;
}

ExprTree expr_vector_get(const ExprVector* vector, size_t index) {
    assert(vector != NULL);
    assert(index < vector->size);

    return vector->data[index];
}

bool expr_vector_push(ExprVector* vector, ExprTree item) {
    assert(vector != NULL);

    if (vector->size >= vector->capacity) {
        size_t new_capacity;

        if (vector->capacity == 0) {
            new_capacity = 1;
        }
        else {
            new_capacity = vector->capacity * 2;
        }

        ExprTree *new_data =
            (ExprTree*) realloc(vector->data, new_capacity * sizeof(ExprTree));

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

void expr_vector_set(ExprVector* vector, size_t index, ExprTree item) {
    assert(vector != NULL);
    assert(index < vector->size);

    vector->data[index] = item;
}
