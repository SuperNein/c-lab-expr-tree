#ifndef POLY_H
#define POLY_H

#include "expr_tree.h"
#include "expr_vector.h"


typedef ExprVector Polynomial;

Polynomial poly_from_expr(ExprTree tree);

Polynomial poly_add(
    const Polynomial *a,
    const Polynomial *b
);

Polynomial poly_mul(
    const Polynomial *a,
    const Polynomial *b
);

Polynomial poly_pow(
    const Polynomial *p,
    int power
);

void poly_normalize(Polynomial *p);

#endif
