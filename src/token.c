#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "token.h"


Token tok_create_value(const double value) {
    Token tok;

    tok.type = TOK_VAL;
    tok.data.value = value;

    return tok;
}

Token tok_create_operator(const char op) {
    Token tok;

    tok.type = TOK_OP;
    tok.data.op = op;

    return tok;
}

Token tok_create_name(const char *name) {
    Token tok;

    tok.type = TOK_NAME;

    strncpy(tok.data.name, name, sizeof(tok.data.name) - 1);
    tok.data.name[sizeof(tok.data.name) - 1] = '\0';

    return tok;
}


bool tok_is_operator(const Token tok) {
    return tok.type == TOK_OP;
}

bool tok_is_value(const Token tok) {
    return tok.type == TOK_VAL;
}

bool tok_is_name(const Token tok) {
    return tok.type == TOK_NAME;
}


bool tok_is_left_paren(const Token tok) {
    return tok_is_operator(tok) && tok.data.op == '(';
}

bool tok_is_right_paren(const Token tok) {
    return tok_is_operator(tok) && tok.data.op == ')';
}


int tok_precedence(const Token tok) {
    assert(tok_is_operator(tok));

    switch (tok.data.op) {
        case '(':
        case ')':
            return 0;

        case '+':
        case '-':
            return 1;

        case '*':
        case '/':
            return 2;

        case '^':
            return 3;

        case '~':
            return 4;

        default:
            return -1;
    }
}

bool tok_is_left_assoc(const Token tok) {
    assert(tok_is_operator(tok));

    switch (tok.data.op) {
        case '~':
        case '^':
            return false;

        default:
            return true;
    }
}

void tok_print(const Token tok) {
    switch (tok.type) {
        case TOK_VAL:
            printf("%g", tok.data.value);
            break;

        case TOK_OP:
            printf("%c", tok.data.op);
            break;

        case TOK_NAME:
            printf("%s", tok.data.name);
            break;

        default:
            printf("<unknown>");
            break;
    }
}
