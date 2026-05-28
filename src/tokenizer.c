#include <ctype.h>
#include <stdlib.h>

#include "tokenizer.h"


static bool is_operator(const char ch) {
    switch (ch) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
        case '(':
        case ')':
            return true;

        default:
            return false;
    }
}

static bool is_unary_position(const TokVector *tokens) {
    if (tok_vector_is_empty(tokens)) {
        return true;
    }

    Token prev = tok_vector_get(
        tokens,
        tok_vector_size(tokens) - 1
    );

    if (!tok_is_operator(prev)) {
        return false;
    }

    return prev.data.op != ')';
}

ErrorCode tokenize(
    const char *expr,
    TokVector *tokens
) {
    if (expr == NULL || tokens == NULL) {
        return ERR_INVALID_EXPR;
    }

    size_t i = 0;

    while (expr[i] != '\0') {
        char ch = expr[i];

        if (isspace(ch)) {
            i++;
            continue;
        }

        if (
            isdigit(ch)
            || (ch == '.' && isdigit(expr[i + 1]))
        ) {
            char *end;

            double value = strtod(
                expr + i,
                &end
            );

            if (!tok_vector_push(
                tokens,
                tok_create_value(value)
            )) {
                return ERR_MEMORY;
            }

            i = end - expr;

            continue;
        }

        if (isalpha(ch) || ch == '_') {
            char name[12];
            size_t len = 0;

            while (
                isalnum(expr[i])
                || expr[i] == '_'
            ) {
                if (len + 1 < sizeof(name)) {
                    name[len++] = expr[i];
                }

                i++;
            }

            name[len] = '\0';

            if (!tok_vector_push(
                tokens,
                tok_create_name(name)
            )) {
                return ERR_MEMORY;
            }

            continue;
        }

        if (is_operator(ch)) {
            if (
                ch == '-'
                && is_unary_position(tokens)
            ) {
                ch = '~';
            }

            if (!tok_vector_push(
                tokens,
                tok_create_operator(ch)
            )) {
                return ERR_MEMORY;
            }

            i++;

            continue;
        }

        return ERR_UNKNOWN_TOKEN;
    }

    return ERR_OK;
}
