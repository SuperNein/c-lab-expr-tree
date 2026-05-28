#include <assert.h>

#include "parser.h"
#include "expr_stack.h"
#include "tok_stack.h"


static bool reduce(
    TokStack *ops,
    ExprStack *exprs
) {
    Token op = tok_stack_pop(ops);

    ExprTree node;

    if (
        tok_is_operator(op)
        && op.data.op == '~'
    ) {
        if (expr_stack_size(exprs) < 1) {
            return false;
        }

        ExprTree operand =
            expr_stack_pop(exprs);

        node = expr_tree_make(
            op,
            NULL,
            operand
        );
    }
    else {
        if (expr_stack_size(exprs) < 2) {
            return false;
        }

        ExprTree right =
            expr_stack_pop(exprs);

        ExprTree left =
            expr_stack_pop(exprs);

        node = expr_tree_make(
            op,
            left,
            right
        );
    }

    expr_stack_push(exprs, node);

    return true;
}


ParseResult parse(const TokVector *tokens) {
    ParseResult result;

    result.error = ERR_OK;
    result.tree = NULL;

    if (
        tokens == NULL
        || tok_vector_is_empty(tokens)
    ) {
        result.error = ERR_EMPTY;
        return result;
    }

    TokStack ops;
    tok_stack_create(&ops);

    ExprStack exprs;
    expr_stack_create(&exprs);

    bool failed = false;

    for (
        size_t i = 0;
        i < tok_vector_size(tokens);
        ++i
    ) {
        Token tok =
            tok_vector_get(tokens, i);

        if (
            tok_is_value(tok)
            || tok_is_name(tok)
        ) {
            ExprTree node =
                expr_tree_create(tok);

            expr_stack_push(
                &exprs,
                node
            );

            continue;
        }

        if (tok_is_left_paren(tok)) {
            tok_stack_push(&ops, tok);
            continue;
        }

        if (tok_is_right_paren(tok)) {
            bool found_left = false;

            while (
                !tok_stack_is_empty(&ops)
            ) {
                Token top =
                    tok_stack_peek(&ops);

                if (
                    tok_is_left_paren(top)
                ) {
                    tok_stack_pop(&ops);

                    found_left = true;
                    break;
                }

                if (!reduce(
                    &ops,
                    &exprs
                )) {
                    result.error =
                        ERR_INVALID_EXPR;

                    failed = true;
                    break;
                }
            }

            if (failed) {
                break;
            }

            if (!found_left) {
                result.error =
                    ERR_MISS_PAREN;

                failed = true;
                break;
            }

            continue;
        }

        if (tok_is_operator(tok)) {
            while (
                !tok_stack_is_empty(&ops)
            ) {
                Token top =
                    tok_stack_peek(&ops);

                if (
                    tok_is_left_paren(top)
                ) {
                    break;
                }

                int top_prec =
                    tok_precedence(top);

                int tok_prec =
                    tok_precedence(tok);

                bool should_reduce =
                    top_prec > tok_prec
                    || (
                        top_prec == tok_prec
                        && tok_is_left_assoc(tok)
                    );

                if (!should_reduce) {
                    break;
                }

                if (!reduce(
                    &ops,
                    &exprs
                )) {
                    result.error =
                        ERR_INVALID_EXPR;

                    failed = true;
                    break;
                }
            }

            if (failed) {
                break;
            }

            tok_stack_push(&ops, tok);
        }
    }

    while (
        !failed
        && !tok_stack_is_empty(&ops)
    ) {
        Token top =
            tok_stack_peek(&ops);

        if (
            tok_is_left_paren(top)
            || tok_is_right_paren(top)
        ) {
            result.error =
                ERR_MISS_PAREN;

            failed = true;
            break;
        }

        if (!reduce(
            &ops,
            &exprs
        )) {
            result.error =
                ERR_INVALID_EXPR;

            failed = true;
            break;
        }
    }

    if (
        !failed
        && expr_stack_size(&exprs) != 1
    ) {
        result.error =
            ERR_INVALID_EXPR;

        failed = true;
    }

    if (!failed) {
        result.tree =
            expr_stack_pop(&exprs);
    }

    tok_stack_destroy(&ops);
    expr_stack_destroy(&exprs);

    return result;
}
