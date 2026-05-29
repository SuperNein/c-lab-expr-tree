#include <stdio.h>

#include "printer.h"


static void print_tree_impl(
    FILE *stream,
    ExprTree tree,
    const char *prefix,
    bool is_left
) {
    if (tree == NULL) {
        return;
    }

    fprintf(stream, "%s", prefix);

    fprintf(
        stream,
        "%s",
        is_left ? "├── " : "└── "
    );

    if (
        tok_is_operator(tree->token)
        && tree->token.data.op == '~'
    ) {
        fprintf(stream, "-");
    }
    else {
        tok_print(tree->token);
    }

    fprintf(stream, "\n");

    char child_prefix[256];

    snprintf(
        child_prefix,
        sizeof(child_prefix),
        "%s%s",
        prefix,
        is_left ? "│   " : "    "
    );

    if (
        tree->left != NULL
        || tree->right != NULL
    ) {
        print_tree_impl(
            stream,
            tree->left,
            child_prefix,
            true
        );

        print_tree_impl(
            stream,
            tree->right,
            child_prefix,
            false
        );
    }
}


void print_expr_tree(
    FILE *stream,
    ExprTree tree
) {
    if (tree == NULL) {
        fprintf(stream, "(empty tree)\n");
        return;
    }

    if (
        tok_is_operator(tree->token)
        && tree->token.data.op == '~'
    ) {
        fprintf(stream, "-\n");
    }
    else {
        tok_print(tree->token);
        fprintf(stream, "\n");
    }

    print_tree_impl(
        stream,
        tree->left,
        "",
        true
    );

    print_tree_impl(
        stream,
        tree->right,
        "",
        false
    );
}

void print_expr_inline(
    FILE *stream,
    ExprTree tree
) {
    if (tree == NULL) {
        return;
    }

    Token tok = tree->token;

    if (tok_is_value(tok)) {
        fprintf(
            stream,
            "%g",
            tok.data.value
        );

        return;
    }

    if (tok_is_name(tok)) {
        fprintf(
            stream,
            "%s",
            tok.data.name
        );

        return;
    }

    if (tok_is_operator(tok)) {
        // unary
        if (tok.data.op == '~') {
            fprintf(stream, "-");

            print_expr_inline(
                stream,
                tree->left
            );

            return;
        }

        fprintf(stream, "(");

        print_expr_inline(
            stream,
            tree->left
        );

        fprintf(
            stream,
            " %c ",
            tok.data.op
        );

        print_expr_inline(
            stream,
            tree->right
        );

        fprintf(stream, ")");

        return;
    }
}

void print_poly(
    FILE *stream,
    Polynomial poly
) {
    bool first = true;

    for (
        size_t i = poly.size;
        i > 0;
        --i
    ) {
        const size_t power = i - 1;

        ExprTree coef = expr_vector_get(
                &poly,
                power
            );

        if (!coef) {
            continue;
        }

        if (!first) {
            fprintf(stream, " + ");
        }

        first = false;

        fprintf(stream, "(");

        print_expr_inline(
            stream,
            coef
        );

        fprintf(stream, ")");

        if (power > 0) {
            fprintf(stream, "x");

            if (power > 1) {
                fprintf(
                    stream,
                    "^%zu",
                    power
                );
            }
        }
    }

    if (first) {
        fprintf(stream, "0");
    }
}
