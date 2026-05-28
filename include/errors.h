#ifndef ERRORS_H
#define ERRORS_H

typedef enum {
    ERR_OK = 0,

    ERR_MEMORY,
    ERR_EMPTY,
    ERR_UNKNOWN_TOKEN,
    ERR_INVALID_EXPR,
    ERR_ZERO_DIVISION,
    ERR_MISS_PAREN,
} ErrorCode;

#endif
