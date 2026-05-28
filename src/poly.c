#include <stddef.h>

#include "poly.h"


static bool root_is_operator(
    ExprTree tree,
    const char op
) {
    return tree != NULL
        && tok_is_operator(tree->token)
        && tree->token.data.op == op;
}


static ExprTree create_binary(
    const char op,
    ExprTree left,
    ExprTree right
) {
    return expr_tree_make(
        tok_create_operator(op),
        left,
        right
    );
}


static ExprTree expand_mul(
    ExprTree left,
    ExprTree right
) {
    if (root_is_operator(left, '+')) {
        ExprTree ac = poly_expand(
            create_binary(
                '*',
                expr_tree_copy(left->left),
                expr_tree_copy(right)
            )
        );

        ExprTree bc = poly_expand(
            create_binary(
                '*',
                expr_tree_copy(left->right),
                expr_tree_copy(right)
            )
        );

        return create_binary(
            '+',
            ac,
            bc
        );
    }

    if (root_is_operator(right, '+')) {
        ExprTree ab = poly_expand(
            create_binary(
                '*',
                expr_tree_copy(left),
                expr_tree_copy(right->left)
            )
        );

        ExprTree ac = poly_expand(
            create_binary(
                '*',
                expr_tree_copy(left),
                expr_tree_copy(right->right)
            )
        );

        return create_binary(
            '+',
            ab,
            ac
        );
    }

    return create_binary(
        '*',
        left,
        right
    );
}


ExprTree poly_expand(const ExprTree tree) {
    if (tree == NULL) {
        return NULL;
    }

    if (expr_tree_is_leaf(tree)) {
        return expr_tree_copy(tree);
    }

    ExprTree left =
        poly_expand(tree->left);

    ExprTree right =
        poly_expand(tree->right);

    if (root_is_operator(tree, '*')) {
        return expand_mul(
            left,
            right
        );
    }

    return expr_tree_make(
        tree->token,
        left,
        right
    );
}
