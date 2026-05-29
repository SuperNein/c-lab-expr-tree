#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "errors.h"
#include "parser.h"
#include "printer.h"
#include "tokenizer.h"
#include "tok_vector.h"
#include "expr_tree.h"
#include "poly.h"

#define MAX_LINE_LENGTH 256


static int read_line(
    FILE *stream,
    char *buffer,
    const size_t size
) {
    if (fgets(buffer, size, stream) == NULL) {
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
    FILE *stream,
    char *line,
    const size_t size,
    const int index
) {
    if (stream == stdin && isatty(STDIN_FILENO)) {
        printf(
            "Enter polynomial %d: ",
            index
        );

        fflush(stdout);
    }

    if (!read_line(
        stream,
        line,
        size
    )) {
        fprintf(
            stderr,
            "Failed to read input.\n"
        );

        return 0;
    }

    return 1;
}


static Polynomial process_expression(const char *expr) {
    TokVector tokens = tok_vector_create();

    ErrorCode tok_err = tokenize(expr, &tokens);

    if (tok_err != ERR_OK) {
        fprintf(
            stderr,
            "Tokenizer error: %d\n",
            tok_err
        );

        tok_vector_destroy(&tokens);

        return expr_vector_create();
    }

    ParseResult result = parse(&tokens);

    if (result.error != ERR_OK) {
        fprintf(
            stderr,
            "Parser error: %d\n",
            result.error
        );

        tok_vector_destroy(&tokens);

        return expr_vector_create();
    }

    Polynomial poly = poly_from_expr(result.tree);

    poly_normalize(&poly);

    print_poly(
        stdout,
        poly
    );

    printf("\n");

    expr_tree_destroy(result.tree);
    tok_vector_destroy(&tokens);

    return poly;
}


int main(
    const int argc,
    char *argv[]
) {
    FILE *input = stdin;

    if (argc > 1) {
        input = fopen(argv[1], "r");

        if (input == NULL) {
            perror("fopen");

            return 1;
        }
    }

    char line1[MAX_LINE_LENGTH];
    char line2[MAX_LINE_LENGTH];

    if (!input_interface(
        input,
        line1,
        MAX_LINE_LENGTH,
        1
    )) {
        if (input != stdin) {
            fclose(input);
        }

        return 1;
    }

    if (!input_interface(
        input,
        line2,
        MAX_LINE_LENGTH,
        2
    )) {
        if (input != stdin) {
            fclose(input);
        }

        return 1;
    }

    printf("\nExpression 1:\n");

    Polynomial poly1 = process_expression(line1);

    printf("\nExpression 2:\n");

    Polynomial poly2 = process_expression(line2);

    Polynomial result = poly_mul(
            &poly1,
            &poly2
        );

    poly_normalize(&result);

    printf(
        "\nMultiplication result:\n"
    );

    print_poly(
        stdout,
        result
    );

    printf("\n");

    expr_vector_destroy(&poly1);
    expr_vector_destroy(&poly2);
    expr_vector_destroy(&result);

    if (input != stdin) {
        fclose(input);
    }

    return 0;
}
