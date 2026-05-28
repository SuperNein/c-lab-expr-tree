#ifndef EXPR_QUEUE_H
#define EXPR_QUEUE_H

#include <stddef.h>

#include "expr_tree.h"


typedef struct {
    ExprTree *items;

    size_t front;
    size_t back;

    size_t size;
    size_t capacity;
} ExprQueue;

void expr_queue_create(ExprQueue *q);
void expr_queue_destroy(ExprQueue *q);

bool expr_queue_is_empty(const ExprQueue *q);
size_t expr_queue_size(const ExprQueue *q);

ExprTree expr_queue_peek(const ExprQueue *q);

void expr_queue_push(ExprQueue *q, ExprTree item);
ExprTree expr_queue_pop(ExprQueue *q);

#endif
