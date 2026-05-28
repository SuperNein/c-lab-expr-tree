#include <stdlib.h>
#include <assert.h>

#include "expr_tree.h"
#include "expr_stack.h"


ExprTree expr_tree_create(const Token token) {
    ExprTree tree = (ExprTree) malloc(sizeof(ExprNode));
    assert(tree);

    tree->token = token;

    tree->left = NULL;
    tree->right = NULL;

    return tree;
}

ExprTree expr_tree_make(
    const Token token,
    ExprTree left,
    ExprTree right
) {
    ExprTree tree = expr_tree_create(token);

    tree->left = left;
    tree->right = right;

    return tree;
}

bool expr_tree_is_leaf(const ExprTree tree) {
    assert(tree);

    return tree->left == NULL
        && tree->right == NULL;
}

void expr_tree_destroy(ExprTree tree) {
    if (tree == NULL) {
        return;
    }

    expr_tree_destroy(tree->left);
    expr_tree_destroy(tree->right);

    free(tree);
}


ExprTree expr_tree_copy(ExprTree tree) {
    if (tree == NULL) {
        return NULL;
    }

    ExprTree root_copy =
        expr_tree_create(tree->token);

    ExprStack orig_stack;
    expr_stack_create(&orig_stack);

    ExprStack copy_stack;
    expr_stack_create(&copy_stack);

    expr_stack_push(
        &orig_stack,
        tree
    );

    expr_stack_push(
        &copy_stack,
        root_copy
    );

    while (!expr_stack_is_empty(&orig_stack)) {
        ExprTree orig = expr_stack_pop(&orig_stack);
        ExprTree copy = expr_stack_pop(&copy_stack);

        if (orig->left != NULL) {
            copy->left =
                expr_tree_create(
                    orig->left->token
                );

            expr_stack_push(
                &orig_stack,
                orig->left
            );

            expr_stack_push(
                &copy_stack,
                copy->left
            );
        }

        if (orig->right != NULL) {
            copy->right = expr_tree_create(
                    orig->right->token
                );

            expr_stack_push(
                &orig_stack,
                orig->right
            );

            expr_stack_push(
                &copy_stack,
                copy->right
            );
        }
    }

    expr_stack_destroy(&orig_stack);
    expr_stack_destroy(&copy_stack);

    return root_copy;
}
