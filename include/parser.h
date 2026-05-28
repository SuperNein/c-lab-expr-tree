#ifndef PARSER_H
#define PARSER_H

#include "errors.h"
#include "expr_tree.h"
#include "tok_vector.h"


typedef struct {
    ErrorCode error;
    ExprTree tree;
} ParseResult;

ParseResult parse(const TokVector *tokens);

#endif
