#include <math.h>
#include <stddef.h>
#include <string.h>

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


static bool root_is_value(ExprTree tree) {
    return tree != NULL && tok_is_value(tree->token);
}


static double root_value(ExprTree tree) {
    return tree->token.data.value;
}


static ExprTree create_value(const double value) {
    return expr_tree_create(
        tok_create_value(value)
    );
}

ExprTree poly_simplify_constants(const ExprTree tree) {
    if (tree == NULL) {
        return NULL;
    }

    if (expr_tree_is_leaf(tree)) {
        return expr_tree_copy(tree);
    }

    ExprTree left = poly_simplify_constants(tree->left);
    ExprTree right = poly_simplify_constants(tree->right);

    char op = tree->token.data.op;

    if (
        root_is_value(left)
        && root_is_value(right)
    ) {
        double a = root_value(left);
        double b = root_value(right);

        double result = 0;

        switch (op) {
            case '+':
                result = a + b;
                break;

            case '-':
                result = a - b;
                break;

            case '*':
                result = a * b;
                break;

            case '/':
                result = a / b;
                break;

            case '^':
                result = pow(a, b);
                break;

            default:
                return create_binary(
                    op,
                    left,
                    right
                );
        }

        expr_tree_destroy(left);
        expr_tree_destroy(right);

        return create_value(result);
    }

    if (op == '*') {
        if (
            root_is_value(left)
            && root_value(left) == 0
        ) {
            expr_tree_destroy(left);
            expr_tree_destroy(right);

            return create_value(0);
        }

        if (
            root_is_value(right)
            && root_value(right) == 0
        ) {
            expr_tree_destroy(left);
            expr_tree_destroy(right);

            return create_value(0);
        }

        if (
            root_is_value(left)
            && root_value(left) == 1
        ) {
            expr_tree_destroy(left);

            return right;
        }

        if (
            root_is_value(right)
            && root_value(right) == 1
        ) {
            expr_tree_destroy(right);

            return left;
        }
    }

    if (op == '+') {
        if (
            root_is_value(left)
            && root_value(left) == 0
        ) {
            expr_tree_destroy(left);

            return right;
        }

        if (
            root_is_value(right)
            && root_value(right) == 0
        ) {
            expr_tree_destroy(right);

            return left;
        }
    }

    if (op == '^') {
        if (
            root_is_value(right)
            && root_value(right) == 0
        ) {
            expr_tree_destroy(left);
            expr_tree_destroy(right);

            return create_value(1);
        }

        if (
            root_is_value(right)
            && root_value(right) == 1
        ) {
            expr_tree_destroy(right);

            return left;
        }
    }

    return create_binary(
        op,
        left,
        right
    );
}


static bool same_variable(
    ExprTree a,
    ExprTree b
) {
    if (
        a == NULL
        || b == NULL
    ) {
        return false;
    }

    if (
        !tok_is_name(a->token)
        || !tok_is_name(b->token)
    ) {
        return false;
    }

    return strcmp(
        a->token.data.name,
        b->token.data.name
    ) == 0;
}

ExprTree poly_simplify_algebraic(const ExprTree tree) {
    if (tree == NULL) {
        return NULL;
    }

    if (expr_tree_is_leaf(tree)) {
        return expr_tree_copy(tree);
    }

    ExprTree left = poly_simplify_algebraic(
            tree->left
        );

    ExprTree right = poly_simplify_algebraic(
            tree->right
        );

    char op = tree->token.data.op;

    if (op == '*') {
        // x * x -> x^2
        if (same_variable(left, right)) {
            ExprTree result = create_binary(
                    '^',
                    expr_tree_copy(left),
                    create_value(2)
                );

            expr_tree_destroy(left);
            expr_tree_destroy(right);

            return result;
        }

        // x^a * x -> x^(a+1)
        if (
            root_is_operator(left, '^')
            && tok_is_name(left->left->token)
            && same_variable(
                left->left,
                right
            )
            && root_is_value(left->right)
        ) {
            double power = root_value(left->right);

            ExprTree result = create_binary(
                    '^',
                    expr_tree_copy(
                        left->left
                    ),
                    create_value(
                        power + 1
                    )
                );

            expr_tree_destroy(left);
            expr_tree_destroy(right);

            return result;
        }

        // x * x^a -> x^(a+1)
        if (
            root_is_operator(right, '^')
            && tok_is_name(right->left->token)
            && same_variable(
                left,
                right->left
            )
            && root_is_value(right->right)
        ) {
            double power = root_value(right->right);

            ExprTree result = create_binary(
                    '^',
                    expr_tree_copy(
                        right->left
                    ),
                    create_value(
                        power + 1
                    )
                );

            expr_tree_destroy(left);
            expr_tree_destroy(right);

            return result;
        }

        // x^a * x^b -> x^(a+b)
        if (
            root_is_operator(left, '^')
            && root_is_operator(right, '^')
            && same_variable(
                left->left,
                right->left
            )
            && root_is_value(left->right)
            && root_is_value(right->right)
        ) {
            double a = root_value(left->right);
            double b = root_value(right->right);

            ExprTree result = create_binary(
                    '^',
                    expr_tree_copy(
                        left->left
                    ),
                    create_value(a + b)
                );

            expr_tree_destroy(left);
            expr_tree_destroy(right);

            return result;
        }
    }

    return create_binary(
        op,
        left,
        right
    );
}

ExprTree poly_normalize(const ExprTree tree) {
    if (tree == NULL) {
        return NULL;
    }

    ExprTree expanded = poly_expand(tree);

    ExprTree simplified = poly_simplify_constants(
            expanded
        );

    ExprTree algebraic = poly_simplify_algebraic(
            simplified
        );

    expr_tree_destroy(expanded);
    expr_tree_destroy(simplified);

    return algebraic;
}
