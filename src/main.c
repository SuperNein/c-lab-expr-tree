#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "errors.h"
#include "parser.h"
#include "printer.h"
#include "tokenizer.h"
#include "tok_vector.h"
#include "expr_tree.h"

#define MAX_LINE_LENGTH 256


static int read_line(
    char *buffer,
    const size_t size
) {
    if (fgets(buffer, size, stdin) == NULL) {
        return 0;
    }

    size_t len = strlen(buffer);

    while (
        len > 0
        && (
            buffer[len - 1] == '\n'
            || buffer[len - 1] == '\r'
        )
    ) {
        buffer[len - 1] = '\0';
        len--;
    }

    return len > 0;
}


static int input_interface(
    char *line,
    const size_t size,
    const int index
) {
    if (isatty(STDIN_FILENO)) {
        printf(
            "Enter polynomial %d: ",
            index
        );

        fflush(stdout);
    }

    if (!read_line(line, size)) {
        fprintf(
            stderr,
            "Failed to read input.\n"
        );

        return 0;
    }

    return 1;
}


static void process_expression(
    const char *expr
) {
    TokVector tokens =
        tok_vector_create();

    ErrorCode tok_err =
        tokenize(expr, &tokens);

    if (tok_err != ERR_OK) {
        fprintf(
            stderr,
            "Tokenizer error: %d\n",
            tok_err
        );

        tok_vector_destroy(&tokens);

        return;
    }

    ParseResult result =
        parse(&tokens);

    if (result.error != ERR_OK) {
        fprintf(
            stderr,
            "Parser error: %d\n",
            result.error
        );

        tok_vector_destroy(&tokens);

        return;
    }

    expr_print_tree(
        stdout,
        result.tree
    );

    expr_tree_destroy(result.tree);
    tok_vector_destroy(&tokens);
}


int main(void) {
    char line1[MAX_LINE_LENGTH];
    char line2[MAX_LINE_LENGTH];

    if (!input_interface(
        line1,
        MAX_LINE_LENGTH,
        1
    )) {
        return 1;
    }

    if (!input_interface(
        line2,
        MAX_LINE_LENGTH,
        2
    )) {
        return 1;
    }

    printf("\nExpression 1 tree:\n\n");
    process_expression(line1);

    printf("\nExpression 2 tree:\n\n");
    process_expression(line2);

    return 0;
}
