#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "tok_stack.h"

#define INITIAL_CAPACITY 64


void tok_stack_create(TokStack *s) {
    assert(s != NULL);
    
    s->capacity = INITIAL_CAPACITY;
    s->size = 0;
    
    s->data = malloc(s->capacity * sizeof(Token));
    assert(s->data);
}

void tok_stack_push(TokStack *s, Token node) {
    assert(s != NULL);

    if (s->size >= s->capacity) {
        size_t new_capacity = s->capacity * 2;
        Token *new_data = (Token*) realloc(
            s->data,
            new_capacity * sizeof(Token)
        );
        
        assert(new_data);
        
        s->data = new_data;
        s->capacity = new_capacity;
    }

    s->data[s->size] = node;
    s->size++;
}

Token tok_stack_pop(TokStack *s) {
    assert(s != NULL && s->size > 0);
    
    s->size--;
    return s->data[s->size];
}

Token tok_stack_peek(const TokStack *s) {
    assert(s != NULL && s->size > 0);

    return s->data[s->size - 1];
}

bool tok_stack_is_empty(const TokStack *s) {
    assert(s != NULL);
    return s->size == 0;
}

size_t tok_stack_size(const TokStack *s) {
    assert(s != NULL);
    return s->size;
}

void tok_stack_destroy(TokStack *s) {
    if (s == NULL) return;
    
    if (s->data != NULL) {
        free(s->data);
        s->data = NULL;
    }
    
    s->size = 0;
    s->capacity = 0;
}
