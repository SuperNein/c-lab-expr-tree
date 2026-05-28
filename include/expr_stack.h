#ifndef EXPR_STACK_H
#define EXPR_STACK_H

#include <stddef.h>

#include "expr_tree.h"


typedef struct {
    ExprTree *data;

    size_t size;
    size_t capacity;
} ExprStack;

void expr_stack_create(ExprStack *s);
void expr_stack_destroy(ExprStack *s);

size_t expr_stack_size(const ExprStack *s);
bool expr_stack_is_empty(const ExprStack *s);

ExprTree expr_stack_peek(const ExprStack *s);

void expr_stack_push(ExprStack *s, ExprTree node);
ExprTree expr_stack_pop(ExprStack *s);

#endif