#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "errors.h"
#include "tok_vector.h"

ErrorCode tokenize(
    const char *expr,
    TokVector *tokens
);

#endif
