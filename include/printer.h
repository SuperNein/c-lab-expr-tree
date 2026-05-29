#ifndef PRINTER_H
#define PRINTER_H

#include <stdio.h>

#include "expr_tree.h"
#include "poly.h"


void print_poly(
    FILE *stream,
    Polynomial poly
);

void print_expr_tree(
    FILE *stream,
    ExprTree tree
);

void print_expr_inline(
    FILE *stream,
    ExprTree tree
);

#endif
