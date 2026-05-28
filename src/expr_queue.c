#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "expr_queue.h"

#define INIT_CAPACITY 4
#define CAPACITY_GROWTH 2


void expr_queue_create(ExprQueue *q) {
    q->capacity = INIT_CAPACITY;

    q->items = (ExprTree*) malloc(q->capacity * sizeof(ExprTree));
    assert(q->items);

    q->front = 0;
    q->back = 0;
    q->size = 0;
}

bool expr_queue_is_empty(const ExprQueue *q) {
    return q->size == 0;
}

void expr_queue_push(ExprQueue *q, const ExprTree item) {
    if (q->size == q->capacity) {
        size_t new_capacity = q->capacity * CAPACITY_GROWTH;

        ExprTree *new_items = (ExprTree*) malloc(new_capacity * sizeof(ExprTree));
        assert(new_items);

        for (size_t i = 0; i < q->size; ++i) {
            size_t old_index = (q->front + i) % q->capacity;
            new_items[i] = q->items[old_index];
        }

        free(q->items);

        q->capacity = new_capacity;
        q->items = new_items;

        q->front = 0;
        q->back = q->size;
    }

    q->items[q->back] = item;
    q->back = (q->back + 1) % q->capacity;
    q->size++;
}

ExprTree expr_queue_pop(ExprQueue *q) {
    assert(!expr_queue_is_empty(q));

    ExprTree item = q->items[q->front];

    q->front = (q->front + 1) % q->capacity;
    q->size--;

    return item;
}

size_t expr_queue_size(const ExprQueue *q) {
    return q->size;
}

ExprTree expr_queue_peek(const ExprQueue *q) {
    assert(!expr_queue_is_empty(q));

    return q->items[q->front];
}

void expr_queue_destroy(ExprQueue *q) {
    free(q->items);

    q->items = NULL;
    q->size = 0;
    q->capacity = 0;
    q->front = 0;
    q->back = 0;
}
