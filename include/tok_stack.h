#ifndef TOK_STACK_H
#define TOK_STACK_H

#include "token.h"

typedef struct {
    Token *data;

    size_t size;
    size_t capacity;
} TokStack;

void tok_stack_create(TokStack *s);
void tok_stack_destroy(TokStack *s);

size_t tok_stack_size(const TokStack *s);
bool tok_stack_is_empty(const TokStack *s);

Token tok_stack_peek(const TokStack *s);

void tok_stack_push(TokStack *s, Token node);
Token tok_stack_pop(TokStack *s);

#endif
