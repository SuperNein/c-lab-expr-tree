#ifndef PRINTER_H
#define PRINTER_H

#include <stdio.h>

#include "expr_tree.h"
#include "expr_vector.h"


void expr_print(
    FILE *stream,
    ExprTree tree
);

void expr_print_tree(
    FILE *stream,
    ExprTree tree
);

#endif
