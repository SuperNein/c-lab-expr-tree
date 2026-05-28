#include <stdio.h>

#include "printer.h"


static void print_expr_impl(
    FILE *stream,
    ExprTree tree,
    int parent_prec
) {
    if (tree == NULL) {
        return;
    }

    Token tok = tree->token;

    if (!tok_is_operator(tok)) {
        tok_print(tok);
        return;
    }

    int prec = tok_precedence(tok);

    bool need_paren =
        prec < parent_prec;

    if (need_paren) {
        fprintf(stream, "(");
    }

    if (tok.data.op == '~') {
        fprintf(stream, "-");

        print_expr_impl(
            stream,
            tree->right,
            prec
        );
    }
    else {
        print_expr_impl(
            stream,
            tree->left,
            prec
        );

        if (tok.data.op == '~') {
            fprintf(stream, "-");
        }
        else {
            tok_print(tok);
        }

        int right_prec = prec;

        if (
            tok.data.op == '^'
            || tok.data.op == '-'
            || tok.data.op == '/'
        ) {
            right_prec++;
        }

        print_expr_impl(
            stream,
            tree->right,
            right_prec
        );
    }

    if (need_paren) {
        fprintf(stream, ")");
    }
}


void expr_print(
    FILE *stream,
    ExprTree tree
) {
    if (tree == NULL) {
        fprintf(stream, "(empty)");
        return;
    }

    print_expr_impl(
        stream,
        tree,
        0
    );
}


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


void expr_print_tree(
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
