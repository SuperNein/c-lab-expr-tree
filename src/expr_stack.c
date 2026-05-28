#include <stdlib.h>
#include <assert.h>

#include "expr_stack.h"

#define INITIAL_CAPACITY 64


void expr_stack_create(ExprStack *s) {
    assert(s != NULL);
    
    s->capacity = INITIAL_CAPACITY;
    s->size = 0;
    
    s->data = malloc(s->capacity * sizeof(ExprTree));
    assert(s->data);
}

ExprTree expr_stack_peek(const ExprStack *s) {
    assert(s != NULL && s->size > 0);

    return s->data[s->size - 1];
}

void expr_stack_push(ExprStack *s, ExprTree node) {
    assert(s != NULL);

    if (s->size >= s->capacity) {
        size_t new_capacity = s->capacity * 2;
        ExprTree *new_data = (ExprTree*) realloc(
            s->data,
            new_capacity * sizeof(ExprTree)
        );
        
        assert(new_data);
        
        s->data = new_data;
        s->capacity = new_capacity;
    }

    s->data[s->size] = node;
    s->size++;
}

ExprTree expr_stack_pop(ExprStack *s) {
    assert(s != NULL && s->size > 0);
    
    s->size--;
    return s->data[s->size];
}

bool expr_stack_is_empty(const ExprStack *s) {
    assert(s != NULL);
    return s->size == 0;
}

size_t expr_stack_size(const ExprStack *s) {
    assert(s != NULL);
    return s->size;
}

void expr_stack_destroy(ExprStack *s) {
    if (s == NULL) return;
    
    if (s->data != NULL) {
        free(s->data);
        s->data = NULL;
    }
    
    s->size = 0;
    s->capacity = 0;
}
