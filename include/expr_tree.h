#ifndef EXPR_TREE_H
#define EXPR_TREE_H

#include "token.h"

typedef struct ExprNode ExprNode;
typedef ExprNode *ExprTree;

struct ExprNode {
    Token token;

    ExprTree left;
    ExprTree right;
};

ExprTree expr_tree_create(Token token);

ExprTree expr_tree_make(
    Token token,
    ExprTree left,
    ExprTree right
);

void expr_tree_destroy(ExprTree tree);

bool expr_tree_is_leaf(const ExprTree tree);

ExprTree expr_tree_copy(ExprTree tree);

#endif
