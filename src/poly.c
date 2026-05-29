#include <string.h>
#include <math.h>

#include "poly.h"


static ExprTree make_number(const double value) {
    return expr_tree_create(
        tok_create_value(value)
    );
}


static ExprTree make_operator(
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


static bool is_x(const Token tok) {
    return tok_is_name(tok)
        && strcmp(tok.data.name, "x") == 0;
}

static bool is_const_value(
    ExprTree tree,
    const double value
) {
    return tree
        && tok_is_value(tree->token)
        && tree->token.data.value == value;
}


static bool is_zero(
    ExprTree tree
) {
    return is_const_value(
        tree,
        0
    );
}


static bool is_one(
    ExprTree tree
) {
    return is_const_value(
        tree,
        1
    );
}

static void poly_ensure_size(
    Polynomial *poly,
    const size_t size
) {
    while (poly->size < size) {
        expr_vector_push(poly, NULL);
    }
}


static ExprTree poly_get_safe(
    const Polynomial *poly,
    const size_t index
) {
    if (index >= poly->size) {
        return NULL;
    }

    return expr_vector_get(poly, index);
}


Polynomial poly_add(
    const Polynomial *a,
    const Polynomial *b
) {
    Polynomial result =
        expr_vector_create();

    const size_t max_size =
        a->size > b->size
        ? a->size
        : b->size;

    poly_ensure_size(
        &result,
        max_size
    );

    for (size_t i = 0; i < max_size; ++i) {
        ExprTree left = poly_get_safe(a, i);
        ExprTree right = poly_get_safe(b, i);

        if (left && right) {
            expr_vector_set(
                &result,
                i,
                make_operator(
                    '+',
                    expr_tree_copy(left),
                    expr_tree_copy(right)
                )
            );
        }
        else if (left) {
            expr_vector_set(
                &result,
                i,
                expr_tree_copy(left)
            );
        }
        else if (right) {
            expr_vector_set(
                &result,
                i,
                expr_tree_copy(right)
            );
        }
    }

    return result;
}


Polynomial poly_mul(
    const Polynomial *a,
    const Polynomial *b
) {
    Polynomial result = expr_vector_create();

    if (
        a->size == 0 ||
        b->size == 0
    ) {
        return result;
    }

    poly_ensure_size(
        &result,
        a->size + b->size - 1
    );

    for (size_t i = 0; i < a->size; ++i) {
        ExprTree left = poly_get_safe(a, i);

        if (!left) {
            continue;
        }

        for (size_t j = 0; j < b->size; ++j) {
            ExprTree right = poly_get_safe(b, j);

            if (!right) {
                continue;
            }

            ExprTree product = make_operator(
                    '*',
                    expr_tree_copy(left),
                    expr_tree_copy(right)
                );

            ExprTree current = expr_vector_get(
                    &result,
                    i + j
                );

            if (!current) {
                expr_vector_set(
                    &result,
                    i + j,
                    product
                );
            }
            else {

                ExprTree sum = make_operator(
                        '+',
                        expr_tree_copy(current),
                        product
                    );

                expr_tree_destroy(current);

                expr_vector_set(
                    &result,
                    i + j,
                    sum
                );
            }
        }
    }

    return result;
}


Polynomial poly_pow(
    const Polynomial *poly,
    const int power
) {
    Polynomial result = expr_vector_create();

    poly_ensure_size(
        &result,
        1
    );

    expr_vector_set(
        &result,
        0,
        make_number(1)
    );

    for (int i = 0; i < power; ++i) {
        Polynomial temp = poly_mul(
                &result,
                poly
            );

        expr_vector_destroy(&result);

        result = temp;
    }

    return result;
}


Polynomial poly_from_expr(ExprTree tree) {
    Polynomial result = expr_vector_create();

    if (!tree) {
        return result;
    }

    Token tok = tree->token;

    // const
    if (
        tok_is_value(tok) ||
        (
            tok_is_name(tok) &&
            !is_x(tok)
        )
    ) {
        poly_ensure_size(
            &result,
            1
        );

        expr_vector_set(
            &result,
            0,
            expr_tree_copy(tree)
        );

        return result;
    }

    // x
    if (is_x(tok)) {
        poly_ensure_size(
            &result,
            2
        );

        expr_vector_set(
            &result,
            1,
            make_number(1)
        );

        return result;
    }

    // operator
    if (tok_is_operator(tok)) {
        Polynomial left = poly_from_expr(
                tree->left
            );

        Polynomial right = poly_from_expr(
                tree->right
            );

        switch (tok.data.op) {
            case '+': {
                Polynomial res = poly_add(
                        &left,
                        &right
                    );

                expr_vector_destroy(&left);
                expr_vector_destroy(&right);

                return res;
            }

            case '-': {
                Polynomial neg = expr_vector_create();

                poly_ensure_size(
                    &neg,
                    right.size
                );

                for (
                    size_t i = 0;
                    i < right.size;
                    ++i
                ) {
                    ExprTree coef = expr_vector_get(
                            &right,
                            i
                        );

                    if (!coef) {
                        continue;
                    }
                    expr_vector_set(
                        &neg,
                        i,
                        make_operator(
                            '*',
                            make_number(-1),
                            expr_tree_copy(coef)
                        )
                    );
                }

                Polynomial res = poly_add(
                        &left,
                        &neg
                    );

                expr_vector_destroy(&left);
                expr_vector_destroy(&right);
                expr_vector_destroy(&neg);

                return res;
            }

            case '*': {
                Polynomial res = poly_mul(
                        &left,
                        &right
                    );

                expr_vector_destroy(&left);
                expr_vector_destroy(&right);

                return res;
            }

            case '^': {
                if (
                    !tree->right ||
                    !tok_is_value(
                        tree->right->token
                    )
                ) {

                    expr_vector_destroy(&left);
                    expr_vector_destroy(&right);

                    return result;
                }

                double raw_power =
                    tree->right
                        ->token
                        .data
                        .value;

                if (
                    floor(raw_power)
                    != raw_power
                ) {

                    expr_vector_destroy(&left);
                    expr_vector_destroy(&right);

                    return result;
                }

                int power = (int)raw_power;

                if (power < 0) {
                    expr_vector_destroy(&left);
                    expr_vector_destroy(&right);

                    return result;
                }

                Polynomial res = poly_pow(
                        &left,
                        power
                    );

                expr_vector_destroy(&left);
                expr_vector_destroy(&right);

                return res;
            }

            default:
                expr_vector_destroy(&left);
                expr_vector_destroy(&right);

                return result;
        }
    }

    return result;
}


static ExprTree simplify_expr(ExprTree tree) {
    if (!tree) {
        return NULL;
    }

    Token tok = tree->token;

    if (
        tok_is_value(tok)
        || tok_is_name(tok)
    ) {
        return expr_tree_copy(tree);
    }

    ExprTree left = simplify_expr(tree->left);
    ExprTree right = simplify_expr(tree->right);

    // unary
    if (
        tok_is_operator(tok)
        && tok.data.op == '~'
    ) {
        if (
            left &&
            tok_is_value(left->token)
        ) {
            double value = left->token.data.value;

            expr_tree_destroy(left);

            return make_number(-value);
        }

        return make_operator(
            '~',
            left,
            NULL
        );
    }

    if (!tok_is_operator(tok)) {
        expr_tree_destroy(left);
        expr_tree_destroy(right);

        return expr_tree_copy(tree);
    }

    char op = tok.data.op;

    // constant
    if (
        left
        && right
        && tok_is_value(left->token)
        && tok_is_value(right->token)
    ) {
        double a = left->token.data.value;

        double b = right->token.data.value;

        expr_tree_destroy(left);
        expr_tree_destroy(right);

        switch (op) {
            case '+':
                return make_number(a + b);

            case '-':
                return make_number(a - b);

            case '*':
                return make_number(a * b);

            case '^':
                return make_number(
                    pow(a, b)
                );
        }
    }

    switch (op) {
        case '+': {
            if (is_zero(left)) {
                expr_tree_destroy(left);
                return right;
            }

            if (is_zero(right)) {
                expr_tree_destroy(right);
                return left;
            }

            break;
        }

        case '-': {
            if (is_zero(right)) {
                expr_tree_destroy(right);
                return left;
            }

            break;
        }

        case '*': {
            if (
                is_zero(left)
                || is_zero(right)
            ) {
                expr_tree_destroy(left);
                expr_tree_destroy(right);

                return make_number(0);
            }

            if (is_const_value(left, -1)) {
                expr_tree_destroy(left);

                return make_operator(
                    '~',
                    right,
                    NULL
                );
            }

            if (is_const_value(right, -1)) {
                expr_tree_destroy(right);

                return make_operator(
                    '~',
                    left,
                    NULL
                );
            }

            if (is_one(left)) {
                expr_tree_destroy(left);

                return right;
            }

            if (is_one(right)) {
                expr_tree_destroy(right);

                return left;
            }

            break;
        }

        case '^': {
            if (is_zero(right)) {
                expr_tree_destroy(left);
                expr_tree_destroy(right);

                return make_number(1);
            }

            if (is_one(right)) {
                expr_tree_destroy(right);

                return left;
            }

            break;
        }
    }

    return make_operator(
        op,
        left,
        right
    );
}


void poly_normalize(Polynomial *poly) {
    for (
        size_t i = 0;
        i < poly->size;
        ++i
    ) {
        ExprTree coef = expr_vector_get(
                poly,
                i
            );

        if (!coef) {
            continue;
        }

        ExprTree simplified = simplify_expr(coef);

        expr_tree_destroy(coef);

        expr_vector_set(
            poly,
            i,
            simplified
        );
    }

    while (
        poly->size > 0 &&
        expr_vector_get(
            poly,
            poly->size - 1
        ) == NULL
    ) {
        poly->size--;
    }
}
