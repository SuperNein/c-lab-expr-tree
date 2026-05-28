#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOK_VAL,
    TOK_OP,
    TOK_NAME,
} TokenType;

typedef struct {
    TokenType type;

    union {
        double value;       // TOK_VAL
        char op;            // TOK_OP:   * + - / ^ ~ ( )
        char name[12];      // TOK_NAME
    } data;

} Token;

Token tok_create_value(double value);
Token tok_create_operator(char op);
Token tok_create_name(const char *name);

bool tok_is_operator(Token tok);
bool tok_is_value(Token tok);
bool tok_is_name(Token tok);

bool tok_is_left_paren(Token tok);
bool tok_is_right_paren(Token tok);

bool tok_is_left_assoc(Token tok);

int tok_precedence(Token tok);

void tok_print(Token tok);

#endif
